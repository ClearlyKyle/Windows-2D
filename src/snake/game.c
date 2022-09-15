#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../engine/input.h"

/* Snake size will be a 5x5px rectangle */

#define MAX_SNAKE_SIZE 100
#define SNAKE_SQAURE_SIZE 20
#define NUMBER_OF_SQAURES (400 / SNAKE_SQAURE_SIZE) - 1

typedef struct vec2
{
    int x, y;
} vec2;

typedef struct Snake
{
    vec2         tail[MAX_SNAKE_SIZE];
    unsigned int size;
    char         direction;
    vec2         fruit;
    int          score;
    int          state;
} Snake;

Snake snake = {0};

static int random_int(int min, int max)
{
    return min + rand() % (max + 1 - min);
}

static void snake_add_tail(const vec2 position)
{
    snake.tail[snake.size++] = position;
}

static bool snake_self_collision(void)
{
    for (unsigned int i = 1; i < snake.size; i++)
    {
        if (snake.tail[0].x == snake.tail[i].x && snake.tail[0].y == snake.tail[i].y)
            return true;
    }
    return false;
}

static void snake_move(void)
{
    for (unsigned int i = snake.size - 1; i > 0; i--)
    {
        snake.tail[i] = snake.tail[i - 1];
    }

    const unsigned int width  = Window_Width();
    const unsigned int height = Window_Height();

    switch (snake.direction)
    {
    case 'w':
        snake.tail[0].y -= SNAKE_SQAURE_SIZE;
        if (snake.tail[0].y < 0)
            snake.tail[0].y = height - SNAKE_SQAURE_SIZE;
        break;
    case 's':
        snake.tail[0].y += SNAKE_SQAURE_SIZE;
        if (snake.tail[0].y >= (int)height)
            snake.tail[0].y = 0;
        break;
    case 'a':
        snake.tail[0].x -= SNAKE_SQAURE_SIZE;
        if (snake.tail[0].x < 0)
            snake.tail[0].x = width - SNAKE_SQAURE_SIZE;
        break;
    case 'd':
        snake.tail[0].x += SNAKE_SQAURE_SIZE;
        if (snake.tail[0].x >= (int)width)
            snake.tail[0].x = 0;
        break;
    }

    // drawing snake
    for (unsigned int i = 0; i < snake.size; i++)
    {
        vec2 position = snake.tail[i];

        Window_Draw_Rectangle(position.x, position.y, SNAKE_SQAURE_SIZE, SNAKE_SQAURE_SIZE, 0xFF0000);
    }

    // draw fruit
    Window_Draw_Rectangle(snake.fruit.x, snake.fruit.y, SNAKE_SQAURE_SIZE, SNAKE_SQAURE_SIZE, 0xE4D400);

    if (snake_self_collision())
    {
        snake.state = 0; // Restart the game
    }
}

static void snake_update_score(void)
{
    snake.score++;

    char buff[32];
    sprintf_s(buff, 32, "Score : %d\0", snake.score);

    Window_Set_Title(buff);
}

void Game_init(void)
{
    srand((unsigned)time(NULL));

    snake.size      = 1;
    snake.score     = 0;
    snake.state     = 1;
    snake.tail[0].x = random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    snake.tail[0].y = random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    snake.direction = 'a';

    // screen is 400px wide, so 400/SNAKE_SQAURE_SIZE = num of positions
    const int fruit_x = random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    const int fruit_y = random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    snake.fruit       = (vec2){fruit_x, fruit_y};

    snake_update_score();
}

void Game_update(const double elapsed_time)
{
    if (input_key_is_down(KEY_W))
        snake.direction = (snake.direction != 's') ? 'w' : 's'; // UP

    if (input_key_is_down(KEY_S))
        snake.direction = (snake.direction != 'w') ? 's' : 'w'; // DOWN

    if (input_key_is_down(KEY_A))
        snake.direction = (snake.direction != 'd') ? 'a' : 'd'; // LEFT

    if (input_key_is_down(KEY_D))
        snake.direction = (snake.direction != 'a') ? 'd' : 'a'; // RIGHT

    // collision with food
    if (snake.fruit.x == snake.tail[0].x && snake.fruit.y == snake.tail[0].y)
    {
        snake_add_tail(snake.fruit);
        snake_update_score();

        const int fruit_x = random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
        const int fruit_y = random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
        snake.fruit       = (vec2){fruit_x, fruit_y};
    }
}

void Game_on_render(void)
{
    if (snake.state == 0) // Restart
    {
        Game_init();
    }
    else // Move the snake along
    {
        snake_move();
    }
}

void Game_on_exit(void)
{
    return;
}