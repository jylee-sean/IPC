#include <sys/stat.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
 
#define MAXBUF    8 //1024 
 
int main(int argc, char **argv) 
{ 
    struct sockaddr_in serveraddr; 
    int server_sockfd; 
    int client_len; 
	int cnt, icnt;
    char buf[MAXBUF]; 
    char rbuf[MAXBUF]; 
    int n;

    printf("%s\n","version 1");
 
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        perror("error :"); 
        exit(0); 
    } 
 
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = inet_addr("10.12.20.39"); 
//  serveraddr.sin_port = htons(atoi(argv[1])); 
 	serveraddr.sin_port = htons(12345); 
  
    client_len = sizeof(serveraddr); 
 
    if (connect(server_sockfd, (struct sockaddr *)&serveraddr, client_len) < 0) 
    { 
        perror("connect error :"); 
        exit(0); 
    } 
	cnt =0;
	printf("%s \n", "log");
    while(1)
    {
		cnt++;
		if(cnt == 1000)
		{
			printf("%s \n", "clinet read & write");
			cnt = 0;
		}
        memset(buf, 0x00, MAXBUF);  

		printf("size: %lu\n", strlen(buf));


		// int fd = open("file_name");
		// read(fd, "")


        //read(STDIN_FILENO, buf, MAXBUF); // wait ?? == scanf("%s", buf);
        if(read(STDIN_FILENO, buf, MAXBUF) > 0)
        {
		    //file descriptor refer to https://inuplace.tistory.com/129
		    printf("size: %lu\n", strlen(buf));
            if(strlen(buf) > MAXBUF){
                printf("%s\n", "buffer overflow");
                break;
            }
        }else{

            break;
        }

		// what if over size ?
        if (write(server_sockfd, buf, strlen(buf)) <= 0) 
        { 
            perror("write error : "); 
            break;
        }
		else
		{
			//printf("%s \n", "write success");
		}
        printf("read from client buf : %s", buf); 

        // seversocket buffer size 는 sever의 buffer size와는 다름? 크기는 몇?
        memset(buf, 0x00, MAXBUF); 
        if (read(server_sockfd, buf, MAXBUF) <= 0) 
        { 
            perror("read error : "); 
            break;
        }
        else
        {
            //printf("%s \n", "read from server");
            //getchar();
        }
        printf("read from server socket : (%lu) : %s",strlen(buf), buf);

    } 
    close(server_sockfd); 
    return 0;
} 



// read/write가 실행될때마다 자동적으로 file offset이 변경됨

// read 할 때 max buf 해야되는지? O
// write 할때 strlen + 1 할 필요 있는지?  X
// buffer size보다 많이 입력할 경우 ? server측에서는 n번(size/buffer_size) 만큼 돌면서 버퍼 0될때까지 출력
//                               client측에서는 serversocket에 쓰긴하나 (클라이언트 버퍼 최대사이즈 이상부터는 짤려있음)
//                               또한 서버소켓 버퍼에서 데이터 읽으면 서버버퍼사이즈만큼만 읽어와짐
//                               
// client buffer size 보다 server buffer size 가 더 작을 경우? n 번 call

// 데이터가 server buffer size 보다 많을 때 server 가 buffer를 읽는 순서? 순차적으로


// file descriptor 의 buffer size ? limit 은 ?
// abcdefg(\n) ===> 8byte 입력시 사이즈 19 뜨면서 buffer overflow 발생하는 이유??

