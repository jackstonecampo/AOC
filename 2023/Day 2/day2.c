// advent of code day 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    MAX_RED = 12,
    MAX_GREEN = 13,
    MAX_BLUE = 14
} ColourMaxValue;

typedef enum {
    POSSIBLE = 1,
    IMPOSSIBLE = 0,
} IsPossible;

typedef enum {
    END_GAME_ID = ':',
    END_SUBSET = ';',
    END_CUBE = ','
} SectionTerminator;

typedef struct {
    char buf[10];
    int index;
    char secondary[2];
    int index2;
} Buffer;


Buffer reset_buffer() {
    char buf;
    int i = 0;
    Buffer new = {buf, i, buf, i};
    return new;
}

ColourMaxValue get_colour_max(char* str) {
    char* colours[] = {"red", "green", "blue"};
    for (int i = 0; i < 3; i++) {
        char* colour = colours[i];
        if (strstr(str, colour) != NULL) {
            switch (colour[0]) {
                case 'r': return MAX_RED;
                case 'g': return MAX_GREEN;
                case 'b': return MAX_BLUE;
            }
        
        }
    }

    return 0;
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* filename = argv[1];
    FILE* file = fopen(filename, "r");

    char c;
    Buffer buffer;

    int game_id;

    int sum_valid_game_ids = 0;

    IsPossible possible = POSSIBLE;
    
    while ((c = fgetc(file)) != EOF) {

        if (isdigit(c)) {
            buffer.secondary[buffer.index2] = c;
            buffer.index2++;
        }
        
        if (c == END_GAME_ID) {
            game_id = atoi(buffer.secondary);
            buffer = reset_buffer();
            continue;
        }

        if (c == END_SUBSET || c == END_CUBE) {
            int colour_max = get_colour_max(buffer.buf);
            if (atoi(buffer.secondary) > colour_max) {
                possible = IMPOSSIBLE;
            }
            buffer = reset_buffer();
            continue;
        }

        if (c == '\n') {
            int colour_max = get_colour_max(buffer.buf);
            if (atoi(buffer.secondary) > colour_max) {
                possible = IMPOSSIBLE;
            }

            printf("GAME %d possible: %d\n", game_id, possible);

            if (possible) {
                sum_valid_game_ids += game_id;
            }

            possible = POSSIBLE;
            buffer = reset_buffer();
            continue;
        }

        buffer.buf[buffer.index] = c;

        buffer.index++;
    }

    printf("total: %d\n", sum_valid_game_ids);
}
