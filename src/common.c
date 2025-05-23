#include "common.h"

void error(const char* msg, const char* msg1)
{
    fprintf(stderr, "ERROR: %s%s\n", msg, msg1 ? msg1 : "");
    exit(1);
}

void warning(const char* msg)
{
    fprintf(stderr, "WARNING: %s\n", msg);
}
