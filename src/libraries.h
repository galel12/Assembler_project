#ifndef LIBRARIES_H
#define LIBRARIES_H

// **  Libraries  **

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// **  Definitions / Constants  **

#define MaxLineLength 81
#define Max_LabelLength 31
typedef unsigned short TBYTE;

// **  Structs / enums  **

typedef enum type
{
    S_INSTRUCTION, S_DATA, S_STRING, S_ENTRY=4, S_EXTERN=8 // instruction = 0000 0000 0000 (0), data = 0000 0000 0001 (1), string = 0000 0000 0010 (2), entry = 0000 0000 0100 (4), extern = 0000 0000 1000 (8)
}symbolType;

typedef struct mcro
{
    char *name;
    char **text;
    int numOfLines;
} mcro;

typedef struct Node
{
    mcro* data;
    struct Node* next;
} Node;

typedef struct McroList
{
    struct Node* head;
    struct Node* tail;
} McroList;

typedef struct Saved_Words
{
    char* words;
} Saved_Words;

typedef struct op_Code
{
    char* name;
    int decimal_code;
} op_Code;

typedef struct LegalAddressingMethods
{
    char* Method_Name;
    int Src_lglAddressingMethod[3];
    int Dest_lglAddressingMethod[3];
} LegalAddressingMethods;

typedef struct LegalNumOfOperands
{
    char* Method_Name[9];
    int NUM_OF_OPERANDS;
} LegalNumOfOperands;

typedef struct Registers
{
    char* register_name;
} Registers;

typedef struct Symbols
{
    char* Symbol_name;
    int decimal_address;
    symbolType type;

} Symbols;

typedef struct Binary_code
{
    unsigned int machineCode:12;

}binary_code;

typedef struct extSymbols
{
    char* Symbol_name;
    int decimal_address;

} ext_symbols;


typedef enum instructions
{
    mov, cmp, add, sub, not,
    clr, lea, inc, dec, jmp, 
    bne, red, prn, jsr, rts,
    stop 
}instructions;

typedef enum word_type
{
    LABEL=1, INSTRUCTION, DATA, STRING, ENTRY, EXTERN
} word_type;

typedef enum Sentence_Structure
{
    EMPTY=1, COMMENT, GUIDING, COMMAND
} Sentence_Structure;

typedef enum Legal_Methods
{
    Instant_addressing=1, Direct_addressing=3, Direct_register_addressing=5
} Legal_Methods;

typedef enum errors {
    NO_ERROR=0,SYNTAX_ERROR ,LABEL_ALREADY_EXISTS, LABEL_TOO_LONG, LABEL_Illegal_FIRST_CHAR, LABEL_NOT_ALPHANUMERIC, LABEL_CANT_BE_SAVED_WORD, JUST_LABEL,
    DATA_GOT_COMMAS_IN_A_ROW, DATA_EXPECTED_NUM, DATA_COMMA_AFTER_LAST_OP,
    STRING_TOO_MANY_OPERANDS, STRING_OPERAND_NOT_VALID,
    EXPECTED_COMMA_BETWEEN_OPERANDS,EXTERN_GOT_NO_LABEL, EXTERN_INVALID_LABEL, EXTERN_TOO_MANY_OPERANDS,
    COMMAND_UNEXPECTED_COMMA, COMMAND_TOO_MANY_OPERANDS, COMMAND_INVALID_NUMBER_OF_OPERANDS, COMMAND_ILLEGAL_OPERANDS_METHOD,
    ENTRY_LABEL_DOES_NOT_EXIST,ENTRY_NO_LABEL, ENTRY_ALREADY_EXTERN, COMMAND_LABEL_DOES_NOT_EXIST,LINE_TOO_LONG,
    CANNOT_OPEN_FILE,ENTRY_TOO_MANY_OPERANDS, COMMAND_COMMA_BEFORE_FIRST_OP,COMMAND_TOO_MANY_COMMAS
} errors;



// ** function prototypes **

void CheckFiles(FILE *ptr);

void CheckMemoryAllocation(void *ptr);

void* checkAndResizeArray(void* arr,int sizeOfElement,int logSize,int*pSize);

void trim(char * s);

int CountSpacesUntilFirstChar(char *p);

void deleteblankspaces(char *s, int i, int k);


//  **  prototypes - Pre-assembler (mcro.c)  **

McroList createMcroList(FILE* fileName);

mcro* readMcroFromFile(FILE* file);

char **readMcroDataFromFile(FILE *file,int* length);

int macroExpansion(FILE *oldfp, FILE *newfp);

void freeMcroList(McroList* lst);


// **  prototypes - First-pass  **

int FirstPass(FILE *file, Symbols **symbolTableOut, binary_code **dcTable, binary_code **icTable, int *ic, int *dc, int *labelCount);

int Sentence_type(char* sentence, bool* LabelFlag, char* p);

int not_in_saved_words(char* word);

int lineLength_isValid(char* sentence);

int is_Label(char* word);

int is_LabelWithoutColon(char* word);

int is_entry_OR_extern(char *word);

int is_Instruction(char *word);

char* Find_Which_Instruction_name(char* word);

int Find_Instruction_OPcode(char* word);

int is_Data_OR_String(char *word);

int check_Guiding_Sentence(char* sentence, bool* LabelFlag, char* Content);

int check_Instruction_Sentence(char* sentence, bool* LabelFlag, char* Content);

int check_DataContent(char* Line, char* data);

int check_StringContent(char* Line, char* data);

int check_entryORexternContent(char* Line, char* second_word, word_type entryORextern);

int check_instruction_Content(char* Sentence,char* instruction,char* content, int* Num_Of_OP);

char* is_register(char* word);

int check_Addressing_Method(char* word);

int is_valid_Addressing_Method(char* Instruction, char* Operand, char* Operand_type);

int check_content_length(char* content, word_type Type);

int* getNumberFromString(char* str,int* size);

binary_code* getBinaryCodeFromData(char* content,bool isNumber, int* length);

binary_code* getBinaryCodeFromInstructions(char* Content,char* Instruction, int numOfOperands, int* num_of_encoding_words);

TBYTE changeBitsValue(TBYTE num,TBYTE value,int startBit,int numBits);

TBYTE get_EncodeFromOP(Legal_Methods Addressing_Type,char* Operand, char* srcORtarget);

void TableCpy(binary_code** des,binary_code* src,int* startIndex,int count,int*pSize);

char *getLabelNameFromLine(char *line);

void free_Symbol_TABLE(Symbols** symbolTable, int numOfLabels);

void freeTABLE(binary_code** Table);

void update_DataSymbolsAddress(Symbols* symbolTable, int instruction_counter);

int newLabel(Symbols* symbolTable, char *Label, int label_counter);

int is_empty(char *s);

int count_commas(char *s);

int comma_before_firstOp(char *s);

int comma_after_lastOp(char *s);

int count_operands(char *s);


// **  prototypes - Second-pass  **

int SecondPass(FILE *file, Symbols *symbolTable, binary_code* IC_Table, ext_symbols **EXTERNS, int* numOfExt, int label_count, bool *entry_flag);

void Handle_Entry_LabelEncoding(Symbols *symbolTable, char *Label, int label_count);

int symbolIsExtern(Symbols *symbolTable, char *Label, int numOfLabels);

void Handle_instruction_LabelEncoding(Symbols *symbolTable, binary_code* icTable , char *content, int numOfOperands, int indexOfIc, ext_symbols *externals, int* indexExtTable, int label_count);

int find_addressFromST(Symbols *symbolTable,char *content);


// **  prototypes - FILES  **

void createExtFile(FILE *EXT_FILE, ext_symbols *EXTERNS, int numOfExt);

void createEntFile(FILE *ENT_FILE, Symbols* Symbol_Table, int label_count);

void createObjFile(FILE *OBJECT, binary_code* IC_Table, binary_code* DC_Table, int ic, int dc);

char base64_char (TBYTE n);

// **  prototypes - Error Handling  **

int is_error(int error);

void write_error(int lineCounter,int error);

void skip_line(FILE *fp);


// **  Extern variables  **

extern const Registers registers[8];

extern const op_Code instruction_opCode[16];

extern const LegalAddressingMethods instruction_legalMethods[16];

extern const LegalNumOfOperands Legal_Num_Of_Operands[3];

extern const Saved_Words saved_words[28];

extern int num_of_op;

extern int Line_counter;

extern int error;

#endif
