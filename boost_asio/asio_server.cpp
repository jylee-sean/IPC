
#include <boost/version.hpp>
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>


using boost::asio::ip::tcp;

class session;

std::unordered_map<int, session> g_clients;
std::atomic_int g_client_id = 0;



class session
{
	int my_id;
	tcp::socket socket_;
	enum {max_length = 1024};
	char data_[max_length];


	public:
	session(): socket_(nullptr)
	{
		std::cout<< "session creation error"<<std::endl;
	}

	session(tcp::socket socket, int id) : socket_(std::move(socket)), my_id(id)
	{
		do_read();
	}

	void do_read()
	{
		socket_.async_read_some(boost::asio::buffer(data_, max_length), 
		[this](boost::system::error_code ec, std::size_t length)
		{
			if(ec){
				std::cout<< "Disconnect client [ " << my_id << " ] "<<std::endl;
				g_clients.erase(my_id); 
			}
			else{
				data_[length] = 0;
				std::cout<< "client [" << my_id << " ]: "<< data_ << std::endl;
				g_clients[my_id].do_write(length);
			}
		});
	}

	void do_write(std::size_t length)
	{
		boost::asio::async_write(socket_, boost::asio::buffer(data_,length),
		[this](boost::system::error_code ec, std::size_t/*length*/)
		{
			if(!ec) g_clients[my_id].do_read();
			else g_clients.erase(my_id);
		} );
	}
};

void accept_callback(boost::system::error_code ec, tcp::socket& socket, tcp::acceptor& my_acceptor)
{
	int new_id = g_client_id++;
	std::cout<< " New Client [ " << new_id << " ] connected"<<std::endl;

	g_clients.try_emplace(new_id, std::move(socket), new_id);

	my_acceptor.async_accept([&my_acceptor](boost::system::error_code ec, tcp::socket socket){
		accept_callback(ec, socket, my_acceptor);
	});
}


int main(int argc, char* argv[])
{

	boost::asio::io_context io_context;
	
	boost::asio::ip::tcp::acceptor my_acceptor{io_context, tcp::endpoint(tcp::v4(), 7777)};
	
	std::cout << "server started at port 7777" << std::endl;

	my_acceptor.async_accept([&my_acceptor](boost::system::error_code ec, tcp::socket socket){
		accept_callback(ec, socket, my_acceptor);
	});

	io_context.run();

	


    return 0;
}

