#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
void parse(char *buffer, char **argv1)
{
    while (*buffer != '\0')
    {
        while (*buffer == ' ' || *buffer == '\t' || *buffer == '\n')
        {
            *buffer++ = '\0';
        }
        *argv1++ = buffer;
        while (*buffer != '\0' && *buffer != ' ' && *buffer != '\t' && *buffer != '\n')
            buffer++;
    }
    *argv1 = '\0';
}
/*void  execute(char **argv1)
{
    pid_t  pid;
    int    status;
    if ((pid = fork()) < 0) 
    {     // fork a child process           
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if(pid == 0)
    {
        if(execvp(*argv1,argv1)<0)
        {
            printf("***ERROR***\n");
            exit(1);
        }
    }
    else
    {
        while(wait(&status) !=pid)
        ;
    }  
}*/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char *argv1[64];
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    while (1)
    {
        char *argv1[64];
        bzero(buffer, 255);
        printf("Client: ");
        gets(buffer);
        // parse(buffer, argv1);       /*   parse the line               */
        // if (strcmp(argv1[0], "exit") == 0)  /* is it an "exit"?     */
        //    exit(0);            /*   exit if it is                */
        // execute(argv1);
        n = write(sockfd, buffer, strlen(buffer));
        parse(buffer, argv1);
        if (strcmp(buffer, "exit") == 0)
        {
            exit(0);
        }
        if (n < 0)
            error("ERROR writing to socket");

        if (strcmp(argv1[0], "kill") == 0)
        {
            printf("process killed\n");
            continue;
        }
        int j;
        recv(sockfd, &j, sizeof(j), 0);
        printf("process id send by server is %d\n", j);
        //bzero(buffer,255);
        /*n = read(sockfd,buffer,255);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("Server : %s\n",buffer);
        int i = strncmp("Bye" , buffer , 3);
        if(i == 0)
               break;*/
    }
    close(sockfd);
    return 0;
}
