
#include <boost/version.hpp>
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <future>


using boost::asio::ip::tcp;


class wall
{
	public:
		static constexpr std::size_t buf_size = 16* 1024u;
		wall(){
		
		}

		void accept_callback(boost::system::error_code ec, tcp::socket& socket, tcp::acceptor& my_acceptor)
		{
			//int new_id = g_client_id++;
			//std::cout<< " New Client [ " << new_id << " ] connected"<<std::endl;
			//g_clients.try_emplace(new_id, std::move(socket), new_id); // 왜 std::move ?

			my_acceptor.async_accept([&my_acceptor,this](boost::system::error_code ec, tcp::socket socket){
				socket.write_some(boost::asio::buffer(_contents));
				accept_callback(ec, socket, my_acceptor);
			});
		}

		bool post(std::string c, const std::string port){
			std::cout<<"post"<<std::endl;
			_contents = c;
			boost::asio::ip::tcp::acceptor my_acceptor{_ioctx, tcp::endpoint(tcp::v4(), std::stoi(port))};

			if(_future.valid()){
				return false;
			}
			_future = std::async(std::launch::async, [&](){
			my_acceptor.async_accept([&my_acceptor, this](boost::system::error_code ec, tcp::socket socket){
				socket.write_some(boost::asio::buffer(_contents));
				accept_callback(ec, socket, my_acceptor);
				});
				_ioctx.run();
			});

			while(true){
				if(_future.wait_for(std::chrono::seconds(1)) == std::future_status::ready){
					return true;
				}
			}

			return true;
		}
	
	private:
		boost::asio::io_context _ioctx; // hw 정보를 읽어옴
		//boost::asio::ip::tcp::socket _socket;
		std::string _contents;
		std::string _port;
		std::future<void> _future;

};

int main(int argc, char* argv[])
{

	wall w;
	w.post("hello world", "10004");

	while(1){

	}

	// boost::asio::io_context io_context;
	// boost::asio::ip::tcp::acceptor my_acceptor{io_context, tcp::endpoint(tcp::v4(), 7777)};
	// std::cout << "server started at port 7777" << std::endl;
	// my_acceptor.async_accept([&my_acceptor](boost::system::error_code ec, tcp::socket socket){
	// 	accept_callback(ec, socket, my_acceptor);
	// });
	// io_context.run();

	


    return 0;
}

