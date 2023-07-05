//client side code

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
void error (const char *msg)
{
    perror (msg);
    exit(0);
}
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer [255];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }
    portno=atoi (argv[2]);
    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname (argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(1);
    }
    bzero ((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family= AF_INET;
    bcopy ((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons (portno);
    if (connect (sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");


    //code start here

    int data[10];
    printf("please input 4 bits data: \n");
    scanf("%d",&data[0]);
    scanf("%d",&data[1]);
    scanf("%d",&data[2]);
    scanf("%d",&data[4]);
    
    //calculate the even parity for encoding
    //xor gives 0-> correct 
    //          1-> incorrect
    //thus here xor gives 0-> even no. of 1's
    //thus here xor gives 1-> odd no. of 1's

    //   P1       D3     D5     D7
    data[6] = data[4]^data[2]^data[0];

    //   P2       D3     D6     D7
    data[5] = data[4]^data[1]^data[0];

    //   P4       D5     D6     D7
    data[3] = data[1]^data[2]^data[0];
    
    //sending the encoded data to client ;
    for(int i=0;i<7;i++){
        n = write(sockfd,&data[i],sizeof(int));
        if(n<0){
            error("error while transmitting the data");
        }
    }
    printf("the data sent is : \n");
    printf("D7 D6 D5 P4 D3 P2 P1\n");
    for(int i=0;i<7;i++){
        printf("%d  ",data[i]);
    }

    //code end here

    close(sockfd);
    return 0;
}