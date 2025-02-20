#include <stdio.h>          // Include standard input/output library (for functions like printf, fopen, etc.)
#include <stdlib.h>         // Include standard library (for functions like malloc, free, atoi, etc.)
#include <string.h>         // Include string manipulation library (for functions like strcpy, strcmp, strlen, etc.)
#include <ctype.h>          // Include character type checking library (for functions like isalpha, isdigit, isspace, etc.)
#define MAX_TOKENS 100      // Define maximum number of tokens that can be stored
#define MAX_ID_LEN 50       // Define maximum length of an identifier (variable name)
typedef struct {              // Structure to store symbol table entries. A symbol table stores information about identifiers.
    int index;              // Index of the symbol in the table (a unique identifier for each symbol)
    char name[MAX_ID_LEN]; // Name of the identifier (e.g., variable name)
    char type[10];         // Data type of the identifier (e.g., "int", "float", "char")
    int size;              // Size of the identifier in memory (e.g., 4 bytes for int, 1 byte for char)
} Symbol;
Symbol symbolTable[MAX_TOKENS]; // Declare the symbol table as an array of Symbol structures. This array can hold up to MAX_TOKENS symbols.
int symbolCount = 0;          // Initialize the count of symbols currently in the symbol table to 0.
int lookupSymbol(char *name) { // Function to check if an identifier (symbol) with the given name already exists in the symbol table.
    for (int i = 0; i < symbolCount; i++) { // Iterate through the existing symbols in the symbol table.
        if (strcmp(symbolTable[i].name, name) == 0) { // Compare the given name with the name of the current symbol.
            return symbolTable[i].index; // If the names match, return the index of the symbol.
        }
    }
    return -1; // If the given name is not found in the symbol table, return -1.
}
int insertSymbol(char *name, char *type, int size) { // Function to insert a new identifier (symbol) into the symbol table.
    int index = lookupSymbol(name); // Check if the symbol already exists.
    if (index == -1) { // If the symbol doesn't exist.
        symbolTable[symbolCount].index = symbolCount + 1; // Assign a new index to the symbol.
        strcpy(symbolTable[symbolCount].name, name); // Copy the name to the symbol table entry.
        strcpy(symbolTable[symbolCount].type, type); // Copy the type to the symbol table entry.
        symbolTable[symbolCount].size = size; // Set the size of the symbol.
        symbolCount++; // Increment the symbol count.
        return symbolTable[symbolCount - 1].index; // Return the index of the newly inserted symbol.
    }
    return index; // If the symbol already exists, return its existing index.
}
// Function to check if a given string is a keyword.
int isKeyword(const char *str) {
    char *keywords[] = { // Array of keywords.
        "if", "else", "while", "for", "do", "int", "float", "char", "return", "void"
    };
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]); // Calculate the number of keywords.
    for (int i = 0; i < numKeywords; i++) { // Iterate through the keywords.
        if (strcmp(str, keywords[i]) == 0) { // Compare the given string with each keyword.
            return 1; // If it's a keyword, return 1 (true).
        }
    }
    return 0; // If it's not a keyword, return 0 (false).
}
// Function to get the next token from the input file.
void getNextToken(FILE *fp) {
    char ch, buffer[MAX_ID_LEN]; // Character 'ch' to store the current character read, 'buffer' to store the current token being built.
    int line = 1, col = 0, tokenIndex = 1; // 'line' and 'col' to track the line and column number, 'tokenIndex' to assign a unique index to each token.
    while ((ch = fgetc(fp)) != EOF) { // Read characters from the file one by one until the end of file (EOF) is reached.
        col++; // Increment the column number.
        // Handle preprocessor directives (e.g., #include <...>)
        if(ch == '#') { // Check if it is a preprocessor directive
            int i = 0;
            buffer[i++] = ch; // store the hash symbol in buffer
            while ((ch = fgetc(fp)) != '\n' && ch != EOF) { // read until newline or EOF
                buffer[i++] = ch; // store the characters in buffer
            }
            buffer[i] = '\0'; // null terminate the buffer
            printf("%d. < Preprocessor , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // print the token information
            if(ch == '\n'){ // check if the character is newline
                line++; // increment line number
                col = 0; // reset column number
            }
            continue; // continue to the next character
        }
        if (isspace(ch)) { // Check if the character is a whitespace character (space, tab, newline, etc.).
            if (ch == '\n') { // If it's a newline character.
                line++; // Increment the line number.
                col = 0; // Reset the column number.
            }
            continue; // Skip whitespace characters and proceed to the next character.
        }
        // Handle character literals (e.g., 'x')
        if(ch == '\'') { // Check if it is a character literal
            int i = 0;
            buffer[i++] = ch; // store the opening quote
            while ((ch = fgetc(fp)) != '\'' && ch != EOF) { // read until closing quote or EOF
                buffer[i++] = ch; // store the characters in buffer
            }
            buffer[i++] = '\''; // store the closing quote
            buffer[i] = '\0'; // null terminate the buffer
            printf("%d. < Character , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // print the token information
            continue; // continue to the next character
        }
        // Handle identifiers and keywords.
        if (isalpha(ch) || ch == '_') { // Check if the character is an alphabet or an underscore (start of an identifier).
            int i = 0;
            buffer[i++] = ch; // Store the character in the buffer.
            while (isalnum(ch = fgetc(fp)) || ch == '_') { // Read subsequent characters of the identifier (alphanumeric or underscore).
                buffer[i++] = ch; // Store the character in the buffer.
            }
            buffer[i] = '\0'; // Null-terminate the buffer to make it a valid string.
            ungetc(ch, fp); // Put the last read character back into the input stream (because it's not part of the identifier).
            if (isKeyword(buffer)) { // Check if the identifier is a keyword.
                printf("%d. < Keyword , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as a keyword.
            } else if (strcmp(buffer, "printf") == 0) {
                // Tokenize printf as an identifier but DO NOT add it to the symbol table.
                printf("%d. < Identifier , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as an identifier.
            } else {
                int index = insertSymbol(buffer, "var", 4); // If it's not a keyword, it's a user-defined identifier. Insert it into the symbol table.
                printf("%d. < Identifier , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as an identifier.
            }
        } else if (isdigit(ch)) { // Check if the character is a digit (start of a number).
            int i = 0;
            buffer[i++] = ch; // Store the digit in the buffer.
            while (isdigit(ch = fgetc(fp))) { // Read subsequent digits of the number.
                buffer[i++] = ch; // Store the digit in the buffer.
            }
            buffer[i] = '\0'; // Null-terminate the buffer.
            ungetc(ch, fp); // Put the last read character back into
            printf("%d. < Number , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as a number.
        } else if (ch == '=' || ch == '>' || ch == '<' || ch == '!') { // Check if the character is a relational operator (or part of one).
            buffer[0] = ch; // Store the operator in the buffer.
            ch = fgetc(fp); // Read the next character to check for a two-character operator (e.g., ==, >=, <=, !=).
            if (ch == '=') { // If the next character is '=', it's a two-character operator.
                buffer[1] = ch;
                buffer[2] = '\0'; // Null-terminate the buffer.
            } else { // If not, it's a single-character operator.
                buffer[1] = '\0'; // Null-terminate the buffer.
                ungetc(ch, fp); // Put the last read character back into the input stream.
            }
            printf("%d. < Symbol , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as a symbol (operator).
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') { // Check if the character is an arithmetic operator.
            if (ch == '/' && (ch = fgetc(fp)) == '/') { // Single-line comment
                while ((ch = fgetc(fp)) != '\n' && ch != EOF); // Skip until newline or EOF
                line++; // Increment line number
                col = 0; // Reset column number
                continue; // Continue to the next character
            } else if (ch == '/' && (ch = fgetc(fp)) == '*') { // Multi-line comment
                while ((ch = fgetc(fp)) != EOF) { // Read until EOF
                    if (ch == '*' && (ch = fgetc(fp)) == '/') break; // Check for */ to close comment
                    if (ch == '\n') { // Check for newline within comment
                        line++; // Increment line number
                        col = 0; // Reset column number
                    } else {
                        col++; // Increment column number within comment
                    }
                }
                continue; // Continue to the next character after comment
            } else {
                buffer[0] = ch; // Store the operator in the buffer.
                buffer[1] = '\0'; // Null-terminate the buffer.
                printf("%d. < Operator , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as an operator.
            }
        } else if (ch == '&' || ch == '|') { // Check if the character is a logical operator (or part of one).
            buffer[0] = ch; // Store the operator in the buffer.
            ch = fgetc(fp); // Read the next character to check for a two-character operator (e.g., &&, ||).
            if (ch == buffer[0]) { // If the next character is the same as the current one, it's a two-character operator.
                buffer[1] = ch;
                buffer[2] = '\0'; // Null-terminate the buffer.
            } else { // If not, it's a single-character operator (though these are less common in C for logical operations).
                buffer[1] = '\0'; // Null-terminate the buffer.
                ungetc(ch, fp); // Put the last read character back into the input stream.
            }
            printf("%d. < Logical Operator , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as a logical operator.
        } else if (ch == '"') { // Check if the character is a double quote (start of a string literal).
            int i = 0;
            buffer[i++] = ch; // Store the opening double quote.
            while ((ch = fgetc(fp)) != '"' && ch != EOF) { // Read until the closing double quote or EOF.
                buffer[i++] = ch; // Store the characters of the string literal.
            }
            buffer[i++] = '"'; // Store the closing double quote.
            buffer[i] = '\0'; // Null-terminate the buffer.
            printf("%d. < String , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as a string.
        } else { // If the character is none of the above (e.g., punctuation, parentheses, brackets, etc.).
            buffer[0] = ch; // Store the character in the buffer.
            buffer[1] = '\0'; // Null-terminate the buffer.
            printf("%d. < Symbol , %d , %d, %s>\n", tokenIndex++, line, col, buffer); // Print the token information as a symbol.
        }
    }
}
void printSymbolTable() {// Function to print the symbol table.
    printf("\nSymbol Table:\n");
    printf("Index    Name     Type     Size\n");
    printf("-----    ----     ----     ----\n");
    for (int i = 0; i < symbolCount; i++) { // Iterate through the symbols in the symbol table.
        printf("%d      %s       %s       %d\n", // Print the index, name, type, and size of each symbol.
               symbolTable[i].index,
               symbolTable[i].name,
               symbolTable[i].type,
               symbolTable[i].size);
    }
}
int main(int argc, char *argv[]) { // Main function of the program.
    if (argc < 2) { // Check if a filename is provided as a command-line argument.
        printf("Usage: %s <filename>\n", argv[0]); // Print usage instructions.
        return 1; // Return an error code.
    }
    FILE *fp = fopen(argv[1], "r"); // Open the file in read mode.
    if (fp == NULL) { // Check if the file was opened successfully.
        printf("Error opening file %s\n", argv[1]); // Print an error message.
        return 1; // Return an error code.
    }
    getNextToken(fp); // Call the function to get and print the tokens from the file.
    fclose(fp); // Close the file.
    printSymbolTable(); // Call the function to print the symbol table.
    return 0; // Return 0 to indicate successful execution.
}
/*PS C:\Users\Ayush Sinha\Desktop\test> gcc q1.c -o q1
PS C:\Users\Ayush Sinha\Desktop\test> ./q1 test.c */
