#include "libraries.h"

int FirstPass(FILE *file, Symbols **symbolTableOut, binary_code **dcTable, binary_code **icTable, int *ic, int *dc, int *labelCount)
{
    rewind(file);
    int IC = 0;
    int DC = 0;
    int L = 0;

    char currLine[MaxLineLength];
    char Label[Max_LabelLength];
    char Word[MaxLineLength];
    char Content[MaxLineLength];
    int len = 0;
    word_type WordType = 0;
    Sentence_Structure SentenceType = 0;
    bool Label_Flag = false;
    int label_counter = 0; 
    int numOfOperands = 0;
    Symbols *symbolTable = (Symbols *)malloc(sizeof(Symbols));
    CheckMemoryAllocation(symbolTable);
    int pSizeSymbolTable = 1, indexSymbolTable = 0;
    int currDecAddress = 100;
    binary_code* DC_TABLE = (binary_code *)malloc(sizeof(binary_code));
    CheckMemoryAllocation(DC_TABLE);
    int dcLength=0, indexDC = 0, pSizeDC = 1;
    binary_code* IC_TABLE = (binary_code *)malloc(sizeof(binary_code));
    CheckMemoryAllocation(IC_TABLE);
    int icLength=0, indexIC = 0, pSizeIC = 1;
    Line_counter  = 0;
    bool errorFlag = false;
    error = NO_ERROR;
    

    while (fgets(currLine, MaxLineLength, file)) // Read from file each line until EOF
    {
        Line_counter++;

        //checks if this line is over 81 characters (i.e. 81 characters which are not '\n').
        //if this line is longer than 81 characters print an error message and skip to the next line in the file
        if(!(lineLength_isValid(currLine)))
        {
            error = LINE_TOO_LONG;
            write_error(Line_counter,error);
            errorFlag = true;
            skip_line(file);
            error = NO_ERROR;
            continue;
        }
        
        SentenceType = Sentence_type(currLine, &(Label_Flag), Content);

        // if there's a label check that the rest of this sentence isn't empty (e.g. : "MAIN:              \n" - incorrect line!)
        // if it's empty then notify by error message and continue to read the next line

        if(Label_Flag)
        {
            char *lbl = getLabelNameFromLine(currLine);
            char *p = strstr(currLine,lbl);
            p = p + strlen(lbl) + 1;
            if(is_empty(p))
            {
                error = JUST_LABEL;
                write_error(Line_counter,error);
                errorFlag = true;
                error = NO_ERROR;
                Label_Flag = false; 
                Content[0] = '\0';
                continue;
            }
        }

        //if we find other error in the current line => notify by error message and continue to read next line
        if(is_error(error) && (SentenceType == -1 || error == SYNTAX_ERROR))
        {
            write_error(Line_counter,error);
            errorFlag = true;
            error = NO_ERROR;
            Label_Flag = false; 
            Content[0] = '\0';
            continue;
        }

        // if we reached here then there are no errors in this line (except maybe a label that defined twice in 2 different lines in the file)
        switch (SentenceType)
        {
        case GUIDING:
            WordType = check_Guiding_Sentence(currLine, &(Label_Flag), Content);
            binary_code *DC_COPY_TABLE;
            switch (WordType)
            {
            case DATA: // LOOP: .data 7,6
                if (Label_Flag)
                {
                    strcpy(Label,getLabelNameFromLine(currLine));
                    if(!newLabel(symbolTable, Label, label_counter) && !symbolIsExtern(symbolTable,Label,label_counter))
                    {
                        error = LABEL_ALREADY_EXISTS;
                        errorFlag = true;
                        write_error(Line_counter,error);
                        error = NO_ERROR;
                        Label_Flag = false; 
                        Content[0] = '\0';
                        continue;
                    }
                   
                    symbolTable = checkAndResizeArray(symbolTable, sizeof(Symbols), indexSymbolTable, &pSizeSymbolTable);
                    symbolTable[indexSymbolTable].Symbol_name = getLabelNameFromLine(currLine);
                    symbolTable[indexSymbolTable].decimal_address = currDecAddress + DC;
                    symbolTable[indexSymbolTable].type = S_DATA;
                    indexSymbolTable++;
                    label_counter++;
                }
                DC_COPY_TABLE = getBinaryCodeFromData(Content, true, &dcLength);
                TableCpy(&DC_TABLE, DC_COPY_TABLE, &indexDC, dcLength, &pSizeDC);
                DC += check_content_length(Content, WordType);
                indexDC += dcLength;
                break;

            case STRING:
                if (Label_Flag)
                {
                    strcpy(Label,getLabelNameFromLine(currLine));
                    if(!newLabel(symbolTable, Label, label_counter) && !symbolIsExtern(symbolTable,Label,label_counter))
                    {
                        error = LABEL_ALREADY_EXISTS;
                        errorFlag = true;
                        write_error(Line_counter,error);
                        error = NO_ERROR;
                        Label_Flag = false; 
                        Content[0] = '\0';
                        continue;
                    }
                   
                    symbolTable = checkAndResizeArray(symbolTable, sizeof(Symbols), indexSymbolTable, &pSizeSymbolTable);
                    symbolTable[indexSymbolTable].Symbol_name = getLabelNameFromLine(currLine);
                    symbolTable[indexSymbolTable].decimal_address = currDecAddress + DC;
                    symbolTable[indexSymbolTable].type = S_STRING;
                    indexSymbolTable++;
                    label_counter++;
                }
                DC_COPY_TABLE = getBinaryCodeFromData(Content, false, &dcLength);
                TableCpy(&DC_TABLE, DC_COPY_TABLE, &indexDC, dcLength, &pSizeDC);
                DC += check_content_length(Content, WordType); //all characters + NULL char
                indexDC += dcLength;
                break;

            case ENTRY: //.entry LOOP
                break;

            case EXTERN: //.extern LOOP
                         
                symbolTable = checkAndResizeArray(symbolTable, sizeof(Symbols), indexSymbolTable, &pSizeSymbolTable);
                len = strlen(Content);
                symbolTable[indexSymbolTable].Symbol_name = (char *)malloc((len + 1) * sizeof(char));
                CheckMemoryAllocation(symbolTable[indexSymbolTable].Symbol_name);
                strcpy(symbolTable[indexSymbolTable].Symbol_name, Content);
                symbolTable[indexSymbolTable].Symbol_name[len] = '\0';
                symbolTable[indexSymbolTable].decimal_address = 1;
                symbolTable[indexSymbolTable].type = S_EXTERN;
                indexSymbolTable++;
                label_counter++;
                break;

            default:
                break;
            }
            break;

        case COMMAND:
            WordType = check_Instruction_Sentence(currLine, &(Label_Flag), Content);
            binary_code *IC_COPY_TABLE;
            if (Label_Flag)
            {
                strcpy(Label,getLabelNameFromLine(currLine));
                if(!newLabel(symbolTable, Label, label_counter) && !symbolIsExtern(symbolTable,Label,label_counter))
                {
                    error = LABEL_ALREADY_EXISTS;
                    errorFlag = true;
                    write_error(Line_counter,error);
                    error = NO_ERROR;
                    Label_Flag = false; 
                    Content[0] = '\0';
                    continue;
                }
                symbolTable = checkAndResizeArray(symbolTable, sizeof(Symbols), indexSymbolTable, &pSizeSymbolTable);
                symbolTable[indexSymbolTable].Symbol_name = getLabelNameFromLine(currLine);
                symbolTable[indexSymbolTable].decimal_address = currDecAddress + IC;
                symbolTable[indexSymbolTable].type = S_INSTRUCTION;
                indexSymbolTable++;
                label_counter++;

                sscanf(currLine, "%*s %s", Word); // Second word is the command
                char *command = Find_Which_Instruction_name(Word);
                check_instruction_Content(currLine, command, Content, &numOfOperands);
                IC_COPY_TABLE = getBinaryCodeFromInstructions(Content, command, numOfOperands, &L);
                icLength += L;
                TableCpy(&IC_TABLE, IC_COPY_TABLE, &indexIC, L, &pSizeIC);
                IC = L + IC;
                indexIC = icLength;
                
            }
            else
            {
                sscanf(currLine, "%s", Word); // First word is the command
                char *command = Find_Which_Instruction_name(Word);
                check_instruction_Content(currLine, command, Content, &numOfOperands);
                IC_COPY_TABLE = getBinaryCodeFromInstructions(Content, command, numOfOperands, &L);
                icLength += L;
                TableCpy(&IC_TABLE, IC_COPY_TABLE, &indexIC, L, &pSizeIC);
                IC = L + IC;
                indexIC = icLength;
            }

        default: //if it's empty or comment line then we do nothing and move to the next line 
            break;
        }

        Label_Flag = false; 
        error = NO_ERROR;
        Content[0] = '\0'; 
    }

    if(errorFlag)
    {
        return 0;
    }

    update_DataSymbolsAddress(symbolTable, IC);
    *dcTable = DC_TABLE;
    *dc = DC;
    *icTable = IC_TABLE;
    *ic = IC;
    *symbolTableOut = symbolTable;
    *labelCount = label_counter;
    return 1;
}


