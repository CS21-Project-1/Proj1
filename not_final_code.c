#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_SYMBOLS 100

typedef struct {
    char symbol[MAX_LINE_LENGTH];
    char address[MAX_LINE_LENGTH];
} SymbolEntry;

int main() {
    FILE *file;
    char filename[] = "mips.txt";
    char line[MAX_LINE_LENGTH];
    int num_lines;
    SymbolEntry symbol_table[MAX_SYMBOLS];
    int symbol_count = 0;
    int address = 0x00400000; // Starting address
    
    // Open the file
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 1;
    }

    FILE *symbol_file = fopen("symboltable.txt", "a");
    if (symbol_file == NULL) {
        fprintf(stderr, "Error creating symboltable.txt\n");
        fclose(file);
        return 1;
    }

    // Read the number of lines
    if (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        num_lines = atoi(line);
    } else {
        fprintf(stderr, "Error reading number of lines\n");
        fclose(file);
        fclose(symbol_file);
        return 1;
    }

    // Read and process each line of MIPS code
    for (int i = 0; i < num_lines; i++) {
        if (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
            printf("Line %d: %s", i + 1, line); // Debug print
            //fprintf(symbol_file,"%s",line);
            // Skip lines that don't contain MIPS instructions
            if (strstr(line, ".") == NULL) {
                // Check if the line has a label
                char *token = strtok(line, ":");
                if (strcmp(token, "leonard") == 0||strcmp(token, "sturgis") == 0 || strcmp(token, "cooper") == 0) {
                    strcpy(symbol_table[symbol_count].symbol, token);
                    sprintf(symbol_table[symbol_count].address, "0x%08x", address);
                    printf("Address 0x%08x\n", address); // Print address for MIPS instruction line
                    printf("Symbol: %s, Address: %s\n", symbol_table[symbol_count].symbol, symbol_table[symbol_count].address);
                    symbol_count++;
                }
                address += 4; // Increment address by 4 for each instruction
            }
        } else {
            fprintf(stderr, "Error reading line %d\n", i + 1);
            fclose(file);
            fclose(symbol_file);
            return 1;
        }
    }

    // Output the symbol table to symboltable.txt
    for (int i = 0; i < symbol_count; i++) {
        fprintf(symbol_file, "%s\t%s\n", symbol_table[i].symbol, symbol_table[i].address);
    }

    printf("Symbol table generated and saved as symboltable.txt\n");

    // Close files
    fclose(symbol_file);
    fclose(file);

    return 0;
}
