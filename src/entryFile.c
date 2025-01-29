#include "libraries.h"

void createEntFile(FILE *ENT_FILE, Symbols* Symbol_Table, int label_count)
{
    rewind(ENT_FILE);
    int i = 0;
    int symbol_type;
    while (i < label_count)
    {
        symbol_type = Symbol_Table[i].type;
        if((symbol_type & S_ENTRY) == S_ENTRY)
            fprintf(ENT_FILE, "%s\t%d\n", Symbol_Table[i].Symbol_name, Symbol_Table[i].decimal_address);

        i++;
    }

}