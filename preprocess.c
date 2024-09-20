#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_DEFS 100
#define MAX_VAR_LEN 100
#define MAX_LINE_LEN 10000  // Increase buffer size for large replacements

// Struct to hold variable-definition pairs
typedef struct {
    char var[MAX_VAR_LEN];
    char val[MAX_VAR_LEN];
} Definition;

Definition defs[MAX_DEFS];
int def_count = 0;

// Function to add a definition
void add_definition(char *arg) {
    if (strncmp(arg, "-D", 2) == 0) {
        char *equal_sign = strchr(arg, '=');
        if (equal_sign != NULL) {
            *equal_sign = '\0';  // Split var and val
            strcpy(defs[def_count].var, arg + 2);
            strcpy(defs[def_count].val, equal_sign + 1);
        } else {
            strcpy(defs[def_count].var, arg + 2);
            strcpy(defs[def_count].val, "");  // No value, empty string
        }
        def_count++;
    }
}

// Check if a character is part of a valid identifier (alphanumeric or underscore)
int is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

// Function to replace variables in the line, but only full words
void substitute_line(char *line) {
    char buffer[MAX_LINE_LEN];  // Temporary buffer for storing the substituted line
    buffer[0] = '\0';  // Initialize buffer

    int pos = 0;  // Current position in the line being processed

    while (line[pos] != '\0') {
        int replaced = 0;

        // Check each defined variable for a match
        for (int i = 0; i < def_count; i++) {
            int var_len = strlen(defs[i].var);

            // Check if the variable matches and is a full word (not part of another word)
            if (strncmp(&line[pos], defs[i].var, var_len) == 0 &&
                (pos == 0 || !is_identifier_char(line[pos - 1])) &&  // Ensure left boundary
                !is_identifier_char(line[pos + var_len])) {  // Ensure right boundary

                strcat(buffer, defs[i].val);  // Add the replacement value to the buffer
                pos += var_len;  // Move position past the variable
                replaced = 1;
                break;
            }
        }

        // If no replacement was made, copy the current character
        if (!replaced) {
            int len = strlen(buffer);
            buffer[len] = line[pos];
            buffer[len + 1] = '\0';
            pos++;
        }
    }

    // Copy the substituted line back into the original line buffer
    strcpy(line, buffer);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file> -D<var>=<val>...\n", argv[0]);
        return 1;
    }

    // Parse command-line definitions
    for (int i = 2; i < argc; i++) {
        add_definition(argv[i]);
    }

    // Open the input file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // Process the file line by line
    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), file)) {
        substitute_line(line);
        printf("%s", line);  // Print the substituted line
    }

    fclose(file);
    return 0;
}
