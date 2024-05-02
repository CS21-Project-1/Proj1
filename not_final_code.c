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

    // Read the number of lines
    if (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        num_lines = atoi(line);
    } else {
        fprintf(stderr, "Error reading number of lines\n");
        fclose(file);
        return 1;
    }

    // Read and process each line of MIPS code
    for (int i = 0; i < num_lines; i++) {
        if (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
            printf("Line %d: %s", i + 1, line); // Debug print
            // Skip lines that don't contain MIPS instructions
            if (strstr(line, ".") == NULL) {
                printf("Address 0x%08x\n", address);
                address += 4; // Increment address by 4 for each instruction
            }
        } else {
            fprintf(stderr, "Error reading line %d\n", i + 1);
            fclose(file);
            return 1;
        }
    }

    // Close file
    fclose(file);

    return 0;
}
