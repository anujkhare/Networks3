#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*this function will return a bool value
depending on the message sent by the main
server*/
int checkinmainserver(int q){
int status=-1;

    int sockfd, portno, n,u;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
     }
    else{
        printf("check 2\n");
    }

    char argv[1]="127.0.0.1" ;

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        //exit(0);
    }
    else{
        printf("check 3\n");
    }
    //bzero((char *) &serv_addr, sizeof(serv_addr));
    //bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    portno = 8889;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");
     }
    else{
        printf("check 4\n");
    }

    bzero(buffer,256);
    sprintf(buffer,"%d",q);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0){
         error("ERROR writing to socket");
     }
    else{
        printf("check 5\n");
    }

    bzero(buffer,256);
    n = read(sockfd,buffer,255);

    if (n < 0){
         error("ERROR reading from socket");
    }
    else{
        printf("check 6\n");
    }
    printf("%s\n",buffer);

    status=atoi(buffer);

return status;
}


/*here maintain a queue to update the status of messages.
maintaining the queue(cache size of 10)*/
int status(int queue[5],int check){
int stats=-1;
int i;
for(i=0;i<5;i++){
    if(check==queue[i]){
        stats=1;
        break;
    }
    else{
        /*call for another function to connect to the other server*/
        return 2;
    }
}

return stats=1;
}

int main(int argc, char *argv[]){
int sockfd, newsockfd, portno, clilen,queue[5],p=0,u;
struct sockaddr_in serv_addr, cli_addr;

printf("initial cache entries:");
for(p=0;p<5;p++){
    scanf("%d",&queue[p]);
    printf("element %d of queue is:%d\n",p,(queue[p]));
}


sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(argc < 2){
         fprintf(stderr,"ERROR, no port provided\n");
         //exit(1);
     }
     else{
        printf("hello and welcome\n");
     }

    if (sockfd < 0){
        printf("ERROR opening socket");
    }
    else{
        printf("check 1\n");
    }

     //bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

    if (bind(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
         printf("ERROR on binding");
    }
    else{
        printf("check 2\n");
    }

     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

     if (newsockfd < 0){
        printf("ERROR on accept");
     }
     else{
        printf("check 3\n");
     }

     /*message=readmessage(newsockfd);
     printf("%s",message);*/

     char buffer[256];
     bzero(buffer,256);
     int k = read(newsockfd,buffer,255);
     if (k < 0){
        printf("ERROR reading from socket");
     }
     else{
        printf("Here is the message: %s\n",buffer);
     }

        int a = atoi(buffer);
        int l = status(queue,a);

        printf("value returned:%d and int obtained is :%d\n",l,a);

        switch(l){
        case 1:printf("found\n");break;
        case 2:printf("notfound - so take particular action by shifting the queue and adding the new element\n");
         u=checkinmainserver(a);
         printf(u);
        break;
        }

        k = write(newsockfd,"found/not found",15);
        if (k < 0){
            error("ERROR writing to socket");
        }
        else{
            printf("successfully replied\n");
        }






return 0;
}
