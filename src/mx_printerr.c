#include "header.h"

int mx_strlen();

void mx_printerr(const char *s) {
    write(2, s, mx_strlen(s));
}

