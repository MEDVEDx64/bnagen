#include "logging.h"
#include "state.h"
#include <stdio.h>
#include <SDL/SDL.h>

SDL_mutex * log_mutex = NULL;
/* Char buffer where time string is stored */
char log_ticks[0x100];

void updateTicks()
{
    sprintf(log_ticks, "%06u", SDL_GetTicks());
}

void genPrint(const char * mesg)
{
    if(!log_mutex || !SDL_WasInit(0)) return;

    updateTicks();

    SDL_LockMutex(log_mutex);
    printf("%s: %s", log_ticks, mesg);
    SDL_UnlockMutex(log_mutex);
}

#ifndef LOG_PRINT_DELAY
#   define LOG_PRINT_DELAY 500
#endif // LOG_PRINT_DELAY

int loggingThread()
{
    char buffer[0x200]; // text string buffer
    //genPrint("++ Starting log file ++");

    while(genState != GEN_SHUTDOWN)
    {
        /* We'll only prinp rendering and indexing
            states here */
        if(genState == GEN_WORKING)
        {
            sprintf(buffer, "> %d/%d\n", progbar, progbar_max/2);
            genPrint(buffer);
        }
        else if(genState == GEN_INDEXING)
        {
            sprintf(buffer, "> %d/%d\n", progbar-progbar_max/2, progbar_max/2);
            genPrint(buffer);
        }

        SDL_Delay(LOG_PRINT_DELAY);
    }

    SDL_DestroyMutex(log_mutex);
    log_mutex = NULL;
    return 0;
}

int genStartLogging()
{
    if(!SDL_WasInit(0) || log_mutex) return 1;
    log_mutex = SDL_CreateMutex();
    SDL_CreateThread(loggingThread, NULL);
    return 0;
}
