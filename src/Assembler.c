#include "libraries.h"


int main(int argc, char *argv[]) 
{
    FILE *oldFile;
    FILE *newFile;
    FILE *obj;
    FILE *ent;
    FILE *ext;
    char Original_FileName[MaxLineLength];
    char Opening_FileName[MaxLineLength];
    char FileNameWithAm[MaxLineLength];  //Output file for macros expansion
    char FileNameWithOb[MaxLineLength];  //Output file for object file
    char FileNameWithEnt[MaxLineLength]; //Output file for .entry symbols
    char FileNameWithExt[MaxLineLength]; //Output file for .extern symbols
    Symbols* Symbol_TableOUT;
    binary_code* DC_TableOut;
    binary_code* IC_TableOut;
    ext_symbols *ExtTableOut;
    int IC;
    int DC;
    int label_count;
    bool entry_label;
    int extern_LabelsCount;
    int File_ind;

    if(argc < 2)
    {
        puts("You have to give at least 2 arguments for this function!");
        return 1;
    }

    for(File_ind = 1; File_ind < argc; File_ind++)
    {
        label_count = 0;
        extern_LabelsCount = 0;
        entry_label = false;

        strcpy(Original_FileName, argv[File_ind]);
        strcpy(Opening_FileName, Original_FileName);
        strcpy(FileNameWithAm, Original_FileName);
        strcpy(FileNameWithOb, Original_FileName);
        strcpy(FileNameWithEnt, Original_FileName);
        strcpy(FileNameWithExt, Original_FileName);
    
        strcat(Opening_FileName, ".as"); 
        strcat(FileNameWithAm, ".am");  

        oldFile = fopen(Opening_FileName, "r+");
        if(oldFile == NULL)
        {
            error = CANNOT_OPEN_FILE;
            write_error(0,error);
            continue;
        }
        
        newFile = fopen(FileNameWithAm, "w+");
        CheckFiles(newFile);
    
        if (!macroExpansion(oldFile, newFile))
        {
            exit(1);
        } 
    
        if(!FirstPass(newFile, &Symbol_TableOUT, &DC_TableOut, &IC_TableOut, &IC, &DC, &label_count)) 
        {
            printf("\nError found during first-pass therefore we'll not continue to second-pass, we'll continue to the next file!\n\n");
            continue;
        }

        if(!SecondPass(newFile, Symbol_TableOUT, IC_TableOut, &ExtTableOut, &extern_LabelsCount, label_count, &entry_label))
        {
            printf("\nError found during second-pass therefore we'll not export the related documents and continue to the next file!\n\n");
            continue;
        }
        

        if(extern_LabelsCount > 0)
        {
            strcat(FileNameWithExt, ".ext"); 
            ext = fopen(FileNameWithExt, "w+");
            CheckFiles(ext);

            createExtFile(ext,ExtTableOut,extern_LabelsCount);
        }

        strcat(FileNameWithOb, ".ob"); 
        obj = fopen(FileNameWithOb, "w+");
        CheckFiles(obj);
        
        createObjFile(obj,IC_TableOut,DC_TableOut, IC, DC);

        if(entry_label)
        {
            strcat(FileNameWithEnt, ".ent"); 
            ent = fopen(FileNameWithEnt, "w+");
            CheckFiles(ent);
            
            createEntFile(ent,Symbol_TableOUT,label_count);
        }


        free_Symbol_TABLE(&Symbol_TableOUT, label_count);
        fclose(newFile);
        fclose(oldFile);
        fclose(ext);
        fclose(ent);
        fclose(obj);
    }
     
    return 0;
}


