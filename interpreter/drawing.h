#include <stdlib.h>
#include <stdio.h>

void drawPixel(int x, int y, int color) {
    // Move the cursor to the specified position on the console
    printf("\033[%d;%dH", y + 1, x + 1);

    // Print the appropriate character for the color
    if (color == 0) {
        printf(" ");
    } else if (color == 1) {
        printf("#");
    }
}
