#include "libraries.h"

int is_error(int error)
{
    return error != NO_ERROR;
}

void write_error(int lineCounter,int error){
    if(lineCounter)
        fprintf(stderr, "FOUND ERROR IN (line %d): ", lineCounter);
        
    switch(error){
        case SYNTAX_ERROR:
            fprintf(stderr, "First non-space character must be a letter a dot or ';' sign.\n");
            break;

        case LABEL_ALREADY_EXISTS:
            fprintf(stderr, "Label already exists.\n");
            break;

        case LABEL_TOO_LONG:
            fprintf(stderr, "Label is too long! ,label max length is: %d.\n", Max_LabelLength);
            break;

        case LABEL_Illegal_FIRST_CHAR:
            fprintf(stderr, "Label must start with an alphabet character.\n");
            break;

        case LABEL_NOT_ALPHANUMERIC:
            fprintf(stderr, "Label must contain only alpha-numeric characters.\n");
            break;

        case LABEL_CANT_BE_SAVED_WORD:
            fprintf(stderr, "Label can't have the same name as a saved name. \n");
            break;

        case JUST_LABEL:
            fprintf(stderr, "Label must be followed by a command or a directive.\n");
            break;

        case DATA_GOT_COMMAS_IN_A_ROW:
            fprintf(stderr, ".data directive got more than 1 comma in a row.\n");
            break;

        case DATA_EXPECTED_NUM:
            fprintf(stderr, ".data directive expected a numeric parameter.\n");
            break;

        case DATA_COMMA_AFTER_LAST_OP:
            fprintf(stderr, ".data directive got an unexpected comma after the last number.\n");
            break;

        case STRING_TOO_MANY_OPERANDS:
            fprintf(stderr, ".string directive must contain exactly one parameter.\n");
            break;

        case STRING_OPERAND_NOT_VALID:
            fprintf(stderr, ".string directive operand is not valid.\n");
            break;

        case EXTERN_GOT_NO_LABEL:
            fprintf(stderr, ".extern directive must be followed by a label.\n");
            break;

        case EXTERN_INVALID_LABEL:
            fprintf(stderr, ".extern directive received invalid label.\n");
            break;

        case EXTERN_TOO_MANY_OPERANDS:
            fprintf(stderr, ".extern directive must only have one operand that is a label.\n");
            break;

        case EXPECTED_COMMA_BETWEEN_OPERANDS:
            fprintf(stderr, "Between 2 operands must be a comma.\n");
            break;

        case COMMAND_UNEXPECTED_COMMA:
            fprintf(stderr, "Invalid comma at end of line of the command.\n");
            break;

        case COMMAND_TOO_MANY_OPERANDS:
            fprintf(stderr, "Command can't have more than 2 operands.\n");
            break;

        case COMMAND_INVALID_NUMBER_OF_OPERANDS:
            fprintf(stderr, "Number of operands does not match command requirements.\n");
            break;

        case COMMAND_ILLEGAL_OPERANDS_METHOD:
            fprintf(stderr, "Operands addressing methods do not match command requirements.\n");
            break;

        case ENTRY_LABEL_DOES_NOT_EXIST:
            fprintf(stderr, ".entry directive must be followed by an existing label.\n");
            break;

        case ENTRY_NO_LABEL:
            fprintf(stderr, ".entry directive must be followed by a label.\n");
            break;

        case ENTRY_TOO_MANY_OPERANDS:
            fprintf(stderr, ".entry directive must only have one operand that is a label.\n");
            break;

        case ENTRY_ALREADY_EXTERN:
            fprintf(stderr, "The .entry label was already defined as external.\n");
            break;

        case LINE_TOO_LONG:
            fprintf(stderr, "The line has too many characters\n");
            break;

        case COMMAND_LABEL_DOES_NOT_EXIST:
            fprintf(stderr, "The operand label does not exist.\n");
            break;

        case CANNOT_OPEN_FILE:
            fprintf(stderr, "Cannot open file!.\n");
            break;

        case COMMAND_COMMA_BEFORE_FIRST_OP:
            fprintf(stderr, "Invalid comma before the first operand!\n");
            break;

        case COMMAND_TOO_MANY_COMMAS:
            fprintf(stderr, "Command can have at most one comma!\n");
            break;
    }
}