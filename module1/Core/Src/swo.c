#include <stdio.h>
#include "stm32f4xx.h"

int _write(int fd, char* ptr, int len)
{
    (void)fd;
    int i = 0;
    while (ptr[i] && (i < len)) {
        ITM_SendChar((int)ptr[i]);
        if (ptr[i] == '\n') {
            ITM_SendChar((int)'\r');
        }
        i++;
    }
    return len;
}
