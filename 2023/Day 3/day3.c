#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

enum {
    NUMBER = 1,
    SYMBOL = 0,
};

int main(int argc, char *argv[])
{

    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>", argv[0]);
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("failed to open file");
        return EXIT_FAILURE;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *schematic = malloc(file_size + 1);

    size_t bytes_read = fread(schematic, 1, file_size, file);
    if (bytes_read != file_size) {
        perror("error reading file");
        free(schematic);
        fclose(file);
        return errno;
    }
    schematic[file_size] = '\0';
    fclose(file);

    // get line length
    int line_length;
    for (int i = 0; schematic[i] != '\n'; i++) {
        line_length = i + 2;
    }

    int windices[9];
    int windex = 0;
    // generate relative indices for window
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            windices[windex] = i * line_length + j;
            windex++;
        }
    }

    int previous = SYMBOL;
    char *num_buffer = malloc(3);
    int bufindex = 0;
    int symbol_adjacent;

    int total = 0;

    for (int i = 0; schematic[i] != '\0'; i++) {
        char c = schematic[i];
        if (isdigit(c)) {
            num_buffer[bufindex] = c;
            bufindex++;
            previous = NUMBER;

            // build a window
            if (!symbol_adjacent) {
                for (int w = 0; w < 9; w++) {
                    char wc = schematic[i + windices[w]];
                    if (ispunct(wc) && wc != '.') {
                        symbol_adjacent = 1;
                        break;
                    }

                    symbol_adjacent = 0;
                }
            }
        } else {
            num_buffer[bufindex] = '\0';
            if (previous == NUMBER) {
                int number = atoi(num_buffer);
                if (symbol_adjacent) {
                    // printf("%d is symbol adjacent.\n", number);
                    total += number;
                }
                bufindex = 0;
                symbol_adjacent = 0;
            }

            previous = SYMBOL;
        }
    }
    free(schematic);
    free(num_buffer);
    printf("total: %d\n", total);
}