//server side code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
void error (const char *msg)
{
    perror(msg);
    exit(1);
}
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Port No not provided. Program terminated\n");
        exit(1);
    }
    int sockfd, newsockfd, portno , n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening Socket.");
    }
    bzero((char*) &serv_addr, sizeof(serv_addr));
    portno = atoi (argv[1]);
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons (portno);
    if(bind (sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Binding Failed");
    
    listen (sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if(newsockfd < 0)
    error("Error on Accept");

    //code start here

    int data_received[10];
    int test[10];
    int p,p1,p2,p4;
    
    //data received will always be correct
    printf("the data is received is: ");
    printf("\nD7 D6 D5 P4 D3 P2 P1\n");

    for(int i=0;i<7;i++){
        n = read(newsockfd,&data_received[i],sizeof(int));
        if(n<0){
            error("Error in receiving data");
            break;
        }
        printf("%d  ",data_received[i]);

    }

    //to demonstrate correction, we take an test input with single bit error
    printf("\nplease enter the data to be tested:\n");
    for(int i=0;i<7;i++){
        scanf("%d",&test[i]);
    }

    //calculate the even parity for encoding
    //xor gives 0-> correct 
    //          1-> incorrect
    //thus here xor gives 0-> even no. of 1's
    //thus here xor gives 1-> odd no. of 1's
    //    P1       D3     D5     D7
    p1= test[6]^test[4]^test[2]^test[0];

    //    P2       D3     D6     D7
    p2= test[5]^test[4]^test[1]^test[0];

    //    P4       D5     D6     D7
    p4= test[3]^test[2]^test[1]^test[0];

    //for calculating the position of error in parity bit
    p = (4*p4)+(2*p2)+p1;
    
    printf("\nthe data  for testing is : \n");
    printf("D7 D6 D5 P4 D3 P2 P1\n");
    for(int i=0;i<7;i++){
        printf("%d  ",test[i]);
    }


    if(p==0){
        printf("no error");
    }
    else{
        printf("\nthe error is at position(parity bit)- %d",p);

        //error correction for the test input
        if(test[7-p]==0){
                test[7-p]=1;
        }
        else{
            test[7-p]=0;
        }
        

        printf("\n\ndata after correction of parity bit \n");
        printf("D7 D6 D5 P4 D3 P2 P1\n");

        for(int i=0;i<7;i++){
            printf("%d  ",test[i]);
        }
    }
    //code end here
    


    close(newsockfd);
    close (sockfd);
    return 0;
}