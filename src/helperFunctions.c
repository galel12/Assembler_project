#include "libraries.h"

void CheckFiles(FILE *ptr)
{
    if (ptr == NULL)
    {
        printf("Allocation failed, exit program");
        exit(1);
    }
}

void CheckMemoryAllocation(void *ptr)
{
    if (ptr == NULL)
    {
        printf("Allocation failed, exit program");
        exit(1);
    }
}

/*remove whitespaces from begging and end of string*/
void trim(char *s)
{
    char *p = s;
    int l = strlen(p);

    while (isspace(p[l - 1]))
        p[--l] = 0;
    while (*p && isspace(*p))
        ++p, --l;

    memmove(s, p, l + 1);
}
/**
 * remove all whitespaces (\n\t and a space)
 * i - index of string
 * k - jumping index to remove the blank spaces 
*/
void deleteblankspaces(char *s, int i, int k)
{
    // if s[i] is not NULL it will enter the 'if' statement 
    if(s[i] != '\0')
    {
    	s[i]=s[i+k];
        
     	if(s[i]==' '|| s[i]=='\t' || s[i] == '\n')
     	{
		  k++;
		  i--;
	    }
	    i++;
     	deleteblankspaces(s,i,k);
	}
}

/*Count spaces until the first word in the string*/
int CountSpacesUntilFirstChar(char *p)
{
    int i = 0;
    int count = 0;

    while (isspace(p[i]))
    {
        count++;
        i++;
    }
    return count;
}

/*
 logSize = how many taken spots there's in the array
 *pSize = how much memory allocated for the array
 */
void* checkAndResizeArray(void* arr,int sizeOfElement,int logSize,int*pSize)
{
    void* newArr;
    if(logSize==*pSize)
    {
        (*pSize) *= 2;
        newArr=malloc((*pSize)*sizeOfElement);
        CheckMemoryAllocation(newArr);

        memcpy(newArr,arr,logSize*sizeOfElement);
        free(arr);
        return newArr;
    }

    return arr;
    
} 

// **  FUNCTIONS - Pre-assembler (mcro.c)

McroList createMcroList(FILE *file)
{
    McroList lst;
    Node *curr = NULL;

    CheckFiles(file);
    curr = (Node *)malloc(sizeof(Node));
    CheckMemoryAllocation(curr);
    curr->data = readMcroFromFile(file);
    lst.head = lst.tail = curr;

    while (!feof(file))
    {
        curr = (Node *)malloc(sizeof(Node));
        CheckMemoryAllocation(curr);
        curr->data = readMcroFromFile(file);
        lst.tail->next = curr;
        lst.tail = curr;
        curr->next = NULL;
    }

    return lst;
}

mcro *readMcroFromFile(FILE *file)
{
    const char *mcroCmp = "mcro";
    char str[MaxLineLength];
    bool found = false;
    mcro *res = (mcro *)malloc(sizeof(mcro));
    CheckMemoryAllocation(res);

    while (!feof(file) && !found)
    {
        fscanf(file, "%s", str);
        if (strcmp(str, mcroCmp) == 0)
        {
            found = true;
            fgets(str, MaxLineLength, file);
            res->name = (char *)malloc((strlen(str)) * sizeof(char));
            CheckMemoryAllocation(res->name);
            sscanf(str, "%s", res->name);
            res->name[strlen(str) - 1] = '\0';
            res->text = readMcroDataFromFile(file, &(res->numOfLines));
        }
        else
        {
            fgets(str, MaxLineLength, file);
        }
    }
    return res;
}

char **readMcroDataFromFile(FILE *file, int *length)
{
    char **lines = (char **)malloc(sizeof(char *));
    CheckMemoryAllocation(lines);
    int pSize = 1;
    int index = 0;
    const char *cmpStr = "endmcro";
    const char *newLine = "\n";
    char line[MaxLineLength], compareLine[MaxLineLength];

    do
    {
        if (index == pSize)
        {
            pSize *= 2;
            lines = (char **)realloc(lines, pSize * sizeof(char *));
            CheckMemoryAllocation(lines);
        }

        fgets(line, MaxLineLength, file);
        lines[index] = (char *)malloc((strlen(line) + 1) * sizeof(char));
        CheckMemoryAllocation(lines[index]);
        strcpy(lines[index], line);
        sscanf(lines[index], "%s", compareLine);
        trim(lines[index]);         //delete whitespaces from beginning and end of the string 
        strcat(lines[index], newLine);
        index++;

    } while (strcmp(cmpStr, compareLine) != 0);



    free(lines[index - 1]);
    lines = (char **)realloc(lines, (index - 1) * sizeof(char *));
    CheckMemoryAllocation(lines);
    *length = index - 1;
    return lines;
}

void freeMcroList(McroList* lst)
{
    Node* curr = lst->head;

    while(curr != NULL)
    {
        int i;
        Node* prev = curr;
        mcro* m = curr->data;
        for(i = 0; i < m->numOfLines; i++)
            free(m->text[i]);
        
        free(m->text);
        free(m->name);
        free(m);

        curr=curr->next;
        free(prev);
    }
}


// **  FUNCTIONS - First-pass

/**
 *  ! These functions checks Word Type - 
 *  ! label / instruction / ".data" / ".string" / "entry" / "extern"
 */


/** Checks if the word is a valid Label
 * !if not: Returns 0
 * if it is a label: Returns 1 (LABEL enum count)
 */
int is_Label(char *word)
{
    word_type WordType = 0;
    int i=0;
    int length = (int)strlen(word);

    if(word == NULL || strlen(word) == 0)
    {
        return 0;
    }
    if(not_in_saved_words(word))
    {    
        if (length > 0 && isalpha((int)*word)) // checks if the first letter is A-Z / a-z
        {
            for (i = 1; i < Max_LabelLength; i++)
            {
                if (isalnum(*(word+i))) // checks if word[i] = A-Z /a-z/ 0-9
                {
                    continue;
                }
                else if (*(word+i) == ':')
                {
                    WordType = LABEL;
                    return WordType;
                }
                else
                {
                    error = LABEL_NOT_ALPHANUMERIC;
                    return 0;
                }
            }

            if (*(word+Max_LabelLength) == ':')
            {
                WordType = LABEL;
                return WordType;
            }
            else
            {
                error = LABEL_TOO_LONG;
            }
        }
        else if(!(isalpha(*word)))
        {
            error = LABEL_Illegal_FIRST_CHAR;
        }
        
    }
    else
    {
        error = LABEL_CANT_BE_SAVED_WORD;
        return 0;
    }

    return WordType;
    
}

int lineLength_isValid(char* sentence)
{
    int check = strcspn(sentence,"\n");
    if(check == 80)
    {
        return (sentence[MaxLineLength] == '\n');
    }
    if(check < MaxLineLength - 1)
        return 1;
    
    return 0;
}

/**
 * checks if the word is a saved word that can't be used:
 * if it's not it returns 1 (i.e. we can use it)
 * otherwise it returns 0 (i.e. we can't use it)
*/
int not_in_saved_words(char* word)
{
    int i = 0;
    bool found = false;
    char* cmp_word;

    while (i < 28 && !found)
    {
        cmp_word = saved_words[i].words;
        if(strcmp(word, cmp_word) == 0)
        {
            found = true;
        }
        i++;
    }
    if(found)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/** Checks if the word is a valid Instruction
 * !if not: Returns 0
 * if it is an instruction: Returns 2 (INSTRUCTION enum count)
 */
int is_Instruction(char *word)
{
    word_type WordType;
    char *InstructionName;
    int i = 0;
    bool found = false;
    while (i < 16 && !found)
    {
        InstructionName = instruction_opCode[i].name;
        if (strcmp(InstructionName, word) == 0)
        {
            found = true;
            WordType = INSTRUCTION; // 1
        }
        i++;
    }

    if (found)
        return WordType; // Returns 1

    else
        return 0;
}

/**
 * checks which instruction the given "word" is
 * if it exists in instruction_opCode table then it returns the Instruction Name string
 * otherwise returns NULL
*/
char* Find_Which_Instruction_name(char* word)
{
    char* InstructionName;
    int i = 0;
    bool found = false;
    while (i < 16 && !found)
    {
        InstructionName = instruction_opCode[i].name;
        if (strcmp(InstructionName, word) == 0)
        {
            found = true;
        }
        i++;
    }

    if(found)
    {
        return InstructionName;
    }
    else
    {
        return NULL;
    }
}

/**
 * if the given word is instruction then:
 * it returns the instruction opcode (0 - 15)
 * if it's not instruction returns -1
*/
int Find_Instruction_OPcode(char* word)
{
    char* InstructionName;
    int i = 0;
    bool found = false;
    while (i < 16 && !found)
    {
        InstructionName = instruction_opCode[i].name;
        if (strcmp(InstructionName, word) == 0)
        {
            found = true;
            return i;
        }
        i++;
    }

    return -1;
}


/** Checks if the word is a valid .data OR .string
 * !if not: Returns 0
 * if it is a .data: Returns 3 (DATA enum count)
 * if it is a .string: Returns 4 (STRING enum count)
 */
int is_Data_OR_String(char *word)
{
    word_type WordType;
    char data[] = ".data";
    char string[] = ".string";
    if (strcmp(word, data) == 0)
    {
        WordType = DATA;
        return WordType; // returns 3
    }
    else if (strcmp(word, string) == 0)
    {
        WordType = STRING;
        return WordType; // returns 4
    }

    return 0;
}


/** Checks if the word is a valid .entry OR .extern
 * !if not: Returns 0
 * if it is .entry: Returns 5 (ENTRY enum count)
 * if it is .extern: Returns 6 (EXTERN enum count)
 */
int is_entry_OR_extern(char *word)
{
    char cmpEntry[] = ".entry";
    char cmpExtern[] = ".extern";
    word_type WordType;
    if (strcmp(word, cmpEntry) == 0)
    {
        WordType = ENTRY;
        return WordType; //returns 5
    }
    else if (strcmp(word, cmpExtern) == 0)
    {
        WordType = EXTERN;
        return WordType; //returns 6
    }

    return 0;
}

/**
 * checks if the word is a register (r0 - r7):
 * if it is it returns the register string
 * it it's not it returns NULL
*/
char* is_register(char* word)
{
    if(word == NULL)
    {
        return NULL;
    }
    char *reg;
    int i = 0;
    bool found = false;
    while (i < 8 && !found)
    {
        reg = registers[i].register_name;
        if (strcmp(reg, word) == 0)
        {
            found = true;
        }
        i++;
    }

    if(found)
    {
        return reg;
    }
    else
    {
        return NULL;
    }
}


/**
 * checks if the .data sentence content is valid:
 * if it's valid it returns 1
 * if it's not valid it returns 0
*/
int check_DataContent(char* Line, char* data)
{   
    char wordToFind[]=".data";
    char* subString = strstr(Line,wordToFind);
    char* res = subString + strlen(wordToFind);
    int res_Len = strlen(res);
    int i;
    char k = '\0';
    int ans; 
    int check_error=0;

    ans = sscanf(res, "%d %c", &i, &k); //scan number and a comma ','
    if(ans == 0)
    {
        error = DATA_EXPECTED_NUM;
        return 0;
    }

    char* check = (char*)malloc((res_Len + 1)*sizeof(char));
    strcpy(check, res);
    char* temp = check;
    
    /*ans == 2 => both number and character scanned successfully*/
    while(ans == 2 && k == ',')
    {
        // check = strchr(check, ',');
        temp = strchr(temp, ',');
        temp = temp + 1;
        ans = sscanf(temp, "%d %c", &i, &k);
    }

    if(ans == 0)
    {
        check_error = sscanf(temp, "%c", &k);
        if(check_error == 1 && k == ',')
        {
            error = DATA_GOT_COMMAS_IN_A_ROW;
        }
        else if(check_error == 1 && k != ',')
        {
            error = DATA_EXPECTED_NUM;
        }
    }

    free(check);
    /*
     * ans == 1 => just the number scanned successfully
     * k == '\0' => that's the case when theres only one number (.data 5)
     * k == ',' => that's the case when theres more than one number (.data 5,3)
                  the comma is saved in k char from the last loop
    */
    if (ans == 1 && (k == ',' || k == '\0'))
    {
        
        strcpy(data, res);
        deleteblankspaces(data,0,0); //remove all whitespaces (\n\t and a space)
        return 1;
    }

    if(ans == -1)
    {
        if(k == ',')
        {
            error = DATA_COMMA_AFTER_LAST_OP;
        }
        else
        {
            error = DATA_EXPECTED_NUM;
        }
    }


    return 0;
    
}

/**
 * checks if the .string sentence content is valid:
 * if it's valid it returns 1
 * if it's not valid it returns 0
*/
int check_StringContent(char* Line,char* string)
{
    bool strLine_is_good = false;
    char wordToFind[]=".string";
    char* subString = strstr(Line,wordToFind);
    char* res = subString + strlen(wordToFind);
    int res_Len = strlen(res);
    int ans;
    int i = 0;
    char k = '\0';
    bool utf8encoded_File = false;
    char* check = (char*)malloc((res_Len + 1)*sizeof(char));
    CheckMemoryAllocation(check);
    char utf8_QuotationMark[MaxLineLength];
    sscanf(res, " %[\xe2\x80\x9c]", utf8_QuotationMark);
    ans = sscanf(res, " %c ", &k);
    char* utf8QuotationCode = "\xe2\x80\x9c";
    utf8encoded_File = ((strcmp(utf8_QuotationMark,utf8QuotationCode) == 0) && (k == '\xe2'));
    strcpy(check, res);
    char* temp = check;

    if(!utf8encoded_File)
    {
        if(k != '\"')
        {
            error = STRING_OPERAND_NOT_VALID;
            free(check);
            return 0;
        }
    
        temp = strchr(temp, '\"');
        int still_string = strcspn(temp + 1, "\"");
        int len  = still_string + 1;
        ans = sscanf(temp, "%c", &k);
        temp = temp + 1;  
        k = '\0';

        while(i <= len  && ans == 1 && k != '\"')
        {
            ans = sscanf(temp + i, "%c", &k);
            i++;
        }
    
        if (k == '\"' && i == len)
        {
            ans = sscanf(temp + i, " %c", &k);
            if(ans == 1) //there are more characters after the second \"
            {
                error = STRING_TOO_MANY_OPERANDS;
                free(check);
                return 0;
            }

            strcpy(string, check);
            free(check);
            trim(string);
            memmove(string, string+1, strlen(string+1)+1);
            string[strlen(string)-1] = '\0';
            return 1;
        }

        error = STRING_OPERAND_NOT_VALID;
        free(check);
        return 0;
    }
    
    else
    {
        temp = strchr(temp, '\x9c');
        int still_string = strcspn(temp + 1, "\xe2");
        int len  = still_string + 1;
        ans = sscanf(temp, "%c", &k);
        temp = temp + 1;  
        k = '\0';

        while(i <= len  && ans == 1 && k != '\xe2')
        {
            ans = sscanf(temp + i, "%c", &k);
            i++;
        }
    
        if (k == '\xe2' && i == len)
        {
            strLine_is_good = true;
            strcpy(string, check);
            trim(string);
            memmove(string, string+3, strlen(string+1)+1);
            string[strlen(string)-3] = '\0';

        }

        free(check);
        return strLine_is_good;
    }
    
    return 0;
} 

/**
 * Checks if entry or extern sentence content is valid
 * if it's valid returns 1
 * if it's not returns 0
*/
int check_entryORexternContent(char* Line, char* second_word, word_type entryORextern)
{
    if((int)strlen(second_word) == 0)
    {
        switch (entryORextern)
        {
            case ENTRY:
                error = ENTRY_NO_LABEL;
                break;
            
            case EXTERN:
                error = EXTERN_GOT_NO_LABEL;
                break;
            
            default:
                break;
        }
        return 0;
    }
    char res[MaxLineLength] = "";
    sscanf(Line,"%*s%s",res);
    char *subString = strstr(res,second_word);
    char str[MaxLineLength];

    int check_label = is_LabelWithoutColon(second_word);
    
    if(check_label)
    {
        subString = subString + strlen(second_word) + 1;
        if(subString != NULL)
        {
            if(sscanf(subString, "%80s", str) == EOF)
            {
                str[0] = '\0';
                return 1;
            }
        
            else
            {
                switch (entryORextern)
                {
                case ENTRY:
                    error = ENTRY_TOO_MANY_OPERANDS;
                    break;
                
                case EXTERN:
                    error = EXTERN_TOO_MANY_OPERANDS;
                    break;
                
                default:
                    break;
                }
                
            }
        }
    }
    str[0]='\0';
    return 0;
}


int is_LabelWithoutColon(char* word)
{
    if(word != NULL)
    {
        char LabelWithoutColon[Max_LabelLength];
        strcpy(LabelWithoutColon, word);
        strcat(LabelWithoutColon, ":");
        int ans;
        ans = is_Label(LabelWithoutColon);
        return ans;
    }
    return 0;
}


/** check if that's a guiding sentence and checks if there's a label
* !if not: Returns 0
* !if it's a data\string\entry\extern sentence with invalid content: Returns -1 
* if it is a .data: Returns 3 (DATA enum count)
* if it is a .string: Returns 4 (STRING enum count)
* if it is a .entry: Returns 5 (ENTRY enum count)
* if it is a .extern: Returns 6 (EXTERN enum count)
*/
int check_Guiding_Sentence(char* sentence, bool* LabelFlag, char* Content)
{
    word_type WordType=0;
    char FirstWord[MaxLineLength];
    char SecondWord[MaxLineLength];
    char ThirdWord[MaxLineLength];
    FirstWord[0] = '\0';
    SecondWord[0] = '\0';
    ThirdWord[0] = '\0';
    int ans=0;

    sscanf(sentence, "%s %s %s", FirstWord, SecondWord, ThirdWord);
    
    if(!(is_entry_OR_extern(FirstWord)) && !(is_Data_OR_String(FirstWord)) && !(is_Instruction(FirstWord)))
    {
        WordType = is_Label(FirstWord);
    }

    if(is_error(error))
    {
        return -1;
    }

    if(WordType == LABEL)
    {
            /* Ignore labels that are defined before ".entry" / ".extern" declarations
               example: "MAIN: .entry LOOP" => ignore MAIN: label */
            if(!(is_entry_OR_extern(SecondWord))) 
                *LabelFlag = true;

            else
            {
                WordType = is_entry_OR_extern(SecondWord);
                switch(WordType)
                {
                    case ENTRY:
                        ans = check_entryORexternContent(sentence, ThirdWord,ENTRY);
                        if(ans)
                        {
                            strcpy(Content, ThirdWord);
                            return WordType;
                        }
                        else 
                        {
                            return -1;
                        }
                        break;

                    case EXTERN:
                        ans = check_entryORexternContent(sentence, ThirdWord,EXTERN);
                        if(ans)
                        {
                            strcpy(Content, ThirdWord);
                            return WordType;
                        }
                        else 
                        {
                            return -1;
                        }
                        break;

                    default:
                        break;
                }
            }
    }

    else
    {
        WordType = is_entry_OR_extern(FirstWord);
        switch(WordType)
        {
            case ENTRY:
                ans = check_entryORexternContent(sentence, SecondWord, ENTRY);
                if(ans)
                {
                    strcpy(Content, SecondWord);
                    return WordType;
                }
                else // it's .entry sentence with invalid content (e.g. : " .entry     \n"  - the entry statement got no label)
                {
                    return -1;
                }
                break;

            case EXTERN:
                ans = check_entryORexternContent(sentence, SecondWord, EXTERN);
                if(ans)
                {
                    strcpy(Content, SecondWord);
                    return WordType;
                }
                else // it's .extern sentence with invalid content (e.g. : " .extern     \n"  - the entry statement got no label)
                {
                    return -1;
                }
                break;

            default:
                break;
        }
    }    
     /**
      * if there's a label check if the (second) word is .data\.string (i.e. "Main12: .string "something"")
      * if there isn't a label check if the (first) word is .data\.string (i.e. ".string "something"")
     */
    if(*LabelFlag)
    {
        WordType = is_Data_OR_String(SecondWord);
    }
    else
    {
        WordType = is_Data_OR_String(FirstWord);
    }

    switch (WordType)
    {
        case DATA:
            ans = check_DataContent(sentence, Content);
            if(ans)
                return WordType;
            else
                return -1;
            break;

        case STRING:
            ans = check_StringContent(sentence, Content);
            if(ans)
                return WordType;
            else
                return -1;
            break;

        default:
            break;
    }

    error = NO_ERROR;
    return 0;
}

/**
 * checks if the instruction sentence is valid and checks for a label
 * Returns 1 if it's valid
 * Returns 0 if it's not instruction line
 * !Returns -1 if content is not valid (like invalid label or invalid content of instruction)
 * if it's valid (returns 1) and there's a label then: labelFlag = true
*/
int check_Instruction_Sentence(char* sentence, bool* LabelFlag, char* Content)
{
    word_type WordType=0;
    char FirstWord[MaxLineLength];
    char SecondWord[MaxLineLength];
    int ans;
    int numOfOp;

    sscanf(sentence, "%s %s", FirstWord, SecondWord);
    if(!(is_Instruction(FirstWord)))
    {
        WordType = is_Label(FirstWord);
    }

    if(is_error(error)) // there's a label and it's invalid
    {
        return -1;
    }

    if(WordType == LABEL)
    {   
        *LabelFlag = true;
        WordType = is_Instruction(SecondWord);
        if (WordType == INSTRUCTION)
        {
            char* command = Find_Which_Instruction_name(SecondWord);
            ans = check_instruction_Content(sentence,command, Content, &num_of_op);
            if(ans)
                return WordType;
            else
                return -1;
        }
    } 

    else
    {
        WordType = is_Instruction(FirstWord);
        if (WordType == INSTRUCTION)
        {
            char* command = Find_Which_Instruction_name(FirstWord);
            ans = check_instruction_Content(sentence,command, Content, &num_of_op);
            if(ans)
                return WordType;
            else
                return -1;
        }
    } 

    return 0;
}

/**
 * Returns 1 if instruction content is valid
 * Returns 0 if it's not valid
 * 
 * if it's valid (returns 1):
 * content gets the content string
 * Num_Of_OP gets the number of oparends of the instruction
*/
int check_instruction_Content(char* Sentence,char* instruction,char* content, int* Num_Of_OP)
{
    char* subString = strstr(Sentence,instruction);  
    char* res = subString + strlen(instruction);
    char* comma = ",";
    char* tok = "\n\t ";
    char* source = "source";
    char* target = "target";
    char res_copy[MaxLineLength];
    char res_with_whitespaces[MaxLineLength];
    strcpy(res_with_whitespaces,res);
    strcpy(res_copy, res);
    deleteblankspaces(res_copy,0,0);
    char content_copy[MaxLineLength];
    strcpy(content_copy, res_copy);
    Legal_Methods Addressing_Type = 0;
    char* target_operand;
    char* source_operand;
    int comma_count = count_commas(res_copy);
    int check_num_op = count_operands(res_with_whitespaces);

    if(check_num_op > 2) // any instruction can have at most 2 operands
    {
        error = COMMAND_TOO_MANY_OPERANDS;
        return 0;
    }

    if(check_num_op == 2 && comma_count == 0)
    {
        error = EXPECTED_COMMA_BETWEEN_OPERANDS;
        return 0;
    }

    if(comma_before_firstOp(res_copy)) // checks there isn't comma before the first operand (if exists)
    {
        error = COMMAND_COMMA_BEFORE_FIRST_OP;
        return 0;
    }

    if(comma_count > 1) // any instruction line can have at most 1 comma (between 2 operands)
    {
        error = COMMAND_TOO_MANY_COMMAS;
        return 0;
    }

    switch (check_num_op)
    {
        case 1:
            if(comma_count > 0)
            {
                error = COMMAND_UNEXPECTED_COMMA;
                return 0;
            }
            break;
        
        case 2:
            if(comma_after_lastOp(res_copy))
            {
                error = COMMAND_UNEXPECTED_COMMA;
                return 0;
            }
            break;
        
        default:
            break;
    }

    if(strchr(res_copy, ',') == NULL)
    {
        source_operand = NULL;
        target_operand = strtok(res_copy, tok);
    }
    else
    {
        source_operand = strtok(res_copy, comma);
        target_operand = strtok(NULL, tok);
    }
    
    

    /* char* check = strtok(NULL, comma);

    if (check != NULL)
    {
        printf("Too many operands");
    } */

    bool valid_SrcMethod = is_valid_Addressing_Method(instruction, source_operand, source);
    bool valid_TrgMethod = is_valid_Addressing_Method(instruction, target_operand, target);

    if(valid_SrcMethod && valid_TrgMethod)
    {
        if (source_operand == NULL && target_operand == NULL)
        {
            *Num_Of_OP = 0;
        }
        else if (source_operand == NULL && target_operand != NULL)
        {
            *Num_Of_OP = 1;
        }
        else
        {
            *Num_Of_OP = 2;
        }
        strcpy(content, content_copy);
        return 1;
    }
    
    error = COMMAND_ILLEGAL_OPERANDS_METHOD;
    return 0;
}

//checks if the given string contains just whitespaces 
int is_empty(char *s) 
{
  while (*s != '\0') {
    if (!isspace((unsigned char)*s))
      return 0;
    s++;
  }
  return 1;
}

int count_commas(char *s) 
{
    int count = 0;
    char *p;
    for(p = s; (p = strchr(p,',')) != NULL; ++p)
    {
        ++count;
    }

    return count;
}

int comma_before_firstOp(char *s)
{
    char p[MaxLineLength];
    strcpy(p,s);
    char k = '\0';
    sscanf(p, "%c", &k);

    if(k == ',')
    {
        return 1;
    }

    return 0;
}

int comma_after_lastOp(char *s)
{
    char p[MaxLineLength];
    strcpy(p,s);
    char *temp = strchr(p,',');
    temp += 1;
    temp = strtok(temp," \t\n");
    if(temp == NULL)
    {
        return 1;
    }

    return 0;
}

int count_operands(char *s)
{
    char p[MaxLineLength];
    strcpy(p,s);
    char *operand;
    int count = 0;

    operand = strtok(p," ,\t\n");
    while(operand != NULL)
    {
        count++;
        operand = strtok(NULL," ,\t\n");
    }

    return count;
}


/**
 * !Return 0 if Method not found
 * Return 1 if Instant addressing (enum Instant_addressing count)
 * Return 3 if Direct addressing (enum Direct_addressing count)
 * Return 5 if Direct register addressing (enum Direct_register_addressing count)
 */
int check_Addressing_Method(char* word)
{
    if(word == NULL)
    {
        return 0;
    }
    Legal_Methods Addressing_Type = 0;
    int num;
    bool label = false;
    char* reg = is_register(word);

    if(sscanf(word, "%d", &num) == 1)
    {
        Addressing_Type = Instant_addressing; //1
    }
    else if (reg != NULL)
    {
        Addressing_Type = Direct_register_addressing; //5
    }
    else
    {
        label = is_LabelWithoutColon(word);
        if(label)
            Addressing_Type = Direct_addressing; //3
    }


    return Addressing_Type;
}

/**
 * Checks if it's valid addressing method for instruction sentences
 * if it's valid: returns 1
 * if it's not: returns 0
*/
int is_valid_Addressing_Method(char* Instruction, char* Operand, char* Operand_type)
{
    int i = 0;
    int j;
    char* command;
    Legal_Methods Addressing_Type = 0;
    Addressing_Type = check_Addressing_Method(Operand);

    while (i < 16)
    {
        command = instruction_opCode[i].name;

        if (strcmp(Instruction, command) == 0)
        {
            if (strcmp(Operand_type, "source") == 0)
            {
                int* legal_methods;
                legal_methods = instruction_legalMethods[i].Src_lglAddressingMethod;
                int arr_length = sizeof(instruction_legalMethods[i].Src_lglAddressingMethod)/sizeof(*legal_methods);
                
                for (j = 0; j < arr_length; j++)
                {
                    if(*(legal_methods) != 0 && Addressing_Type == 0)
                    {
                        return 0;
                    }

                    if(Addressing_Type == *(legal_methods+j))
                    {
                        return 1;
                    }
                }
            }

            else //(Operand_type == "target")
            {
                int* legal_methods = instruction_legalMethods[i].Dest_lglAddressingMethod;
                int arr_length = sizeof(instruction_legalMethods[i].Dest_lglAddressingMethod)/sizeof(*legal_methods);

                for (j = 0; j < arr_length; j++)
                {
                    if(*(legal_methods) != 0 && Addressing_Type == 0)
                    {
                        return 0;
                    }

                    if(Addressing_Type == *(legal_methods+j))
                    {
                        return 1;
                    }
                }
            }
        }
        i++;
    }
    return 0;
}


int check_content_length(char* content, word_type Type)
{
    int ans;
    int count = 0;
    int j = 0;
    int i;
    char k;
    char* copy_content;

    switch (Type)
    {
        case DATA:
            copy_content = (char*)malloc((strlen(content) + 1) * sizeof(char));
            strcpy(copy_content, content);
            char* temp = copy_content;

            ans = sscanf(temp, "%d%c", &i, &k);
            count++;
            while(ans == 2)
            {
                temp = strchr(temp, ',');
                temp = temp + 1;
                ans = sscanf(temp, "%d%c", &i, &k);
                count++;
            }

            free(copy_content);
            break;

        case STRING:
            count = strlen(content)+1;
            break;
        
        default:
            break;
    }

    return count;
}

void free_Symbol_TABLE(Symbols** symbolTable, int numOfLabels)
{
    int i = 0;
    Symbols* curr = *symbolTable;

    while(i < numOfLabels)
    {
        Symbols* prev = curr;
        i++;
        curr += 1;
        free(prev->Symbol_name);
    }
}

void freeTABLE(binary_code** Table)
{
    binary_code* curr = *Table;
    int i=0;

    while(curr != NULL)
    {
        binary_code* prev = curr;
        i++;
        curr= curr+i;
        free(prev);
    }
}

/**
 * encoding the content of .data or .string lines to binary 
 * returns array of binary codes 
*/
binary_code *getBinaryCodeFromData(char *content, bool isNumber, int *length)
{
    binary_code *res;
    if (isNumber) // dealing with this case: .data 6,-9,15
    {
        int size, i;
        int *numbers = getNumberFromString(content, &size);
        res = (binary_code *)malloc(size * sizeof(binary_code));
        CheckMemoryAllocation(res);

        for (i = 0; i < size; i++)
        {
            res[i].machineCode = (unsigned int)numbers[i];
            if (numbers[i] < 0)
            {
                res[i].machineCode *= (-1);
                res[i].machineCode = ~res[i].machineCode;
                res[i].machineCode += 1;
            }
        }
        free(numbers);
        *length = size;
    }
    else // dealing with this case: .string "abcdef"
    {
        int i, len;
        len = strlen(content);
        res = (binary_code *)malloc((len + 1) * sizeof(binary_code));
        CheckMemoryAllocation(res);

        for (i = 0; i < len; i++)
        {
            res[i].machineCode = (unsigned int)content[i];
           
        }

        res[len].machineCode = 0; // '\0' = 000000000000
       
        *length = len + 1;
    }

    return res;
}

//                 org Number  new value     in place    num of bits to modify
TBYTE changeBitsValue(TBYTE num, TBYTE value, int startBit, int numBits)
{
    TBYTE res = num;
    int i;
    for (i = 0; i < numBits; i++)
    {
        TBYTE mask = 1 << (i + startBit);
        mask = ~mask;
        res &= mask;
    }

    value <<= startBit;
    res |= value;
    return res;
}

int *getNumberFromString(char *str, int *size)
{
    const char tokens[] = ",";
    int *arr = (int *)malloc(sizeof(int));
    CheckMemoryAllocation(arr);
    char *cpyStr = (char *)malloc((strlen(str) + 1) * sizeof(char));
    CheckMemoryAllocation(cpyStr);
    char *numStr;
    int pSizeSymbolTable = 1;
    int indexSymbolTable = 0;

    strcpy(cpyStr, str);

    numStr = strtok(cpyStr, tokens);

    while (numStr != NULL)
    {
        arr = (int *)checkAndResizeArray(arr, sizeof(int), indexSymbolTable, &pSizeSymbolTable);
        sscanf(numStr, "%d", &(arr[indexSymbolTable]));
        indexSymbolTable++;
        numStr = strtok(NULL, tokens);
    }
    free(cpyStr);
    *size = indexSymbolTable;
    return arr;
}

TBYTE get_EncodeFromOP(Legal_Methods Addressing_Type, char *Operand, char *srcORtarget)
{
    TBYTE encoding = 0;
    int operand;

    switch (Addressing_Type)
    {
    case Instant_addressing: // number
        sscanf(Operand, "%d", &operand);
        operand = (unsigned int)operand;
        if (operand < 0)
        {
            operand *= (-1);
            operand = ~operand;
            operand += 1;
        }
        encoding = changeBitsValue(0, operand, 2, 10);
        break;

    case Direct_addressing:
        // ! it's a label we need to know the label address to encode this line

        break;

    case Direct_register_addressing: // register
        sscanf(Operand, "@r%d", &operand);
        if (strcmp(srcORtarget, "source") == 0)
        {
            encoding = changeBitsValue(0, operand, 7, 5);
        }
        else
        {
            encoding = changeBitsValue(0, operand, 2, 5);
        }
        break;

    default:
        break;
    }

    return encoding;
}

char *getLabelNameFromLine(char *line)
{
    char name[MaxLineLength];
    char *res;
    sscanf(line, " %[^:]", name);
    int nameLen = strlen(name);
    res = (char *)malloc((nameLen + 1) * sizeof(char));
    CheckMemoryAllocation(res);
    strcpy(res, name);
    return res;
}

/**              destination= ptr to big array
 *                                 src=the array to copy from
 *                                                 startIndex= ptr to number of the first cell to write to
 *                                                                  count= how many cells to copy
 *                                                                          pSize= ptr to the physical size of the big array
 */
void TableCpy(binary_code **des, binary_code *src, int *startIndex, int count, int *pSize)
{
    int i;
    binary_code *res = *des;
    for (i = 0; i < count; i++)
    {
        res = (binary_code *)checkAndResizeArray(res, sizeof(binary_code), *startIndex + i, pSize);
        res[*startIndex + i] = src[i];
    }
    *des = res;
}



binary_code *getBinaryCodeFromInstructions(char *Content, char *Instruction, int numOfOperands, int *num_of_encoding_words)
{
    Legal_Methods Src_Addressing_Type = 0;
    Legal_Methods Target_Addressing_Type = 0;
    char *SourceOP;
    char *TargetOP;
    char str1[MaxLineLength];
    char str2[MaxLineLength];
    char str[MaxLineLength];
    switch (numOfOperands)
    {
    case 0:
        SourceOP = NULL;
        TargetOP = NULL;
        break;

    case 1:
        SourceOP = NULL;
        sscanf(Content, "%s", str);
        TargetOP = str;
        Target_Addressing_Type = check_Addressing_Method(TargetOP);
        break;

    case 2:
        sscanf(Content, "%[^,],%s", str1, str2);
        SourceOP = str1;
        TargetOP = str2;
        Src_Addressing_Type = check_Addressing_Method(SourceOP);
        Target_Addressing_Type = check_Addressing_Method(TargetOP);
        break;

    default:
        break;
    }

    binary_code *IC_Table = (binary_code *)malloc(sizeof(binary_code));
    CheckMemoryAllocation(IC_Table);
    int pSizeSymbolTable = 1;
    int indexSymbolTable = 0;
    int opcode;
    int Target_operand;
    int SRC_operand;
    TBYTE encoding_opcodeLine, LINE;
    opcode = Find_Instruction_OPcode(Instruction);
    encoding_opcodeLine = changeBitsValue(0, opcode, 5, 4);

    switch (numOfOperands)
    {
    case 0:
        IC_Table[0].machineCode = encoding_opcodeLine;
        *num_of_encoding_words = 1;
        break;

    case 1:
        indexSymbolTable++;
        IC_Table = (binary_code *)checkAndResizeArray(IC_Table, sizeof(binary_code), indexSymbolTable, &pSizeSymbolTable);
        TBYTE encoding_TargetLine;

        encoding_opcodeLine = changeBitsValue(encoding_opcodeLine, Target_Addressing_Type, 2, 3);
        IC_Table[0].machineCode = encoding_opcodeLine;
        IC_Table[1].machineCode = get_EncodeFromOP(Target_Addressing_Type, TargetOP, "target");
        *num_of_encoding_words = 2;
        break;

    case 2:
        indexSymbolTable++;
        IC_Table = (binary_code *)checkAndResizeArray(IC_Table, sizeof(binary_code), indexSymbolTable, &pSizeSymbolTable);

        encoding_opcodeLine = changeBitsValue(encoding_opcodeLine, Src_Addressing_Type, 9, 3);
        encoding_opcodeLine = changeBitsValue(encoding_opcodeLine, Target_Addressing_Type, 2, 3);
        IC_Table[0].machineCode = encoding_opcodeLine;

        // both operands are registers
        if (Src_Addressing_Type == Direct_register_addressing && Target_Addressing_Type == Direct_register_addressing)
        {
            sscanf(TargetOP, "@r%d", &Target_operand);
            sscanf(SourceOP, "@r%d", &SRC_operand);
            LINE = changeBitsValue(0, Target_operand, 2, 5);
            LINE = changeBitsValue(LINE, SRC_operand, 7, 5);
            IC_Table[1].machineCode = LINE;
            *num_of_encoding_words = 2;
        }
        else
        {
            indexSymbolTable++;
            IC_Table = (binary_code *)checkAndResizeArray(IC_Table, sizeof(binary_code), indexSymbolTable, &pSizeSymbolTable);
            IC_Table[1].machineCode = get_EncodeFromOP(Src_Addressing_Type, SourceOP, "source");
            IC_Table[2].machineCode = get_EncodeFromOP(Target_Addressing_Type, TargetOP, "target");
            *num_of_encoding_words = 3;
        }

        break;

    default:
        break;
    }

    return IC_Table;
}



/** check the sentence type:
* if it's an empty line: Returns 1 (EMPTY enum count)
* if it's comment: Returns 2 (COMMENT enum count)
* if it's guiding: Returns 3 (GUIDING enum count)
* if it's command: Returns 4 (COMMAND enum count)
*/
int Sentence_type(char *sentence, bool *LabelFlag, char *content)
{
    char FirstWord[MaxLineLength];
    char SecondWord[MaxLineLength];
    Sentence_Structure SentenceType;

    if (sscanf(sentence, " %[^\n\t ] ", FirstWord) == EOF)
    {
        SentenceType = EMPTY; // 1
        return SentenceType;
    }
    else if (sentence[0] == ';')
    {
        SentenceType = COMMENT; // 2
        return SentenceType;
    }
    else if (check_Guiding_Sentence(sentence, LabelFlag, content))
    {
        if(is_error(error))
            return -1;
        
        SentenceType = GUIDING; // 3
        return SentenceType;
    }
    else if (check_Instruction_Sentence(sentence, LabelFlag, content))
    {
        if(is_error(error))
            return -1;
        
        SentenceType = COMMAND; // 4
        return SentenceType;
    }
    
    error = SYNTAX_ERROR;
    return 0;
}


/**
 * after the first pass this function updates all the addresses of symbols of type data (S_DATA or S_STRING) by increasing all of them by IC (instruction counter)
 * that separates the program to two sections the first for instructions and the second the data (.data  or .string)
*/
void update_DataSymbolsAddress(Symbols* symbolTable, int instruction_counter)
{
    int i = 0;
    while(symbolTable[i].Symbol_name)
    {
        if(symbolTable[i].type == S_DATA || symbolTable[i].type == S_STRING)
        {
            symbolTable[i].decimal_address += instruction_counter;
        }
        i++;
    }
}


/**
 * Checks if the label is already exists in the symbol table
 * Returns 1 if label isn't found (i.e. it's a new label so we can add it to the table)
 * Returns 0 if label is found (i.e. it's not new label)
*/
int newLabel(Symbols* symbolTable, char *Label, int label_counter)
{
    int i = 0;
    while(i < label_counter)
    {
        if(strcmp(symbolTable[i].Symbol_name,Label) == 0)
        {
            return 0;
        }
        i++;
    }
    return 1;
}


/**
 * add .entry to the label type in our symbol table
*/
void Handle_Entry_LabelEncoding(Symbols *symbolTable, char *Label, int label_count)
{
    
    Symbols *temp = symbolTable;
    int i = 0;
    while(i < label_count)
    {
        if(strcmp(temp[i].Symbol_name, Label) == 0)
        {
            temp[i].type |= S_ENTRY;
        }
        i++;
    }
}

int symbolIsExtern(Symbols *symbolTable, char *Label, int numOfLabels)
{
    
    Symbols *temp = symbolTable;
    int i = 0;
    while(i < numOfLabels)
    {
        if(temp[i].type == S_EXTERN && strcmp(temp[i].Symbol_name, Label) == 0)
        {
            return 1;
        }
        i++;
    }

    return 0;
}


void Handle_instruction_LabelEncoding(Symbols *symbolTable, binary_code* icTable , char *content, int numOfOperands, int indexOfIc, ext_symbols *externals, int* indexExtTable, int label_count)
{
    TBYTE encoding = 0;
    Legal_Methods source_Addressing_Type = 0;
    Legal_Methods target_Addressing_Type = 0;
    char* comma = ",";
    char* tok = "\n\t ";
    char* target_operand;
    char* source_operand;
    int decimal_address = 0;
    int currDecAddress = 100;
    

    switch (numOfOperands)
    {
        case 0:
            break;
        
        case 1:
            source_Addressing_Type = check_Addressing_Method(content);
            if(source_Addressing_Type == Direct_addressing ) // if it's true than the content is a label (/Symbol) 
            {
                if(newLabel(symbolTable,content,label_count))
                {
                    error = COMMAND_LABEL_DOES_NOT_EXIST;
                    break;
                }

                if(!(symbolIsExtern(symbolTable, content,label_count))) //returns true if label is not extern
                {
                    decimal_address = find_addressFromST(symbolTable, content);
                    encoding = changeBitsValue(0, 2, 0, 2);
                    encoding = changeBitsValue(encoding, decimal_address, 2, 10);
                    icTable[indexOfIc].machineCode = encoding;
                }
                else // it is an extern label => well save its address as the encode
                {
                    int len = (int)strlen(content);
                    externals[*indexExtTable].Symbol_name = (char *)malloc((len + 1) * sizeof(char));
                    CheckMemoryAllocation(externals[*indexExtTable].Symbol_name);
                    strcpy(externals[*indexExtTable].Symbol_name,content);
                    externals[*indexExtTable].Symbol_name[len] = '\0';
                    externals[*indexExtTable].decimal_address =  currDecAddress + indexOfIc;
                    *indexExtTable += 1;

                    icTable[indexOfIc].machineCode = 1;
                }
                
            }
            break;
        
        case 2:
            source_operand = strtok(content, comma);
            target_operand = strtok(NULL, tok);
            source_Addressing_Type = check_Addressing_Method(source_operand);
            target_Addressing_Type = check_Addressing_Method(target_operand);

            if(source_Addressing_Type == Direct_addressing) // its a label
            {
                if(newLabel(symbolTable,source_operand,label_count))
                {
                    error = COMMAND_LABEL_DOES_NOT_EXIST;
                    break;
                }

                if(!(symbolIsExtern(symbolTable, source_operand,label_count))) //returns true if label is not extern
                {
                    int index_source = indexOfIc - 1;
                    decimal_address = find_addressFromST(symbolTable, source_operand);
                    encoding = changeBitsValue(0, 2, 0, 2);
                    encoding = changeBitsValue(encoding, decimal_address, 2, 10);
                    icTable[index_source].machineCode = encoding;
                }
                 else // it is an extern label => well save its address as the encode
                {
                    int index_source = indexOfIc - 1;
                    int len = (int)strlen(source_operand);
                    externals[*indexExtTable].Symbol_name = (char *)malloc((len + 1) * sizeof(char));
                    CheckMemoryAllocation(externals[*indexExtTable].Symbol_name);
                    strcpy(externals[*indexExtTable].Symbol_name,source_operand);
                    externals[*indexExtTable].Symbol_name[len] = '\0';
                   
                    externals[*indexExtTable].decimal_address =  currDecAddress + index_source;
                    *indexExtTable += 1;

                    icTable[index_source].machineCode = 1;
                }
            }

            if(target_Addressing_Type == Direct_addressing) // its a label
            {
                if(newLabel(symbolTable,target_operand,label_count))
                {
                    error = COMMAND_LABEL_DOES_NOT_EXIST;
                    break;
                }

                if(!(symbolIsExtern(symbolTable, target_operand,label_count))) //returns true if label is not extern
                {
                    decimal_address = find_addressFromST(symbolTable, target_operand);
                    encoding = changeBitsValue(0, 2, 0, 2);
                    encoding = changeBitsValue(encoding, decimal_address, 2, 10);
                    icTable[indexOfIc].machineCode = encoding;
                }
                else // it is an extern label => well save its address as the encode
                {
                    int len = (int)strlen(target_operand);
                    externals[*indexExtTable].Symbol_name = (char *)malloc((len + 1) * sizeof(char));
                    CheckMemoryAllocation(externals[*indexExtTable].Symbol_name);
                    strcpy(externals[*indexExtTable].Symbol_name,target_operand);
                    externals[*indexExtTable].Symbol_name[len] = '\0';
                    
                    externals[*indexExtTable].decimal_address =  currDecAddress + indexOfIc;
                    *indexExtTable += 1;

                    icTable[indexOfIc].machineCode = 1;
                }
            }
            break;
        
        default:
            break;
    }

}

int find_addressFromST(Symbols *symbolTable,char *content)
{
    Symbols *temp = symbolTable;

    while(temp[0].Symbol_name)
    {
        if(strcmp(temp[0].Symbol_name, content) == 0)
        {
            return temp[0].decimal_address;
        }
        temp += 1;
    }

    return 0;
}

void skip_line(FILE *fp)
{
    int c;

    while(c = fgetc(fp), c != '\n' && c != EOF);
}

