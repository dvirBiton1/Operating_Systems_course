/*
** server.c -- a stream socket server demo
*/

#include "Ex4.hpp"
#include <tbb/mutex.h>
pStackNode head = NULL;
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

void *myThread(void *arg)
{
    char input[1024] = {0};
    int new_fd = *(int *)arg;
    int numbytes;
    // if ((numbytes = recv(new_fd, input, MAXDATASIZE - 1, 0)) == -1)
    // {
    //     perror("recv");
    //     exit(1);
    // }
    // input[numbytes] = '\0';
    // printf("server: received '%s'\n", input);
    // int len = strlen(input);
    while (strcmp(input, "EXIT"))
    {
        char str[1024];
        bzero(str, 1024);
        if (strncmp(input, "PUSH", 4) == 0)
        {
            for (int i = 5, j = 0; i < strlen(input); i++, j++)
            {
                str[j] = input[i];
            }
            push(str, &head);
        }
        else if (strncmp(input, "POP", 3) == 0)
        {
            pop(&head);
        }
        else if (strncmp(input, "TOP", 3) == 0)
        {
            top(&head, new_fd);
        }
        //for the bonus:
        // if (strncmp(input, "ENQUEUE", 7) == 0)
        // {
        //     for (int i = 8, j = 0; i < strlen(input); i++, j++)
        //     {
        //         str[j] = input[i];
        //     }
        //     enqueue(str, &head);
        // }
        // else if (strncmp(input, "DEQUEUE", 7) == 0)
        // {
        //     dequeue(&head, new_fd);
        // }
        //until here
        bzero(input, 1024);
        if ((numbytes = recv(new_fd, input, MAXDATASIZE - 1, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }
        if (!numbytes)
        {
            printf("client disconnect\n");
            close(new_fd);
            return NULL;
        }
        
        input[numbytes] = '\0';
        printf("server received: ");
        for (int i = 0; i < strlen(input); i++)
        {
            printf("%c", input[i]);
        }
        printf("\n");
    }
    printf("good bye\n");
    // if (send(new_fd, "Hello, world!", 13, 0) == -1) {
    //     perror("send");
    // }
    close(new_fd);
    return NULL;
}

int main(void)
{
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    // pthread_mutex_init(&mutex,NULL);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    pthread_t tid[CLIENT_NUMBER];
    int i = 0;
    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (pthread_create(&tid[i++], NULL, &myThread, &new_fd) != 0)
        {
            printf("Failed to create thread\n");
        }
        if (i >= CLIENT_NUMBER)
        {
            // Update i
            i = 0;
            while (i < CLIENT_NUMBER)
            {
                pthread_join(tid[i++], NULL);
            }
            // Update i
            i = 0;
        }
    }

    return 0;
}