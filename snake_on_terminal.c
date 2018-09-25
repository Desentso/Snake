#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

typedef struct {
    int x;
    int y;
    int direction;
} snake_part;

typedef struct {
    int x;
    int y;
    int direction;
    int length;
    snake_part parts[];
} snake;

typedef struct {
    int x;
    int y;
} apple;


// Constants
static int UP = 0;
static int DOWN = 1;
static int RIGHT = 2;
static int LEFT = 3;
static int GAME_AREA_WIDTH = 30;
static int GAME_AREA_HEIGHT = 15;

// Global structs, "game objects"
static snake the_snake;
static apple apples[30*15];
static int apples_index = 0;

void print_area(int width, int height);
void clear_screen();
int has_snake(int x, int y);
int has_apple(int x, int y);
apple generate_apple();
void move_snake();
int has_snake_collided();
DWORD WINAPI ListenForInput(void* data);


int main() {

    the_snake = (snake) { .x=15, .y=7, .direction=UP, .length=0};

    // Create thread on Windows https://docs.microsoft.com/windows/desktop/ProcThread/creating-threads
    HANDLE thread = CreateThread(NULL, 0, ListenForInput, NULL, 0, NULL);
    
    apples[apples_index] = generate_apple();
    apples_index++;

    int i = 0;
    for (;;) {
        print_area(GAME_AREA_WIDTH, GAME_AREA_HEIGHT);
        Sleep(100);
        move_snake();

        if (has_snake_collided() == TRUE) {
            printf("You lost the game");
            break;
        }

        if (i > 100) {
            break;
        }
        clear_screen();
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
            } else if (has_snake(i, k) == TRUE) {
                printf("~");
            } else if (has_apple(i, k) == TRUE) {
                printf("O");
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

void move_snake() {
    if (the_snake.direction == UP) {
        the_snake.y -= 1;
    } else if (the_snake.direction == DOWN) {
        the_snake.y += 1;
    } else if (the_snake.direction == RIGHT) {
        the_snake.x += 1;
    } else if (the_snake.direction == LEFT) {
        the_snake.x -= 1;
    }

}

int has_snake_collided() {
    if (the_snake.x < 0 || the_snake.x > GAME_AREA_WIDTH || the_snake.y < 0 || the_snake.y > GAME_AREA_HEIGHT) {
        return 1;
    }

    return 0;
}

int has_snake(int x, int y) {
    for (int i = 0; i < the_snake.length; i++){
        if (the_snake.parts[i].x == x && the_snake.parts[i].y == y) {
            return 1;
        }
    }
    return 0;
}

int has_apple(int x, int y) {
    for (int i = 0; i < apples_index; i++){
        if (apples[i].x == x && apples[i].y == y) {
            return 1;
        }
    }
    return 0;
}

apple generate_apple() {
    int x = rand() % GAME_AREA_WIDTH;
    int y = rand() % GAME_AREA_HEIGHT;

    while (has_snake(x, y)) {
        x = rand();
        y = rand();
    }

    return (apple) {.x = x, .y = y};
}

DWORD WINAPI ListenForInput(void* data) {
    //printf("Thread func\n");
    for (;;) {
        // Get char without pressing enter
        char c = _getch();
        switch(c) {
            case 'w':
                the_snake.direction = UP;
                break;
            case 's':
                the_snake.direction = DOWN;
                break;
            case 'd':
                the_snake.direction = RIGHT;
                break;
            case 'a':
                the_snake.direction = LEFT;
                break;
            case 'p':
                return 1;
                break;
        }
        //printf("%c\n", c);
    }
    return 0;
}
