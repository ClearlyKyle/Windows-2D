#include "./engine/app.h"
#include "snake/game.h"
#include "physics/physics_test.h"

int main(void)
{
    // Snake Game
    // App_Startup(400,
    //            400,
    //            "Something here",
    //            Game_init,
    //            Game_update,
    //            Game_on_render,
    //            Game_on_exit);

    // 2D Physics Test
    App_Startup(1000,
                800,
                "Something here",
                Physics_init,
                Physics_update,
                Physics_on_render,
                Physics_on_exit);
}
