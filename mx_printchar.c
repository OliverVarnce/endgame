#include "header.h"

void mx_printchar(char c) {
    char *p = &c;
    write(1, p, 1);
}
