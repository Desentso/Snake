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
    snake_part parts[50];
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
int snake_collided_with_apple();
void grow_snake();
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
        Sleep(200);
        
        if (i % 1 == 0) {
            move_snake();
        }

        if (has_snake_collided() == TRUE) {
            printf("You lost the game");
            break;
        }

        if (snake_collided_with_apple() == TRUE){
            printf("collided with apple");
            grow_snake();
        }

        if (i > 10000) {
            break;
        }
        clear_screen();

        i++;
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
            } else if (has_snake(k, i) == TRUE) {
                printf("~");
            } else if (has_apple(k, i) == TRUE) {
                printf("A");
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

    for (int i = 0; i < the_snake.length; i++) {
        int direction = the_snake.parts[i-1].direction;
        if (i == 0) {
            direction = the_snake.direction;
        }

        if (direction == UP) {
            the_snake.parts[i].y -= 1;
        } else if (direction == DOWN) {
            the_snake.parts[i].y += 1;
        } else if (direction == RIGHT) {
            the_snake.parts[i].x += 1;
        } else if (direction == LEFT) {
            the_snake.parts[i].x -= 1;
        }
    }

}

int has_snake_collided() {
    if (the_snake.x < 0 || the_snake.x > GAME_AREA_WIDTH || the_snake.y < 0 || the_snake.y > GAME_AREA_HEIGHT) {
        return 1;
    }

    return 0;
}

int snake_collided_with_apple() {
    for (int i = 0; i < apples_index; i++){
        printf("apple: %d %d snake: %d %d", apples[i].x, apples[i].y, the_snake.x, the_snake.y);
        if (apples[i].x == the_snake.x && apples[i].y == the_snake.y) {
            return 1;
        }
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
        x = rand() % GAME_AREA_WIDTH;
        y = rand() % GAME_AREA_HEIGHT;
    }

    return (apple) {.x = x, .y = y};
}

void grow_snake() {
    int x;
    int y;

    snake_part last_part = the_snake.parts[the_snake.length-1];
    if (the_snake.length == 0) {
        if (the_snake.direction == UP){
            x = the_snake.x;
            y = the_snake.y + 1;
        } else if (the_snake.direction == DOWN){
            x = the_snake.x;
            y = the_snake.y - 1;
        } else if (the_snake.direction == RIGHT){
            x = the_snake.x - 1;
            y = the_snake.y;
        } else if (the_snake.direction == LEFT){
            x = the_snake.x + 1;
            y = the_snake.y;
        }
    } else {
        if (last_part.direction == UP){
            x = last_part.x;
            y = last_part.y + 1;
        } else if (last_part.direction == DOWN){
            x = last_part.x;
            y = last_part.y - 1;
        } else if (last_part.direction == RIGHT){
            x = last_part.x - 1;
            y = last_part.y;
        } else if (last_part.direction == LEFT){
            x = last_part.x + 1;
            y = last_part.y;
        }
    }

    the_snake.parts[the_snake.length] = (snake_part) {.x = x, .y = y, .direction = last_part.direction};
    the_snake.length += 1;
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
