#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "usage: %s <filename>\n", argv[0]);
    }

    char* filename = argv[1];
    FILE* file = fopen(filename, "r");    

    if (file == NULL) {
        perror("failed to open file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* str = malloc(file_size + 1);


    size_t bytes_read = fread(str, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("error reading file");
        free(str);
        fclose(file);
        return errno;
    }
    str[file_size] = '\0';

    // note: digits are checked in reverse ∵ otherwise "twone" would find "one" before "two"
    char* digits[] = {"nine", "eight", "seven", "six", "five", "four", "three", "two", "one", "zero"};

    char window[5];

    int first_digit = ' ';
    int last_digit = ' ';
    long total = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        
        if (str[i] == '\n') {
            int calibration_value = 10 * first_digit + last_digit;
            total += calibration_value;
            first_digit = ' ';
            last_digit = ' ';
        }
        
        // create a sliding window of size 5 ∵ longest number "seven" is 5 characters
        for (int w = 0; w < 5; w++) {
            if (str[i + w] == '\n') {
                window[w] = '\0';
                break;
            }
            window[w] = str[i + w];
        }

        // check if digit at window[0]
        if (isdigit(str[i])) {
            last_digit = str[i] - '0';
            if (first_digit == ' ') {
                first_digit = str[i] - '0';
            }
        }

        // check for a digit match within the window, e.g. "one", "two"
        for (int d = 0; d < 10; d++) {
            char* digit = digits[d];
            char* digit_found = strstr(window, digit);
            if (digit_found != NULL) {
                int pos = digit_found - window;

                // found match in window, now check for preceding digits
                for (int iw = 0; iw < pos; iw++) {
                    if (isdigit(window[iw])) {
                        last_digit = window[iw] - '0';
                        if (first_digit == ' ') {
                            first_digit = window[iw] - '0';
                        }
                    }
                }

                // see declaration of array "digits" (line 38)
                last_digit = 9 - d;
                if (first_digit == ' ') {
                    first_digit = 9 - d;
                }                
            }
        }
    }

    printf("TOTAL: %lu", total);

    free(str);
    fclose(file);
    
    return EXIT_SUCCESS;
}
