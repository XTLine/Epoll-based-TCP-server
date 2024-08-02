#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "172.27.13.157" // 服务器 IP 地址
#define SERVER_PORT 9703          // 服务器端口
#define BUFFER_SIZE 1024          // 缓冲区大小

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    ssize_t read_size;

    // 创建 socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址结构
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    // 连接到服务器
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type your message (or 'quit' to exit):\n");

    while (1)
    {
        // 从用户输入获取消息
        printf("Your message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // 移除换行符
        buffer[strcspn(buffer, "\n")] = 0;

        // 检查是否退出
        if (strcmp(buffer, "quit") == 0)
        {
            break;
        }

        // 发送消息到服务器
        if (send(sock, buffer, strlen(buffer), 0) < 0)
        {
            perror("Send failed");
            break;
        }

        // 接收服务器的回声
        read_size = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (read_size > 0)
        {
            buffer[read_size] = '\0';
            printf("Server echo: %s\n", buffer);
        }
        else if (read_size == 0)
        {
            printf("Server disconnected\n");
            break;
        }
        else
        {
            perror("Receive failed");
            break;
        }
    }

    // 关闭 socket
    close(sock);
    return 0;
}