#ifndef __APP_H__
#define __APP_H__

typedef void (*Appfunction_ptr)(void);

void App_Startup(const int width, const int height, const char *title,
                 Appfunction_ptr init,     // run on application startup
                 Appfunction_ptr update,   // run on every loop
                 Appfunction_ptr on_exit); // run on exit of application

#endif // __APP_H__