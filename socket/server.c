#include <sys/socket.h> 
#include <sys/stat.h> 
#include <arpa/inet.h> 
#include <stdio.h> 
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#define MAXBUF  4 //256 
int main(int argc, char **argv) 
{ 
    int server_sockfd, client_sockfd; 
    int client_len, n; 
    int cnt, icnt;
    char buf[MAXBUF]; 
    struct sockaddr_in clientaddr, serveraddr; 
 
    client_len = sizeof(clientaddr); 


    printf("version: %d\n", 3);
    // 소켓 만들기   
    if ((server_sockfd = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
    { 
        perror("socket error : "); 
        exit(0); 
    } 

    // 소켓에 주소를 묶는다.
    bzero(&serveraddr, sizeof(serveraddr)); 
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    //serveraddr.sin_port = htons(atoi(argv[1])); 
    serveraddr.sin_port = htons(12345); 
    bind (server_sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)); 


    // 소켓 연결 대기열을 만든다.
    listen(server_sockfd, 5); 
    cnt = 0;
    icnt =0;
            printf("%s \n", "log");
    while(1) 
    { 
        //printf("%s \n", "waiting accept");
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&clientaddr, 
                            &client_len); 
        printf("%s \n", "accepted");
        while(1)
        {
            memset(buf, 0x00, MAXBUF); 
            //printf("%d %s \n", icnt, "reading & writing");
            
            if ((n = read(client_sockfd, buf, MAXBUF)) <= 0) 
            { 
                close(client_sockfd); 
                break;
            } 
            else
            {
                //printf("%s\n", " read called");
            }
            if (write(client_sockfd, buf, strlen(buf)/*MAXBUF*/) <=0) 
            { 
                perror("write error : "); 

                close(client_sockfd); 
                break;
            } 
            else
            {
                //printf("%s\n", " write called");
            }
            
            n = strlen(buf);
          
            if(n>1){
                if(buf[n-1] =='\n')
                    printf("readA(%lu) : %s",strlen(buf), buf);
                else
                    printf("readB(%lu) : %s\n",strlen(buf), buf);
            }


        }
        close(client_sockfd); 
    } 
} 