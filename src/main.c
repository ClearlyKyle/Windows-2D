#include "./engine/app.h"
#include "snake/game.h"

int main(void)
{
    App_Startup(800,
                800,
                "Something here",
                Game_init,
                Game_update,
                Game_on_exit);
}
