#include "macros.c"

FILE *logs_fptr = NULL;
FILE *mips_fptr = NULL;
FILE *sym_fptr = NULL;
FILE *exe_fptr = NULL;
SYMBOL *SymbolTable[MAX_SYMBOLS];
uint32_t SymbolCount = 0;

int main() {
    LINETYPE LineState = LS_NONE;
    char Line[MAX_LENGTH];
    int num_lines = 0;
    int program_counter = 0;
    int data_counter = 0;
    int macro_counter = 0;
    gLogLevel = LL_INFO; // Enables logging

    // For logging purposes
    if(gLogLevel != LL_NONE) {
        if((logs_fptr = fopen("project.log", "w")) == NULL) {
            printf("Failed to create log file.\n");
            goto Exit;
        }
    } else remove("project.log");
    
    // Open the files to read and write -- mips.txt, symboltable.txt, execute.txt
    if((mips_fptr = fopen("mips.txt", "r")) == NULL) {
        LogMessage(LL_ERROR, "[%s] Failed to read mips.txt.\n", __FUNCTION__);
        goto Exit;
    }
    if((sym_fptr = fopen("symboltable.txt", "w")) == NULL) {
        LogMessage(LL_ERROR, "[%s] Failed to read symboltable.txt.\n", __FUNCTION__);
        goto Exit;
    }
    if((exe_fptr = fopen("execute.txt", "w")) == NULL) {
        LogMessage(LL_ERROR, "[%s] Failed to read execute.txt.\n", __FUNCTION__);
        goto Exit;
    }
    LogMessage(LL_INFO, "[%s] Successfully opened files.\n", __FUNCTION__);
    // Read the number of lines
    if(fgets(Line, MAX_LENGTH, mips_fptr) != NULL) {
        num_lines = atoi(Line);
        LogMessage(LL_INFO, "[%s] Number of lines: %d.\n", __FUNCTION__, num_lines);
    }
    else {
        LogMessage(LL_ERROR, "[%s] Failed to read number of lines.\n", __FUNCTION__);
        goto Exit;
    }

    for(int i = 0; i < num_lines; i++) {
        if(fgets(Line, MAX_LENGTH, mips_fptr) != NULL) {
            int32_t LineLen = strlen(Line) - 1;

            // Ignore whitespaces
            if(*Line == '\n') {
                i--;
                continue;
            }
            // Replaces the \n at the end of each line to \0
            Line[LineLen] = Line[LineLen] == '\n' ? '\0' : Line[LineLen];

            if(strstr(Line, ".include")) {
                LogMessage(LL_INFO, "[%s][%d] %s\n", __FUNCTION__, i + 1, Line);
                LineState = LS_INCLUDE;
                continue;
            }
            if(strstr(Line, ".text")) {
                LogMessage(LL_INFO, "[%s][%d] %s\n", __FUNCTION__, i + 1, Line);
                LineState = LS_TEXT;
                continue;
            }
            if(strstr(Line, ".data")) {
                LogMessage(LL_INFO, "[%s][%d] %s\n", __FUNCTION__, i + 1, Line);
                LineState = LS_DATA;
                continue;
            }

            if(LineState == LS_TEXT) {
                LogMessage(LL_INFO, "[%s][%d] 0x%08x\t %s\n", __FUNCTION__, i + 1, PC_ADDRESS + (program_counter * 4), Line);
                
                // Check if there's a label and extract the label
                if(isLabel(Line)) {
                    char *label = NULL, temp[MAX_LENGTH] = {'\0'};
                    strcpy_s(temp, sizeof(temp), Line);
                    label = ExtractLabel(temp);

                    if(!isDuplicate(label) && label != NULL) {
                        SYMBOL *Label = (SYMBOL*)malloc(sizeof(SYMBOL));
                        strcpy_s(Label->symbol, sizeof(Label->symbol), label);
                        Label->address = PC_ADDRESS + (program_counter * 4);
                            SymbolTable[SymbolCount] = Label;
                            SymbolCount++;
                    }
                }
                
                // Checks if it's a macro and extract the parameter
                if(isTextMacro(Line)) {
                    char *argument = NULL, temp[MAX_LENGTH] = {'\0'};
                    strcpy_s(temp, sizeof(temp), Line);
                    argument = ExtractArgument(temp);  ///////// 
                    char *arg1 = strtok(argument, ",");
                    char *arg2 = strtok(NULL, "");

                    if(arg1) {
                        if(!isDuplicate(arg1)) {
                            SYMBOL *TextMacro = (SYMBOL*)malloc(sizeof(SYMBOL));
                            strcpy_s(TextMacro->symbol, sizeof(TextMacro->symbol), arg1);
                            TextMacro->address = DATA_ADDRESS + (data_counter * 4);
                                SymbolTable[SymbolCount] = TextMacro;
                                SymbolCount++;
                                data_counter++;
                        }
                    }
                }

                program_counter++;
            }
        }
        else {
            LogMessage(LL_ERROR, "[%s] Failed to read Line %d.\n", __FUNCTION__, i + 1);
            goto Exit;
        }


    //Execute.txt
    if(isLabel(Line)) //IF LINE HAS LABEL
    {
        fprintf(exe_fptr, "%s\n",Line);
        //Extract line
        char *Line_without_label;
        char *colon_pos =strchr(Line, ':');

        if (colon_pos!= NULL)
        {
            Line_without_label=colon_pos+2;
        }

    
        //Extract operands
        char *instruction;
        char *operands;

        char *space_pos=strchr(Line_without_label,' ');

        if (space_pos!=NULL){
            *space_pos='\0';
            instruction=Line_without_label;
            operands=space_pos+1;
        }
    
        //Separate parameters
        char *tok=strtok(operands,",");
        char operand_array[3][10];
        int operand_count=0;
        
        while (tok!=0)
        {
            strcpy(operand_array[operand_count], tok);
            operand_count++;

            tok=strtok(0,",");
        }

        //print operands
        for (int i = 0; i < operand_count; i++) {
            printf("Operand %d: %s\n", i + 1, operand_array[i]);
        }
    }

    else //IF LINE IS NOT A LABEL
    {
        fprintf(exe_fptr, "%s\n",Line);
        //Extract operands
        char *instruction;
        char *operands;

        char *space_pos=strchr(Line,' ');

        if (space_pos!=NULL){
            *space_pos='\0';
            instruction=Line;
            operands=space_pos+1;
        }

        //printf("%s\n",operands);

        //Separate parameters
        char *tok=strtok(operands,",");
        char operand_array[3][10];
        int operand_count=0;
        
        while (tok!=0)
        {
            strcpy(operand_array[operand_count], tok);
            operand_count++;

            tok=strtok(0,",");
        }

        //print operands
        for (int i = 0; i < operand_count; i++) {
            printf("Operand %d: %s\n", i + 1, operand_array[i]);
        }
    }


     
    }

    for(int i = 0; i < SymbolCount; i++) {
        fprintf(sym_fptr, "%s\t 0x%08x\n", SymbolTable[i]->symbol, SymbolTable[i]->address);
    }

    

    Exit:

    CloseFiles();
    return 0;
}

bool isTextMacro(char Line[MAX_LENGTH]) {
    for(int i = 0; i < TEXTMACROS; i++) {
        if(strstr(Line, TextMacros[i]->name) != NULL)
            return true;
    }
    return false;
}

bool isDataMacro(char Line[MAX_LENGTH]) {
    for(int i = 0; i < DATAMACROS; i++) {
        if(strstr(Line, DataMacros[i]->name) != NULL)
            return true;
    }
    return false;
}

bool isLabel(char Line[MAX_LENGTH]) {
    if(strstr(Line, ":") != NULL) return true;
    return false;
}

bool isDuplicate(char *symbol) {
    for(int i = 0; i < SymbolCount; i++) {
        if(strstr(SymbolTable[i]->symbol, symbol)) {
            return true;
        }
    }
    return false;
}

char *ExtractArgument(char *Line) {
    char *substring = NULL;
    substring = strtok(Line, "(");
    substring = strtok(NULL, ")");
    LogMessage(LL_DEBUG, "[%s] Argument: %s\n", __FUNCTION__, substring);
    return substring; 
}

char *ExtractLabel(char *Line) {
    char *substring = NULL;
    substring = strtok(Line, ":");
    LogMessage(LL_DEBUG, "[%s] Label: %s\n", __FUNCTION__, substring);
    return substring;
}

void CloseFiles(void) {
    if(mips_fptr) fclose(mips_fptr);
    if(sym_fptr) fclose(sym_fptr);
    if(exe_fptr) fclose(exe_fptr);
    if(logs_fptr) fclose(logs_fptr);
}

void LogMessage(LOGLEVEL LogLevel, char* Message, ...) {
    char SeverityString[8] = {0};
    char FormattedString[4096] = {0};
    int MessageLength = strlen(Message);
    
    if(gLogLevel < LogLevel) return;

    switch(LogLevel) {
        case LL_NONE:
            return;
            break;
        case LL_INFO:
            strcpy_s(SeverityString, sizeof(SeverityString), "[INFO]");
            break;
        case LL_ERROR:
            strcpy_s(SeverityString, sizeof(SeverityString), "[ERROR]");
            break;
        case LL_DEBUG:
            strcpy_s(SeverityString, sizeof(SeverityString), "[DEBUG]");
            break;
        default:
            return;
            break;
    }

    va_list ArgPointer = NULL;
    va_start(ArgPointer, Message);
    _vsnprintf_s(FormattedString, sizeof(FormattedString), _TRUNCATE, Message, ArgPointer);
    va_end(ArgPointer);

    fprintf(logs_fptr, "%s%s", SeverityString, FormattedString);
}
