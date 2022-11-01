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

typedef struct Snake
{
    vec2         tail[MAX_SNAKE_SIZE];
    unsigned int size;
    char         direction;
    vec2         fruit;
    int          score;
    int          state;
} Snake;

static Snake snake = {0};

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

static void snake_draw(void)
{
    // draw head
    Window_Draw_Rectangle((int)snake.tail[0].x, (int)snake.tail[0].y, SNAKE_SQAURE_SIZE, SNAKE_SQAURE_SIZE, 0xFF0000);

    // drawing tail
    for (unsigned int i = 1; i < snake.size; i++)
    {
        const vec2 position = snake.tail[i];

        Window_Draw_Rectangle((int)position.x, (int)position.y, SNAKE_SQAURE_SIZE, SNAKE_SQAURE_SIZE, 0xFF5050);
    }

    // draw fruit
    Window_Draw_Rectangle((int)snake.fruit.x, (int)snake.fruit.y, SNAKE_SQAURE_SIZE, SNAKE_SQAURE_SIZE, 0xE4D400);
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
    snake.tail[0].x = (float)random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    snake.tail[0].y = (float)random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    snake.direction = 'a';

    // screen is 400px wide, so 400/SNAKE_SQAURE_SIZE = num of positions
    const float fruit_x = (float)random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    const float fruit_y = (float)random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
    snake.fruit         = (vec2){fruit_x, fruit_y};

    snake_update_score();
}

static size_t tick = 0;

void Game_update(const double elapsed_time)
{
    tick++;

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

        const float fruit_x = (float)random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
        const float fruit_y = (float)random_int(0, NUMBER_OF_SQAURES) * SNAKE_SQAURE_SIZE;
        snake.fruit         = (vec2){fruit_x, fruit_y};
    }

    // MOVE SNAKE
    const unsigned int width  = Window_Width();
    const unsigned int height = Window_Height();

    // move head
    if (tick % 16 == 0)
    {
        // update tail
        for (unsigned int i = snake.size - 1; i > 0; i--)
        {
            snake.tail[i] = snake.tail[i - 1];
        }

        switch (snake.direction)
        {
        case 'w':
            snake.tail[0].y -= SNAKE_SQAURE_SIZE;
            if (snake.tail[0].y < 0)
                snake.tail[0].y = (float)(height - SNAKE_SQAURE_SIZE);
            break;
        case 's':
            snake.tail[0].y += SNAKE_SQAURE_SIZE;
            if (snake.tail[0].y >= (int)height)
                snake.tail[0].y = 0;
            break;
        case 'a':
            snake.tail[0].x -= SNAKE_SQAURE_SIZE;
            if (snake.tail[0].x < 0)
                snake.tail[0].x = (float)(width - SNAKE_SQAURE_SIZE);
            break;
        case 'd':
            snake.tail[0].x += SNAKE_SQAURE_SIZE;
            if (snake.tail[0].x >= (int)width)
                snake.tail[0].x = 0;
            break;
        }

        if (snake_self_collision())
        {
            snake.state = 0; // Restart the game
        }
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
        snake_draw();
    }
}

void Game_on_exit(void)
{
    return;
}