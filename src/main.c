#include "physics_examples/collision_testing/collision_testing.h"
#include "physics_examples/physics_world/world.h"
#include "snake/game.h"

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
    // App_Startup(1000,
    //            800,
    //            "Something here",
    //            Collision_testing_init,
    //            Collision_testing_update,
    //            Collision_testing_on_render,
    //            Collision_testing_on_exit);

    // 2D Physics World
    App_Startup(1000,
                800,
                "Something here",
                World_init,
                World_update,
                World_on_render,
                World_on_exit);
}
