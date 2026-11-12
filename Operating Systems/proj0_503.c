#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print the first max_lines of a file
int print_file_lines(const char *filename, int max_lines, int *last_char_was_newline) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "cannot open file\n");
        return 1;
    }

    char line[1024];
    int line_count = 0;
    int printed_anything = 0;

    while (fgets(line, sizeof(line), file) && line_count < max_lines) {
        // If this is the first line of a new file and the previous file didn't end with a newline, print one.
        if (!printed_anything && !(*last_char_was_newline)) {
            printf("\n");
        }

        printf("%s", line); // Print the line exactly as read
        printed_anything = 1;
        line_count++;

        // Check if the last character of the printed line is a newline
        *last_char_was_newline = (line[strlen(line) - 1] == '\n');
    }

    fclose(file);

    // If the file was read and did not end with a newline, force a new line
    if (printed_anything && !(*last_char_was_newline)) {
        printf("\n");
        *last_char_was_newline = 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 0;
    }

    if ((argc - 1) % 2 != 0) {
        fprintf(stderr, "invalid or missing max_lines parameter\n");
        return 1;
    }

    int return_code = 0;
    int last_char_was_newline = 1; // Assume we start on a new line

    for (int i = 1; i < argc; i += 2) {
        char *filename = argv[i];
        int max_lines;

        // Parse max_lines argument
        if (sscanf(argv[i + 1], "%d", &max_lines) != 1 || max_lines < 0) {
            fprintf(stderr, "invalid or missing max_lines parameter\n");
            return_code = 1;
            continue;
        }

        // Print file content and update last_char_was_newline
        if (print_file_lines(filename, max_lines, &last_char_was_newline) != 0) {
            return_code = 1;
        }
        
        // Ensure there's a blank line between outputs of different files
        if (i + 2 < argc && last_char_was_newline) {
            printf("\n");
            last_char_was_newline = 1; // Reset to ensure proper formatting
        }
    }

    // Ensure shell starts on a new line
    if (!last_char_was_newline) {
        printf("\n");
    }

    return return_code;
}
