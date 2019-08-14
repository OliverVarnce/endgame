#include "header.h"

char* reverse(char *buffer, int i, int j) {
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);

    return buffer;
}
