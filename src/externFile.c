#include "libraries.h"

void createExtFile(FILE *EXT_FILE, ext_symbols *EXTERNS, int numOfExt)
{
    rewind(EXT_FILE);
    int i = 0;
    while (i < numOfExt)
    {
        fprintf(EXT_FILE, "%s\t%d\n", EXTERNS[i].Symbol_name, EXTERNS[i].decimal_address);
        i++;
    }
}