#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
//#include <anistd.h>

void error_handling(char *msg);
void *send_message(void *arg);
void *recv_message(void *arg);



#define BUFFSIZE 100
char message[BUFFSIZE];


int main(int argc, char **argv)
{
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread, rcv_thread;
    void* thread_result;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;


    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons("7889");

    if(connect(sock, (struct socketaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        printf("connect error");

    unsigned char msg[100] = {0x01, 2,3,4,5,6,1,2,3,4,2,1,2,3,0x0c};
    while(1){
        printf("send: ");

        write(sock,msg,15);
            sleep(1);
    }
    close(sock);

    return 0;


}