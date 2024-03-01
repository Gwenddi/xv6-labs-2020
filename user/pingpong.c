#include <kernel/types.h>
#include <user/user.h>

int main()
{
    int p1[2]; // p1[1]父进程写端，p1[0]子进程读端
    int p2[2]; // p2[1]子进程写端，p2[0]父进程读端
    pipe(p1);
    pipe(p2);

    char buf = 'p'; // 用于传输的字节

    int pid = fork();

    if (pid < 0)
    {
        fprintf(2, "fork() error!");
        // 不用的管道要及时关闭
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
        exit(1);
    }
    else if (pid == 0) // 子进程读入ping，写入pong
    {
        // 关闭不需要的管道端
        close(p1[1]);
        close(p2[0]);

        // 子进程从p1读端读取字符串
        if (read(p1[0], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "a-->b read error\n");
            exit(1);
        }
        // 打印读取到的字符数组
        printf("%d: received ping\n", getpid());

        // 子进程向p2的写端，写入字符数组
        if (write(p2[1], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "a-->b write error\n");
            exit(1);
        }
        close(p1[0]);
        close(p2[1]);

        exit(0);
    }
    else
    {
        // 关闭不需要的管道端
        close(p1[0]);
        close(p2[1]);

        // 父进程向p1的写端，写入字符数组
        if (write(p1[1], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "b-->a write error\n");
            exit(1);
        }
        // 父进程从p2读端读取字符串
        if (read(p2[0], &buf, sizeof(char)) != sizeof(char))
        {
            fprintf(2, "b-->a read error\n");
            exit(1);
        }

        // 打印读取到的字符数组
        printf("%d: received pong\n", getpid());
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    exit(0);
}