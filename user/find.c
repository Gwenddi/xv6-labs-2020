#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// 1. 如果当前路径为文件，则判断文件名是否和目标文件名相同
// 2. 若为目录，则遍历目录下的文件，并递归处理

/**
 * 该函数从路径中提取文件或目录的名称，并在静态缓冲区 buf 中返回一个格式化的名称。
  首先，它找到路径中最后一个斜杠之后的第一个字符。
  如果名称的长度超过了 DIRSIZ（在 xv6 中定义为 14），则直接返回名称。
  否则，将名称复制到缓冲区，并在名称末尾添加足够的空格以填充 DIRSIZ
*/
char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    // fstat系统调用从文件描述符所引用的inode中检索信息
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // 根据文件类型，执行不同的操作：
    // 如果是文件，直接打印文件信息，包括名称、类型、inode 号和大小。
    // 如果是目录，遍历目录中的每个文件项，获取每个文件的信息，并打印出来。
    //    如果文件项的 inum 为 0，则表示该项未被使用。
    //    使用 memmove 将文件名移动到缓冲区，并添加斜杠。
    //    对每个文件名执行 stat，获取文件信息，并打印到控制台。
    switch (st.type)
    {
    case T_FILE:
        if (strcmp(de.name, filename) == 0)
        {
            printf("%s/%s", path, filename);
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }

        // buf下一层路径
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            // 不要在“.”和“..”目录中递归
            if (de.inum == 0 || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..")) == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            // stat 函数用于获取一个文件或目录的状态信息，并存储在st中
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }

            if (st.type == T_FILE)
            {
                if (strcmp(de.name, filename) == 0)
                {
                    printf("%s\n", buf);
                }
            }
            else if (st.type == T_DIR)
            {
                find(buf, filename);
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Please enter a dir and a filename!\n");
        exit(1);
    }
    else
    {
        char *path = argv[1];
        char *filename = argv[2];
        find(path, filename);
        exit(0);
    }
}