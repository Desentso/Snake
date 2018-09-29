#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>

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
    snake_part parts[30*15];
} snake;

typedef struct {
    int x;
    int y;
} apple;


// Constants
static int UP = 1;
static int DOWN = 2;
static int RIGHT = 3;
static int LEFT = 4;
static int GAME_AREA_WIDTH = 30;
static int GAME_AREA_HEIGHT = 13;

// Global structs, "game objects"
static snake the_snake;
static apple apples[30*15];
static int apples_index = 0;

void draw_game_area_and_objects(int width, int height);
void clear_screen(void);
int has_snake(int x, int y);
int has_apple(int x, int y);
apple generate_apple(void);
void move_snake(void);
int has_snake_collided();
int snake_collided_with_apple(void);
void grow_snake(void);
void eat_apple(int x, int y);
void print_snake(int direction);
DWORD WINAPI ListenForInput(void* data);


int main() {
    // Seed random function with current time
    srand(time(NULL));

    the_snake = (snake) { .x=15, .y=7, .direction=UP, .length=0};

    // Create thread on Windows https://docs.microsoft.com/windows/desktop/ProcThread/creating-threads
    HANDLE thread = CreateThread(NULL, 0, ListenForInput, NULL, 0, NULL);
    
    apples[apples_index] = generate_apple();
    apples_index++;

    for (;;) {
        draw_game_area_and_objects(GAME_AREA_WIDTH, GAME_AREA_HEIGHT);
        Sleep(100);
        
        move_snake();

        if (has_snake_collided() == TRUE) {
            printf("\n\nYou lost the game\n");
            break;
        }

        if (snake_collided_with_apple() == TRUE){
            grow_snake();
        }

        clear_screen();

    }

    return 0;
}

void draw_game_area_and_objects(int width, int height) {
    for (int i = 0; i < width; i++){
        printf("#");
    }

    printf("\n");

    for (int i = 0; i < height; i++){
        for (int k = 0; k < width; k++){
            int snake_part_direction = has_snake(k, i);

            if (k == 0 || k == width-1) {
                printf("#");
            } else if (k == the_snake.x && i == the_snake.y) {
                print_snake(the_snake.direction);
            } else if (snake_part_direction != FALSE) {
                print_snake(snake_part_direction);
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

void print_snake(int direction) {
    if (direction == UP) {
        printf("^");
    } else if (direction == DOWN) {
        printf("v");
    } else if (direction == LEFT) {
        printf("<");
    } else if (direction == RIGHT) {
        printf(">");
    }
}

void clear_screen() {
    system("@cls");
}

void move_snake() {
    int prev_x = the_snake.x;
    int prev_y = the_snake.y;
    int prev_direction = the_snake.direction;

    if (the_snake.direction == UP) {
        the_snake.y -= 1;
    } else if (the_snake.direction == DOWN) {
        the_snake.y += 1;
    } else if (the_snake.direction == RIGHT) {
        the_snake.x += 1;
    } else if (the_snake.direction == LEFT) {
        the_snake.x -= 1;
    }

    snake_part snake_parts_store[30*15];

    for (int i = 0; i < the_snake.length; i++) {
        if (i == 0) {
            snake_parts_store[i] = the_snake.parts[i];
            the_snake.parts[i].x = prev_x;
            the_snake.parts[i].y = prev_y;
            the_snake.parts[i].direction = prev_direction;
        } else {
            snake_parts_store[i] = the_snake.parts[i];
            the_snake.parts[i].x = snake_parts_store[i-1].x;
            the_snake.parts[i].y = snake_parts_store[i-1].y;
            the_snake.parts[i].direction = snake_parts_store[i-1].direction;
        }
    }

}

int has_snake_collided() {
    if (the_snake.x <= 0 || the_snake.x >= GAME_AREA_WIDTH-1 || the_snake.y < 0 || the_snake.y >= GAME_AREA_HEIGHT) {
        return 1;
    }

    for (int i = 0; i < the_snake.length; i++){
        if (the_snake.x == the_snake.parts[i].x && the_snake.y == the_snake.parts[i].y) {
            return 1;
        }
    }

    return 0;
}

int snake_collided_with_apple() {
    for (int i = 0; i < apples_index; i++){
        if (apples[i].x == the_snake.x && apples[i].y == the_snake.y) {
            eat_apple(apples[i].x, apples[i].y);
            apples[apples_index] = generate_apple();
            apples_index++;
            return 1;
        }
    }
    return 0;
}

void eat_apple(int x, int y) {
    for (int i = 0; i < apples_index; i++){
        if (apples[i].x == x && apples[i].y == y) {
            for (int l = i+1; l < apples_index; l++){
                apples[l] = apples[l+1];
            }
            apples_index -= 1;
            break;
        }
    }
} 

int has_snake(int x, int y) {
    for (int i = 0; i < the_snake.length; i++){
        if (the_snake.parts[i].x == x && the_snake.parts[i].y == y) {
            return the_snake.parts[i].direction;
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
    int x = rand() % (GAME_AREA_WIDTH-2) + 1;
    int y = rand() % (GAME_AREA_HEIGHT-2) + 1;

    while (has_snake(x, y) != FALSE) {
        x = rand() % (GAME_AREA_WIDTH-2) + 1;
        y = rand() % (GAME_AREA_HEIGHT-2) + 1;
    }

    return (apple) {.x = x, .y = y};
}

void grow_snake() {
    int x;
    int y;
    int direction;

    snake_part last_part = the_snake.parts[the_snake.length-1];

    if (the_snake.length == 0) {
        direction = the_snake.direction;
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
        direction = last_part.direction;
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

    the_snake.parts[the_snake.length] = (snake_part) {.x = x, .y = y, .direction = direction};
    the_snake.length += 1;
}

DWORD WINAPI ListenForInput(void* data) {
    for (;;) {
        // Get char without pressing enter
        char c = _getch();
        switch(c) {
            case 'w':
                if (the_snake.direction != DOWN) {
                    the_snake.direction = UP;
                }
                break;
            case 's':
                if (the_snake.direction != UP) {
                    the_snake.direction = DOWN;
                }
                break;
            case 'd':
                if (the_snake.direction != LEFT) {
                    the_snake.direction = RIGHT;
                }
                break;
            case 'a':
                if (the_snake.direction != RIGHT) {
                    the_snake.direction = LEFT;
                }
                break;
            // You can exit the game by pressing p
            case 'p':
                return 1;
                break;
        }
    }
    return 0;
}
