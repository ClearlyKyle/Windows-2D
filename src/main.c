#include "./engine/app.h"
#include "snake/game.h"

int main(void)
{
    App_Startup(400,
                400,
                "Something here",
                Game_init,
                Game_update,
                Game_on_render,
                Game_on_exit);
}
