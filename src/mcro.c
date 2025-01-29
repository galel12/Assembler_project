#include "libraries.h"


int macroExpansion(FILE *oldfp, FILE *newfp)
{
    rewind(oldfp);   /*Place the pointer to the beginning of the file*/
    McroList MacroList = createMcroList(oldfp);
    rewind(oldfp); 
    char currLine[MaxLineLength];
    const char *declareMcro = "mcro";
    const char *endMcro = "endmcro";
    struct Node *curr = MacroList.head;
    bool foundMcro = false;
    bool keep_reading = true;
    char firstWord[MaxLineLength];
    int i;
    int spaces;
    

    while (fgets(currLine, MaxLineLength, oldfp)) /*Read from oldfp each line until EOF*/
    {
        foundMcro = false;

        /* Read the first word in the current line and write it in 'firstWord'
         * If its an empty line (currLine = " \n \t  \n") => read the next line 
         */
        if(sscanf(currLine, "%s", firstWord) == EOF)  
        {
            fputs(currLine,newfp);
            continue;
        }

        /*
         Check if the current line is a macro,
         if it is replace it with the appropriate data
        */
        while (curr->next != NULL && !foundMcro)
        {
            if (strcmp(curr->data->name, firstWord) == 0) /*We've reached a Macro*/
            {
                /*Update the foundMcro flag and write the data respectively in the newfp file*/
                foundMcro = true;
                for (i = 0; i < curr->data->numOfLines; i++)
                {
                    spaces = CountSpacesUntilFirstChar(currLine);
                    fprintf(newfp, "%*c", spaces, ' ');
                    fputs(curr->data->text[i], newfp);
                }
            }
            curr = curr->next;
        }
        curr = MacroList.head;

        /*Check if the current line is not a macro name*/

        if (foundMcro == false)
        {
            /*
             Check if the current line is a macro declaration
             that starts with "mcro ***" and ends with "endmcro"
            */
            if (strcmp(firstWord, declareMcro) == 0)
            {
                /*
                 As long as we haven't reached the EOF or "endmcro"
                 keep skipping the lines in the oldfp stream
                */
                while (keep_reading)
                {
                    fgets(currLine, MaxLineLength, oldfp);
                    sscanf(currLine, "%s", firstWord);
                    if ((strcmp(firstWord, endMcro) == 0) || feof(oldfp))
                        keep_reading = false;
                }
                keep_reading = true;
            }

            /*
            if it's not all the above
            then this line is not macro related and we will write it as is in the newfp file
            */
            else
            {
                fputs(currLine, newfp);
            }
        }
    }
    freeMcroList(&MacroList);
    return 1;
}


