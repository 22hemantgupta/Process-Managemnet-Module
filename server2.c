/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<fcntl.h> 
#include<pthread.h>
void parse(char *buffer , char **argv1)
{
    while (*buffer!='\0')
    {
        while(*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
        {
            *buffer++ = '\0';
        }
        *argv1++ = buffer;
        while(*buffer != '\0' && *buffer !=' ' && *buffer != '\t' && *buffer !='\n')
        buffer++;
    }
    *argv1 = '\0';
}
void Die(char *mess) 
{
    perror(mess);
    exit(1);
}
void execute(char **argv1)
{
    pid_t  pid;
    int    status;
    int pipefd[2],lenght;
    if(pipe(pipefd))
        Die("Failed to create pipe");
    if ((pid=fork()) < 0) 
    {     // fork a child process           
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if(pid == 0)
    {
        
        /*if(execvp(*argv1,argv1)<0)
        {
            printf("***ERROR***\n");
            exit(1);
        }*/
        //close(1);
        /*dup2(pipefd[1],1);
        close(pipefd[0]); // close the readonly side of the pipe
        close(pipefd[1]); // close the original write side of the pipe
        execvp(argv1[0],argv1); // finally execute the command*/
        while(1)
        {
        int newfd = open("mysercli.txt",O_WRONLY | O_APPEND | O_CREAT , 0644); 
        // here the newfd is the file descriptor of stdout (i.e. 1) 
        dup2(newfd, 1) ;
        execvp(argv1[0],argv1);
        }
    }
    else
    {
        while(wait(&status) !=pid)
        ;
    }  
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     char *argv1[64];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     while(1)
     {
           bzero(buffer,255);
           n = read(newsockfd,buffer,255);
           if (n < 0) error("ERROR reading from socket");
           printf("Client: %s\n",buffer);
          parse(buffer, argv1);       //   parse the line               
          if (strcmp(argv1[0], "exit") == 0)  // is it an "exit"?     
               exit(0);            //   exit if it is                
          execute(argv1); 
          //bzero(buffer,255);
          /*fgets(buffer,255,stdin);
          n = write(newsockfd,buffer,strlen(buffer));
           if (n < 0) error("ERROR writing to socket");
           int i=strncmp("Bye" , buffer, 3);
           if(i == 0)
               break;*/
     }
     close(newsockfd);
     close(sockfd);
     return 0; 
}