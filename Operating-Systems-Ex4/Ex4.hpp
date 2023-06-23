#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <tbb/mutex.h>

#include <pthread.h>
#include <string>
#include <iostream>
using namespace tbb;

#define PORT "3490" // the pסrt users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

#define CLIENT_NUMBER 3 // how many clients can connect server parllel

#define MAXDATASIZE 1024 // max number of bytes we can get at once
// ********************* malloc, free and calloc*********************************
typedef struct free_block
{
    size_t size;
    struct free_block *next;
} free_block;
static free_block free_block_list_head = {0, 0};

// static const size_t overhead = sizeof(size_t);

static const size_t align_to = 16;

void *mymalloc(size_t size)
{
    size = (size + sizeof(free_block) + (align_to - 1)) & ~(align_to - 1);
    free_block *block = free_block_list_head.next;
    free_block **head = &(free_block_list_head.next);
    while (block != 0)
    {
        if (block->size >= size)
        {
            *head = block->next;
            return ((char *)block) + sizeof(free_block);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (free_block *)sbrk(size);
    block->size = size;

    return ((char *)block) + sizeof(free_block);
}

void myfree(void *ptr)
{
    free_block *block = (free_block *)(((char *)ptr) - sizeof(free_block));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}
void *calloc(size_t nelem, size_t elsize)
{
    void *p;

    p = mymalloc(nelem * elsize);
    if (p == 0)
        return (p);

    bzero(p, nelem * elsize);
    return (p);
}
// ********************** tbb lock ********************************

int flagp = 1;
// pthread_mutex_t mutex;
mutex myMutex1;
mutex myMutex2;
mutex myMutex3;
//********************* stack ***************************
typedef struct StackNode
{
    char data[MAXDATASIZE];
    struct StackNode *next;
} StackNode, *pStackNode;

pStackNode myNewNode(char *data)
{
    pStackNode newN = (pStackNode)(mymalloc(sizeof(StackNode)));
    bzero(newN->data, MAXDATASIZE);
    strcpy(newN->data, data);
    newN->next = NULL;
    return newN;
}

void push(char *data, pStackNode *root)
{
    // pthread_mutex_lock(&mutex);
    myMutex1.lock();
    if (flagp)
    {
        flagp = 0;
        sleep(5);
    }
    pStackNode newN = myNewNode(data);
    newN->next = *root;
    *root = newN;
    printf("\'");
    for (int i = 0; i < strlen(data); i++)
    {
        printf("%c", data[i]);
    }
    printf("\'");
    printf(" push to the stack\n");
    // pthread_mutex_unlock(&mutex);
    myMutex1.unlock();
}
void pop(pStackNode *root)
{
    myMutex2.lock();
    pStackNode temp = *root;
    *root = (*root)->next;
    printf("\'");
    for (int i = 0; i < strlen(temp->data); i++)
    {
        printf("%c", temp->data[i]);
    }
    printf("\' pop from the stack\n");
    myfree(temp);
    myMutex2.unlock();
}
void top(pStackNode *root, int sockfd)
{
    myMutex3.lock();
    char input[1024] = {0};
    strcat(input, "OUTPUT: ");
    for (int i = 0, j = 8; i < strlen((*root)->data); j++, i++)
    {
        input[j] = (*root)->data[i];
    }
    if (send(sockfd, input, strlen(input), 0) == -1)
    {
        perror("send");
    }
    myMutex3.unlock();
}
//****************Bonus Queue*******************************
//for the bonus:
// void enqueue(char *data, pStackNode *root)
// {
//     // pthread_mutex_lock(&mutex);
//     myMutex1.lock();
//     if (flagp)
//     {
//         flagp = 0;
//         sleep(5);
//     }
//     pStackNode newN = myNewNode(data);
//     if (!*root)
//     {
//         *root = newN;
//         newN->next = NULL;
//         printf("\'");
//         for (int i = 0; i < strlen(data); i++)
//         {
//             printf("%c", data[i]);
//         }
//         printf("\'");
//         printf(" enqueue to the stack\n");
//         return;
//     }
//     pStackNode temp = *root;
//     while (temp->next)
//     {
//         temp = temp->next;
//     }
//     temp->next = newN;
//     newN->next = NULL;
//     printf("\'");
//     for (int i = 0; i < strlen(data); i++)
//     {
//         printf("%c", data[i]);
//     }
//     printf("\'");
//     printf(" enqueue to the stack\n");
//     // pthread_mutex_unlock(&mutex);
//     myMutex1.unlock();
// }
// void dequeue(pStackNode *root, int sockfd)
// {
//     myMutex2.lock();
//     pStackNode temp = *root;
//     *root = (*root)->next;
//     printf("\'");
//     for (int i = 0; i < strlen(temp->data); i++)
//     {
//         printf("%c", temp->data[i]);
//     }
//     printf("\' dequeuq from the stack\n");
//     char input[1024] = {0};
//     strcat(input, "OUTPUT: ");
//     for (int i = 0, j = 8; i < strlen(temp->data); j++, i++)
//     {
//         input[j] = temp->data[i];
//     }
//     if (send(sockfd, input, strlen(input), 0) == -1)
//     {
//         perror("send");
//     }
//     myfree(temp);
//     myMutex2.unlock();
// }
//until here
