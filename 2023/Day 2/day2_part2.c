// advent of code day 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

char get_colour(char* str) {
    char* colours[] = {"red", "green", "blue"};
    for (int i = 0; i < 3; i++) {
        char* colour = colours[i];
        if (strstr(str, colour) != NULL) {
            return colour[0];       
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

    int min_red = 0;
    int min_green = 0;
    int min_blue = 0;
    int num_cubes;

    int total_power = 0;
    
    while ((c = fgetc(file)) != EOF) {

        if (isdigit(c)) {
            buffer.secondary[buffer.index2] = c;
            buffer.index2++;
        }
        
        if (c == END_GAME_ID) {
            buffer = reset_buffer();
            continue;
        }

        if (c == END_SUBSET || c == END_CUBE) {
            num_cubes = atoi(buffer.secondary);
            switch (get_colour(buffer.buf)) {
                case 'r': min_red = (num_cubes > min_red) ? num_cubes : min_red; break;
                case 'g': min_green = (num_cubes > min_green) ? num_cubes : min_green; break;
                case 'b': min_blue = (num_cubes > min_blue) ? num_cubes : min_blue; break;
            }
            buffer = reset_buffer();
            continue;
        }

        if (c == '\n') {
            num_cubes = atoi(buffer.secondary);
            switch (get_colour(buffer.buf)) {
                case 'r': min_red = (num_cubes > min_red) ? num_cubes : min_red; break;
                case 'g': min_green = (num_cubes > min_green) ? num_cubes : min_green; break;
                case 'b': min_blue = (num_cubes > min_blue) ? num_cubes : min_blue; break;
            }

            int power = min_red * min_green * min_blue;
            total_power += power;
            min_red = 0;
            min_green = 0;
            min_blue = 0;
            buffer = reset_buffer();
            continue;
        }

        buffer.buf[buffer.index] = c;

        buffer.index++;
    }

    printf("total: %d\n", total_power);
}
