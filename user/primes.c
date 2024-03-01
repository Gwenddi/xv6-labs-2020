#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// 利用递归
// 从左管道中读取一个数，若读不到说明已经到了终点，exit；
// 否则再创建一个右边的管道并fork一个进程，并将筛选后的数写入右边的管道

void primes(int *fd)
{
    int p, d;
    close(fd[1]);
    // read函数原型：ssize_t read(int fd, void *buf, size_t count);
    if (read(fd[0], (void *)&p, sizeof(p)) != sizeof(p))
    {
        fprintf(2, "Read fail\n");
        exit(1);
    }

    printf("prime %d\n", p);

    // 当管道的写端关闭时，read返回0
    if (read(fd[0], (void *)&d, sizeof(d)))
    {
        int f[2];
        pipe(f);
        if (fork() == 0)
        {
            primes(f);
        }
        else
        {
            close(f[0]);
            do
            {
                if (d % p != 0)
                {
                    write(f[1], (void *)&d, sizeof(d));
                }
            } while (read(fd[0], (void *)&d, sizeof(d)));
            close(fd[0]);
            close(f[1]);
            wait(0);
        }
    }
    exit(0);
}

int main()
{
    int fd[2];
    int start = 2;
    int end = 35;
    pipe(fd);

    if (fork() == 0)
    {
        primes(fd);
    }
    else
    {
        close(fd[0]);
        // 对于主进程来说，把从2到35的数字写入到管道中，并等待所有的子进程完成
        for (int i = start; i <= end; i++)
        {
            if (write(fd[1], (void *)&i, sizeof(i)) != sizeof(i))
            {
                fprintf(2, "write faill\n");
                exit(1);
            }
        }
        close(fd[1]);
        wait(0);
    }
    exit(0);
}