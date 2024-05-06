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
    int macro_address= 0x10000000; //for macro address

    
    // Open the file
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return 1;
    }

    FILE *symbol_file = fopen("symboltable.txt", "w");
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
            if (strstr(line, ".") == NULL) {   //CHECKS IF ITS NOT .text
                
                /// THIS CHECKS IF THERE IS A MACRO AND EXTRACTS THE PARAMETER ///
                if (strstr(line, "print_str")!=NULL || strstr(line, "print_integer")!=NULL || strstr(line, "read_integer")!=NULL) // Contains a macro
                {
                    char *start = NULL;
                    char *open_parenthesis = NULL;
                    char *close_parenthesis = NULL;

                    if ((start = strstr(line, "print_str(")) != NULL ||
                        (start = strstr(line, "read_str(")) != NULL ||
                        (start = strstr(line, "print_integer(")) != NULL ||
                        (start = strstr(line, "read_integer(")) != NULL) {
                        // Find the opening parenthesis
                        open_parenthesis = strchr(start, '(');
                        if (open_parenthesis != NULL) {
                            // Find the closing parenthesis
                            close_parenthesis = strchr(open_parenthesis, ')');
                            if (close_parenthesis != NULL) {
                                // Calculate the length of the substring
                                size_t length = close_parenthesis - open_parenthesis - 1;
                                // Allocate memory for the substring
                                char *substring = malloc(length + 1);
                                if (substring != NULL) {
                                    // Copy the substring into the allocated memory
                                    strncpy(substring, open_parenthesis + 1, length);
                                    substring[length] = '\0'; // Null-terminate the string
                                    // Print or use the extracted substring
                                
                                    // Add your actions here
                                    strcpy(symbol_table[symbol_count].symbol, substring);
                                    sprintf(symbol_table[symbol_count].address, "0x%08x", macro_address);
                                    // Don't forget to free the allocated memory when done
                                    free(substring);
                                }
                            }
                        }
                    }
                /// CHECKS FOR MACRO AND EXTRACTS PARAMETER///
                
                
                    
                    
                //printf("Symbol: %s, Address: %s\n", symbol_table[symbol_count].symbol, symbol_table[symbol_count].address); print Symbol 
                symbol_count++;
                
                printf("Address 0x%08x\n", macro_address); // Print address for MIPS instruction line
                macro_address += 4; // Increment address by 4 for each instruction
                address+=4;
                }

                else if (strstr(line, "read_str")!=NULL) // IF MACRO IS READ_STR//
                {
                    char *start = strstr(line, "read_str(");
                    if (start != NULL) {
                        // Find the opening parenthesis
                        char *open_parenthesis = strchr(start, '(');
                        if (open_parenthesis != NULL) {
                            // Find the comma after the first parameter
                            char *comma = strchr(open_parenthesis, ',');
                            if (comma != NULL) {
                                // Find the closing parenthesis
                                char *close_parenthesis = strchr(comma, ')');
                                if (close_parenthesis != NULL) {
                                    // Calculate the length of the substring
                                    size_t length = comma - open_parenthesis - 1;
                                    // Allocate memory for the substring
                                    char *substring = malloc(length + 1);
                                    if (substring != NULL ) {
                                        // Copy the substring into the allocated memory
                                        strncpy(substring, open_parenthesis + 1, length);
                                        substring[length] = '\0'; // Null-terminate the string
                                        // Print or use the extracted substring
                                       strcpy(symbol_table[symbol_count].symbol, substring);
                                    sprintf(symbol_table[symbol_count].address, "0x%08x", macro_address);
                                        // Add your actions here
                                        // Don't forget to free the allocated memory when done
                                        free(substring);
                                    }
                                }
                            }
                        }
                    }
                /// READ_INT///
                
                
                    
                    
                //printf("Symbol: %s, Address: %s\n", symbol_table[symbol_count].symbol, symbol_table[symbol_count].address); print Symbol 
                symbol_count++;
                
                printf("Address 0x%08x\n", macro_address); // Print address for MIPS instruction line
                macro_address += 4; // Increment address by 4 for each instruction
                address+=4;
                }

                else if (strstr(line, ":")!=NULL && strstr(line, "main")==NULL) // MEANING THAT IT IS A LABEL
                {
                char *token = strtok(line, ":");
                
                    
                    strcpy(symbol_table[symbol_count].symbol, token);
                    sprintf(symbol_table[symbol_count].address, "0x%08x", address);
                    
                    //printf("Symbol: %s, Address: %s\n", symbol_table[symbol_count].symbol, symbol_table[symbol_count].address); print Symbol 
                    symbol_count++;
                
                printf("Address 0x%08x\n", address); // Print address for MIPS instruction line
                address += 4; // Increment address by 4 for each instruction
                }
                else if (strstr(line, "exit")!= NULL)
                {
                    break;
                }
                else{
                     printf("Address 0x%08x\n", address); // Print address for MIPS instruction line
                address += 4; // Increment address by 4 for each instruction
                }
            }
        } 

       
        
        else {
            fprintf(stderr, "Error reading line %d\n", i + 1);
            fclose(file);
            fclose(symbol_file);
            return 1;
        }
    }

// Output the symbol table to symboltable.txt, skipping duplicates
for (int i = 0; i < symbol_count; i++) {
    int isDuplicate = 0;
    // Check if the current symbol is a duplicate
    for (int j = i + 1; j < symbol_count; j++) {
        if (strcmp(symbol_table[i].symbol, symbol_table[j].symbol) == 0) {
            isDuplicate = 1;
            break;
        }
    }
    // Print the symbol only if it's not a duplicate
    if (!isDuplicate) {
        fprintf(symbol_file, "%s\t%s\n", symbol_table[i].symbol, symbol_table[i].address);
    }
}


    printf("Symbol table generated and saved as symboltable.txt\n");

    // Close files
    fclose(symbol_file);
    fclose(file);

    return 0;
}
