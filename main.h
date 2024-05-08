#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_LENGTH 1000
#define MAX_SYMBOLS 100
#define MAX_CHAR 32
#define SYSCALL 0x00000000
#define TEXTMACROS 6
#define DATAMACROS 2

typedef enum LINESTATE {
    LS_NONE = 0,
    LS_INCLUDE = 1,
    LS_TEXT = 2,
    LS_DATA = 3
}LINETYPE;

typedef enum ADDRESS {
    PC_ADDRESS = 0x00400000,
    DATA_ADDRESS = 0x10000000,
    MACRO_ADDRESS = 0x10004000
}ADDRESS;

typedef enum LOGLEVEL {
    LL_NONE = 0,
    LL_INFO = 1,
    LL_ERROR = 2,
    LL_DEBUG = 3,
    LL_ALL = 4
}LOGLEVEL;

typedef enum REGISTER {
    ZERO = 0,               // $0
    ASSEMBLER_TEMP = 1,     // $at
    VALUE = 2,              // $v0 - $v1
    ARGUMENT = 4,           // $a0 - $a3
    TEMP1 = 8,              // $t0 - $t7
    SAVED = 16,             // $s0 - $s7
    TEMP2 = 24,             // $t8 - $t9
    KERNEL = 26,            // $k0 - $k1
    GLOBAL_POINTER = 28,    // $gp
    STACK_POINTER = 29,     // $sp
    FRAME_POINTER = 30,     // $fp
    RETURN_ADDRESS = 31     // $ra 
}REGISTER;

typedef struct MACRO {
    char *name;
    int address;
    // void(*function)(void); To be added
}MACRO;

typedef struct SYMBOL{
    char symbol[MAX_CHAR];
    int address;
}SYMBOL;

int32_t Registers[32];
LOGLEVEL gLogLevel;

bool isTextMacro(char Line[MAX_LENGTH]);
bool isDataMacro(char Line[MAX_LENGTH]);
bool isLabel(char Line[MAX_LENGTH]);
bool isDuplicate(char *symbol);
char *ExtractArgument(char *Line);
char *ExtractLabel(char *Line);
void CloseFiles(void);
void LogMessage(LOGLEVEL LogLevel, char* Message, ...);