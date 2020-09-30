/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <bits/stdc++.h>
using namespace std;
//#define atoa(x) #x
int sockfd, newsockfd, portno;
/*void *connection_handler(void *arv)
{
    char **argv2=(char**)arv;
    while(1)
        {
        int newfd = open("mysercli.txt",O_WRONLY | O_APPEND | O_CREAT , 0644); 
        // here the newfd is the file descriptor of stdout (i.e. 1) 
        dup2(newfd, 1) ;
        execvp(argv2[0],argv2);
        }
}*/
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
    *argv1 = NULL;
}
void Die(char *mess)
{
    perror(mess);
    exit(1);
}
int execute(char **argv1)
{
    //cout << " start" << getpid();
    pid_t pid;
    int status;
    if ((pid = fork()) < 0)
    { // fork a child process
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    {
        cout << " hell";
        cout << getppid() << "hi ";
        /*j++;
        if(send(newsockfd, &j, sizeof(j), 0)<0)
          {
              Die("sorry !");
          }
        arr[j]=getpid();
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
        while (1)
        {
            pid_t pid1;
            int status1;
            if ((pid1 = fork()) < 0)
            { // fork a child process
                printf("*** ERROR: forking child process failed\n");
                exit(1);
            }
            else if (pid1 == 0)
            {
                int newfd = open("serverclient1.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
                // here the newfd is the file descriptor of stdout (i.e. 1)
                dup2(newfd, 1);
                if (execvp(*argv1, argv1) < 0)
                {
                    printf("***ERROR***\n");
                    exit(1);
                }
                //close(newfd);
            }
            else
            {
                wait(&status1) != pid1;
            }
            //cout << getpid() << "hi ";
            //return getpid();
        }
        return getpid();
    }
    //return 0;
    /*else
    {
        while(wait(&status) !=pid);
    }*/
}
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    pid_t arr[1000000];
    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *)&cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    int j = 0;
    while (1)
    {
        char *argv1[64];
        char *store[100];
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Client: %s\n", buffer);
        parse(buffer, argv1);              //   parse the line
        if (strcmp(argv1[0], "exit") == 0) // is it an "exit"?
            exit(0);

        //for(int i=0;i<j;i++)
        // {
        //   printf("%d " , arr[i]);
        //}
        if (strcmp(argv1[0], "kill") == 0)
        {
            //itoa(arr[atoi(argv1[1])],argv1[1],10);
            //sprintf(argv1[2], "%d",arr[atoi(argv1[2])]);
            //execvp(argv1[0],argv1);
            /*pid_t pid1;
            int status1;
            if ((pid1 = fork()) < 0)
            { // fork a child process
                printf("*** ERROR: forking child process failed\n");
                exit(1);
            }
            else if (pid1 == 0)
            {*/
            int x = arr[atoi(argv1[1])];
            //printf("%d", x);
            //argv1[1] = "x";
            //argv1[1] = atoa(x);
            char *s;
            if (atoi(argv1[1]) <= j)
            {
                kill(x, SIGKILL);
                //s = "Process Killed Successfully";
            }
            /*else
            {
                s = "Sorry! Enter valid Process Id";
            }
            // fflush();
            if (send(newsockfd, &s, sizeof(s), 0) < 0)
            {
                Die("sorry !");
            }*/
            continue;
            /*}
            else
            {
                wait(&status1) != pid1;
            }*/
        }
        //   exit if it is
        //int thread =pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) j)
        /*if(pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) j)<0)
          {
               perror("could not create thread");
               return 1;
          }   
          //arr[j]=thread;
          int pid=j;*/
        /*pthread_t tid;
          int checking;
        checking = pthread_create(&tid, NULL, connection_handler ,(void *) argv1);
        if (checking !=0)
        perror("create Thread");
        //pthread_exit(NULL);
        arr[j]=tid;
        int pid=j;
        bzero(buffer,255);
          if(send(newsockfd, &pid, sizeof(pid), 0)<0)
          {
              error("sorry !");
          } */
        j++;
        if (send(newsockfd, &j, sizeof(j), 0) < 0)
        {
            Die("sorry !");
        }
        //store[j] = buffer;
        arr[j] = execute(argv1);
        printf("process id : %d\n", arr[j]);
        //bzero(buffer,255);
        /*fgets(buffer,255,stdin);
          n = write(newsockfd,buffer,strlen(buffer));
           if (n < 0) error("ERROR writing to socket");
           int i=strncmp("Bye" , buffer, 3);
           if(i == 0)
               break;*/
    }
    //pthread_exit(NULL);
    close(newsockfd);
    close(sockfd);
    return 0;
}