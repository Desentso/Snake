#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

void print_area(int width, int height);
void clear_screen();

typedef struct {
    int x;
    int y;
    int length;
} snake;

int main() {
    int GAME_AREA_HEIGHT = 15;
    int GAME_AREA_WIDTH = 30;
    snake the_snake;
    the_snake = (snake) { .x = 0, .y = 0, .length = 2};

    HANDLE thread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

    for (;;) {
        print_area(GAME_AREA_WIDTH, GAME_AREA_HEIGHT);
        Sleep(500);
        clear_screen();
    }
    //print_area(GAME_AREA_WIDTH, GAME_AREA_HEIGHT);
    //clear_screen();
    //print_area(GAME_AREA_WIDTH, GAME_AREA_HEIGHT);
    return 0;
}

void print_area(int width, int height) {
    for (int i = 0; i < width; i++){
        printf("#");
    }
    printf("\n");
    for (int i = 0; i < height-2; i++){
        for (int k = 0; k < width; k++){
            if (k == 0 || k == width-1) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < width; i++){
        printf("#");
    }
}

void clear_screen() {
    for (int i = 0; i < 100; i++){
        printf("\n");
    }
}
