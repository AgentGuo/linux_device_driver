#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

char buf[128];

int main()
{
    int fd, m, n, length = 0;
    fd = open("/dev/charDevice", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "open file \"/dev/hello\" failed\n");
        exit(-1);
    }
    llseek(fd, 0, 0);

    n = write(fd, "I wrote a sentence!", 19);
    printf("write length: %d\n", n);
    n = write(fd, "I wrote another sentence", 24);
    printf("write length: %d\n", n);

    printf("read: ");
    while ((m = read(fd, buf + length, 1)) > 0 && buf[length] != '\0') {
        length++;
    }
    for(int i = length - 1; i >= 0; i--){
        printf("%c", buf[i]);
    }
    printf("\n");


    close(fd);
    return 0;
}