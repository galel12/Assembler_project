#include "libraries.h"

int SecondPass(FILE *file, Symbols *symbolTable, binary_code* IC_Table, ext_symbols **EXTERNS, int* numOfExt, int label_count, bool *entry_flag)
{
    rewind(file);
    int IC = 0;
    int L = 0;

    char currLine[MaxLineLength];
    char Label[Max_LabelLength];
    char Word[MaxLineLength];
    char Content[MaxLineLength];
    int len = 0;
    word_type WordType = 0;
    Sentence_Structure SentenceType = 0;
    bool Label_Flag = false;
    bool errorFlag = false;
    int numOfOperands = 0;
    Line_counter  = 0;
    ext_symbols *externals = (ext_symbols *)malloc(2*(sizeof(ext_symbols)));
    CheckMemoryAllocation(externals);
    int pSizeExtTable = 2, indexExtTable = 0;
   

    while (fgets(currLine, MaxLineLength, file)) // Read from file each line until EOF
    {
        Line_counter++;

        SentenceType = Sentence_type(currLine, &(Label_Flag), Content);
        switch (SentenceType)
        {
        case GUIDING:
            WordType = check_Guiding_Sentence(currLine, &(Label_Flag), Content);
            
            switch (WordType)
            {
            case DATA: // LOOP: .data 7,6
                Content[0] = '\0';
                error = NO_ERROR;
                Label_Flag = false; 
                continue;
                break;

            case STRING:
                Content[0] = '\0';
                error = NO_ERROR;
                Label_Flag = false; 
                continue;
                break;
                
            // ! Handle the case that the same .entry label is defined twice in the file (in 2 different lines)
            case ENTRY: //.entry LOOP
                if (symbolIsExtern(symbolTable, Content,label_count))
                {
                    errorFlag = true;
                    error = ENTRY_ALREADY_EXTERN;
                    write_error(Line_counter,error);
                    Content[0] = '\0';
                    error = NO_ERROR;
                    Label_Flag = false; 
                    continue;
                }
                else if(newLabel(symbolTable,Content,label_count))
                {
                    error = ENTRY_LABEL_DOES_NOT_EXIST;
                    errorFlag = true;
                    write_error(Line_counter,error);
                    Content[0] = '\0';
                    error = NO_ERROR;
                    Label_Flag = false; 
                    continue;
                }
                else
                {
                    *entry_flag = true;
                    Handle_Entry_LabelEncoding(symbolTable, Content,label_count);
                }
                break;

            case EXTERN: //.extern LOOP
                Content[0] = '\0';
                error = NO_ERROR;
                Label_Flag = false; 
                continue;
                break;

            default:
                break;
            }
            break;

        case COMMAND:
            WordType = check_Instruction_Sentence(currLine, &(Label_Flag), Content);
            if (Label_Flag)
            {
                sscanf(currLine, "%*s %s", Word); // Second word is the command
            }
            else
            {
                sscanf(currLine, "%s", Word); // first word is the command
            }
            
            char *command = Find_Which_Instruction_name(Word);
            check_instruction_Content(currLine, command, Content, &numOfOperands);
            (void *)getBinaryCodeFromInstructions(Content, command, numOfOperands, &L);
            IC = L + IC;

            Handle_instruction_LabelEncoding(symbolTable,IC_Table,Content, numOfOperands, (IC - 1), externals, &indexExtTable, label_count);
            
            if(indexExtTable + 2 > pSizeExtTable)
            {
                externals = (ext_symbols *)realloc(externals,2*pSizeExtTable*(sizeof(ext_symbols)));
                CheckMemoryAllocation(externals);
                pSizeExtTable *= 2; 
            }
            if(is_error(error))
            {
                write_error(Line_counter,error);
                errorFlag = true;
            }
            break;

        default:
            break;
        }
        error = NO_ERROR;
        Label_Flag = false; 
        Content[0] = '\0'; 
    }

    if(errorFlag)
    {
        return 0;
    }


    *EXTERNS = externals;
    *numOfExt = indexExtTable;

    return 1;
    
}
