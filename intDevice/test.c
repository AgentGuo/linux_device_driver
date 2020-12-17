#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int inputBuf[128], outputBuf[128];

int main()
{
    int fd, m, n;
    fd = open("/dev/intDevice", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open file \"/dev/intDevice\" failed\n");
        exit(-1);
    }

    llseek(fd, 0, 0);
    inputBuf[0] = 666;
    inputBuf[1] = 333;
    n = write(fd, inputBuf, 2 * sizeof(int));

    printf("read: ");
    m = read(fd, outputBuf, 2 * sizeof(int));
    printf("sum = %d\n", outputBuf[0] + outputBuf[1]);

    close(fd);
    return 0;
}