
#include "Ex1.h"

void powerServer()
{
    char *ip = "127.0.0.1";
    int port = 5566;

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");
    int flag = 1;
    while (1)
    {
        if (flag)
        {
            addr_size = sizeof(client_addr);
            client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
            printf("[+]Client connected.\n");
            flag = 0;
        }
        bzero(buffer, 1024);
        if (!recv(client_sock, buffer, sizeof(buffer), 0))
        {
            close(client_sock);
            flag = 1;
        }
        else
        {
            for (int i = 0; i < 1024; i++)
            {
                printf("%c", buffer[i]);
            }
            printf("\n");
        }
    }
}
int main(int argc, char const *argv[])
{
    powerServer();
    return 0;
}
