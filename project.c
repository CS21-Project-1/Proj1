#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_LENGTH 1000

char macros[][MAX_LENGTH] = {"gcd", "print_str", "read_str", "print_integer", "read_integer", "exit"};
int registers[32] = {0};

typedef enum LINETYPE {
    LT_NONE = 0,
    LT_INCLUDE = 1,
    LT_TEXT = 2,
    LT_DATA = 3
}LINETYPE;

typedef enum ADDRESS {
    PC_ADDRESS = 0x00400000,
    DATA_ADDRESS = 0x10000000
}ADDRESS;

typedef enum LOGLEVEL {
    LL_NONE = 0,
    LL_INFO = 1,
    LL_ERROR = 2
}LOGLEVEL;

typedef struct Symbol{
    char *symbol;
    char *address;
}Symbol;

void close_fptr(FILE *mips_fptr, FILE *sym_fptr, FILE *exe_fptr, FILE *logs_fptr) {
    if(mips_fptr) fclose(mips_fptr);
    if(sym_fptr) fclose(sym_fptr);
    if(exe_fptr) fclose(exe_fptr);
    if(logs_fptr) fclose(logs_fptr);
}

void LogMessage(LOGLEVEL LogLevel, FILE *logs_fptr, char* Message, ...) {
    char SeverityString[8] = {0};
    char FormattedString[4096] = {0};
    int MessageLength = strlen(Message);
    
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

int isMacro(char line[MAX_LENGTH]) {
    for(int i = 0; i < _countof(macros); i++) {
        if(strstr(line, macros[i]) != NULL)
            return 1;
    }
    return 0;
}

int main() {
    FILE *mips_fptr = NULL;
    FILE *sym_fptr = NULL;
    FILE *exe_fptr = NULL;
    FILE *logs_fptr = NULL;
    Symbol *SymbolTable;
    LINETYPE type = LT_NONE;
    char line[MAX_LENGTH];
    int num_lines = 0;
    int program_counter = 0;
    int data_counter = 0;

    // For logging purposes
    if((logs_fptr = fopen("project.log", "w")) == NULL) {
        goto Exit;
    }
    
    // Open the files to read and write -- mips.txt, symboltable.txt, execute.txt
    if((mips_fptr = fopen("mips.txt", "r")) == NULL) {
        LogMessage(LL_ERROR, logs_fptr, "[%s] Failed to read mips.txt.\n", __FUNCTION__);
        goto Exit;
    }
    if((sym_fptr = fopen("symboltable.txt", "w")) == NULL) {
        LogMessage(LL_ERROR, logs_fptr, "[%s] Failed to read symboltable.txt.\n", __FUNCTION__);
        goto Exit;
    }
    if((exe_fptr = fopen("execute.txt", "w")) == NULL) {
        LogMessage(LL_ERROR, logs_fptr, "[%s] Failed to read execute.txt.\n", __FUNCTION__);
        goto Exit;
    }
    LogMessage(LL_INFO, logs_fptr, "[%s] Successfully opened files.\n", __FUNCTION__);

    // Read the number of lines
    if(fgets(line, MAX_LENGTH, mips_fptr) != NULL) {
        num_lines = atoi(line);
        LogMessage(LL_INFO, logs_fptr, "[%s] Number of lines: %d.\n", __FUNCTION__, num_lines);
    }
    else {
        LogMessage(LL_ERROR, logs_fptr, "[%s] Failed to read number of lines.\n", __FUNCTION__);
        goto Exit;
    }

    for(int i = 0; i < num_lines; i++) {
        if(fgets(line, MAX_LENGTH, mips_fptr) != NULL) {
            // Ignore whitespaces
            if(*line == '\n') {
                i--;
                continue;
            }

            if(strstr(line, ".include")) {
                type = LT_INCLUDE;
                continue;
            }
            if(strstr(line, ".text")) {
                type = LT_TEXT;
                continue;
            }
            if(strstr(line, ".data")) {
                type = LT_DATA;
                continue;
            }
            
            line[strlen(line) - 1] = line[strlen(line) - 1] == '\n' ? '\0' : line[strlen(line) - 1];
            LogMessage(LL_INFO, logs_fptr, "[%s] %d\t 0x%08x\t %s\n", __FUNCTION__, i + 1, PC_ADDRESS + (program_counter * 4), line);

            if(type == LT_TEXT) {
                // Checks if it's a macro and extract the parameter
                if(isMacro(line)) {
                    char *start = NULL, *open = NULL, *close = NULL;
                }
                program_counter++;
            }
        }
        else {
            LogMessage(LL_ERROR, logs_fptr, "[%s] Failed to read line %d.\n", __FUNCTION__, i + 1);
            goto Exit;
        }
    }

    Exit:

    close_fptr(mips_fptr, sym_fptr, exe_fptr, logs_fptr);
    return 0;
}