#include "kernel/types.h"
#include "user/user.h"

// 写一个用户程序，调用sleep system call实现，执行sleep 10，表示程序等待10个时钟周期
int main(int argc, char *argv[])
{
    // 用户忘记传递参数，argv[0]一般表示程序名称，argv[1]表示传递的参数列表
    if(argc!=2)
    {
        fprintf(2,"must 1 argument for sleep/n");
        exit(1);
    }
    printf("(nothing happens for a little while)");
    int num = atoi(argv[1]);
    sleep(num);
    exit(0);
}