#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

typedef struct {
    int x;
    int y;
    int direction;
    int length;
    snake_part[] parts;
} snake;

typedef struct {
    int x;
    int y;
    int direction;
} snake_part;

static int UP = 0;
static int DOWN = 1;
static int RIGHT = 2;
static int LEFT = 3;

void print_area(int width, int height);
void clear_screen();
void has_snake(int x, int y);
DWORD WINAPI ListenForInput(void* data);


int main() {
    int GAME_AREA_WIDTH = 30;
    int GAME_AREA_HEIGHT = 15;
    static snake the_snake;
    the_snake = (snake) { .x=15, .y=7, .direction=UP, .length=0, .parts=[]};

    // Create thread on Windows https://docs.microsoft.com/windows/desktop/ProcThread/creating-threads
    HANDLE thread = CreateThread(NULL, 0, ListenForInput, NULL, 0, NULL);
    
    int i = 0;
    for (;;) {
        print_area(GAME_AREA_WIDTH, GAME_AREA_HEIGHT);
        Sleep(100);
        clear_screen();

        if (i > 100) {
            break;
        }
    }

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
            } else if (k == the_snake.x && i == the_snake.y) {
                printf("S");
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
    system("@cls");
    // "Clear" screen by entering 100 line breaks
    /*for (int i = 0; i < 100; i++){
        printf("\n");
    }*/
}

int has_snake(int x, int y) {
    
}

DWORD WINAPI ListenForInput(void* data) {
    printf("Thread func\n");
    for (;;) {
        // Get char without pressing enter
        char c = _getch();
        switch() {
            case 'w':
                the_snake
            case 's':
                the_snake
            case 'd':
                the_snake
            case 'a':
                the_snake
        }
        printf("%c\n", c);
    }
    return 0;
}
