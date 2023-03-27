
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// for Message queue
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define BUFFER_SIZE 1024

typedef struct {
    long msgtype;
    int value;
    char buf[BUFFER_SIZE];
} msgbuf;

int main() {
    int key_id;
    msgbuf msg;
    msg.msgtype = 1;
    
    key_id = msgget((key_t) 1234, IPC_CREAT|0666); // Create Message (message queue key, message flag)
    
    if (key_id == -1) {
        printf("Message Get Failed!\n");
        exit(0);
    }
    
    while (1) {
        if (msgrcv(key_id, &msg, sizeof(msg), 1, 0) == -1) { // Receive if msgtype is 1
            printf("Message Receiving Finished!\n");
            exit(0);
        }
        
        printf("value: %d\n", msg.value);
    }

    printf("Message Receiving Finished!\n");
    exit(0);
}


// #include <stdio.h>
// #include <sys/types.h> 
// #include <sys/ipc.h> 
// #include <sys/msg.h> 
// #include <sys/stat.h> 

// struct msgbuf {
//  // 이부분은 고정!!!!
//     long msgtype;
//     // 아래 부분은 모두 변경 가능
//     char mtext[256];
//     char myname[16];
//     int  seq;
// };

// struct msgbuf1 {
//   // 이부분은 고정!!!!
//   long msgtype;      
//   // 아래 부분은 모두 변경 가능
//   char ip[128];  
//   char host[16];
//   int  seq;
// };

// int msgtype;//전역변수로 빼야 됩니다.

// int main(int argc, char **argv)
// {
//     key_t key_id;
//     struct msgbuf mybuf;
//     struct msgbuf1 mybuf1;



//     // 아규먼트가 있을경우 msgtype 가 3인 메시지를 받아오고(홀수) 
//     // 아규먼트가 없을경우 msgtype 가 4인 메시지를 받아온다(짝수)  
//     if (argc == 2)
//         msgtype = 3;
//     else 
//         msgtype = 4;

//     key_id = msgget(1234, IPC_CREAT|0666);
//     if (key_id < 0) {
//         perror("msgget error : ");
//         return 0;
//     }
//     while(1) {
//       if (msgtype == 3) {
//         if (msgrcv( key_id, (void *)&mybuf1, sizeof(struct msgbuf1), msgtype, 0) == -1)
//         {
//           perror("msgrcv error : ");
//           return 0;
//         }
//         printf("%s %d\n",mybuf1.host, mybuf1.seq);
//       } else {
//         if (msgrcv( key_id, (void *)&mybuf, sizeof(struct msgbuf), msgtype, 0) == -1)
//         {
//           perror("msgrcv error : ");
//           return 0;
//         }
//         printf("%s %d\n",mybuf.mtext, mybuf.seq);
//       }
//     }
//     return 0;
// }