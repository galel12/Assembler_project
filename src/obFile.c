#include "libraries.h"

void createObjFile(FILE *OBJECT, binary_code* IC_Table, binary_code* DC_Table, int ic, int dc)
{
    rewind(OBJECT);
    fprintf(OBJECT, "%d %d\n", ic, dc);
    int i = 0;
    char firstCh64, secondCh64;

    TBYTE encoding_FirstLetter=0, encoding_SecondLetter=0;
  

    while(i < ic)
    {
        encoding_FirstLetter = IC_Table[i].machineCode;
        encoding_SecondLetter = IC_Table[i].machineCode;
        
        encoding_FirstLetter = changeBitsValue(encoding_FirstLetter,0,0,6);
        firstCh64 = base64_char(encoding_FirstLetter >> 6);
        
        encoding_SecondLetter = changeBitsValue(encoding_SecondLetter, 0,6,6);
        secondCh64 =base64_char(encoding_SecondLetter);

        fprintf(OBJECT,"%c%c\n",firstCh64,secondCh64);
        i++;
    }

    i = 0;

    while(i < dc)
    {
      encoding_FirstLetter = DC_Table[i].machineCode;
      encoding_SecondLetter = DC_Table[i].machineCode;
      
      encoding_FirstLetter = changeBitsValue(encoding_FirstLetter,0,0,6);
      
      firstCh64 = base64_char(encoding_FirstLetter >> 6);
      
      encoding_SecondLetter = changeBitsValue(encoding_SecondLetter, 0,6,6);
      
      secondCh64 =base64_char(encoding_SecondLetter);

      fprintf(OBJECT,"%c%c\n",firstCh64,secondCh64);
      i++;
    }

}

char base64_char (TBYTE n)
{
  if (n < 26) return n + 'A';
  else if (n < 52) return n + 'G';
  else if (n < 62) return n - 4;
  else if(n == 62) return '+';
  else if(n == 63) return '/';
  return 0;
}