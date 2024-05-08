#include "main.h"

MACRO MacroGCD = {"gcd", MACRO_ADDRESS};
MACRO MacroPrintStr = {"print_str", MACRO_ADDRESS + 0x8};
MACRO MacroReadStr = {"read_str", MACRO_ADDRESS + 0xc};
MACRO MacroPrintInt = {"print_integer", MACRO_ADDRESS + 0x10};
MACRO MacroReadInt = {"read_integer", MACRO_ADDRESS + 0x14};
MACRO MacroExit = {"exit", MACRO_ADDRESS + 0x20};
MACRO *TextMacros[] = {&MacroGCD, &MacroPrintStr, &MacroReadStr, &MacroPrintInt, &MacroReadInt, &MacroExit};

MACRO MacroAllocateStr = {"allocate_str", DATA_ADDRESS};
MACRO MacroAllocateBytes = {"allocate_bytes", DATA_ADDRESS};
MACRO *DataMacros[] = {&MacroAllocateStr, &MacroAllocateBytes};

void gcd_macro() {

};

void printstr_macro() {

};

