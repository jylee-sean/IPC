#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// for Message queue
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define BUFFER_SIZE 1024

typedef struct { // Message buffer structure
  long msgtype; // Message type, must be > 0 with 'long' data type
  int value;
  char buf[BUFFER_SIZE]; // Message data to push in queue
} msgbuf;

int main() {
    int cnt = 0;
    int key_id; // Message queue ID
    msgbuf msg;
    msg.msgtype = 1;

    key_id = msgget((key_t) 1234, IPC_CREAT|0666); // Create Message (message queue key, message flag)

    if (key_id == -1) {
        printf("Message Get Failed!\n");
        exit(0);
    }
    
    while (1) {
        msg.value = ++cnt;
    
        if (cnt >= 10) {
            printf("Message Sending Finished!\n");
            break;
        }

        if (msgsnd(key_id, &msg, sizeof(msg), IPC_NOWAIT) == -1) { // IPC_NOWAIT flag: if no more queu space, fail instead of blocking
            printf("Message Sending Failed!\n");
            exit(0);
        }
        
        printf("value: %d\n", msg.value);
        sleep(1);
    }
    exit(0);
}



// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/types.h>
// #include <sys/ipc.h> 
// #include <sys/msg.h> 
// #include <sys/stat.h> 

// struct msgbuf
// {
//   // 이부분은 고정!!!!
//   long msgtype;      
//   // 아래 부분은 모두 변경 가능
//   char mtext[256];  
//   char myname[16];
//   int  seq;
// };

// struct msgbuf1
// {
//   // 이부분은 고정!!!!
//   long msgtype;      
//   // 아래 부분은 모두 변경 가능
//   char ip[128];  
//   char host[16];
//   int  seq;
// };


// int main(void)
// {
//   key_t key_id;
//   int i;
//   struct msgbuf mybuf;
//   struct msgbuf1 mybuf1;


//   // 1. Message Queue 할당.
//   key_id = msgget((key_t)1234, IPC_CREAT|0666);
//   if (key_id == -1)
//   {
//     perror("msgget error : ");
//     return 0;
//   }
//   // 2. 할당된 ID 확인
//   printf("Key is %d\n", key_id);

//   mybuf.seq = 0;
//   i = 0;
//   while (1) {
//     memset(&mybuf, 0, sizeof(struct msgbuf));
//     memset(&mybuf1, 0, sizeof(struct msgbuf1));


//     // 짝수일경우 메시지 타입이 4
//     // 홀수일경우에는 메시지 타입이 3
//     if (i % 2 == 0) {
//       mybuf.msgtype = 4;
//       snprintf(mybuf.mtext, sizeof(mybuf.mtext), "hello [%d]",i);
//       snprintf(mybuf.myname, sizeof(mybuf.myname), "name [%d]",i);
//       mybuf1.seq = i;
//       mybuf.seq = i;
//       // 3. 메시지를 전송한다. 
//       if (msgsnd( key_id, (void *)&mybuf, sizeof(struct msgbuf), IPC_NOWAIT) == -1)
//       {
//         perror("msgsnd error : ");
//         return 0;
//       } 
//     } else {
//       mybuf1.msgtype = 3;
//       snprintf(mybuf1.ip, sizeof(mybuf1.ip), "10.0.8.%d",i);
//       snprintf(mybuf1.host, sizeof(mybuf1.host), "ip[%d]",i);
//       mybuf1.seq = i;
//       // 3. 메시지를 전송한다. 
//       if (msgsnd( key_id, (void *)&mybuf1, sizeof(struct msgbuf1), IPC_NOWAIT) == -1)
//       {
//         perror("msgsnd error : ");
//         return 0;
//       } 
//     }

//     printf("send %d\n", i);
//     i++;
//     sleep(1);
//   }
//   return 0;
// }