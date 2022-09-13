#include "./engine/app.h"
#include "game.h"

int main(void)
{
    App_Startup(800,
                800,
                "Something here",
                Game_init,
                Game_update,
                Game_on_exit);
}
