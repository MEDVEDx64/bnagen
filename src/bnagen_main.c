/** BnA map generator
    MEDVEDx64, 2013.03.10-15
    GPLv2.
**/

#define BNAGEN_VERSION "0.04_3_indev"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "params.h"
#include "loader.h"
#include "drawthemap.h"
#include "state.h"
#include "help_message.h"

#include <stdio.h>
#include <string.h>

/* Printing an error message and running out */
#define FATAL_ERROR(mesg) do {      \
    printf(mesg);                   \
    return 1;                       \
}                                   \
while(0)

/* Screen width/height */
#define SCRW 300
#define SCRH 200

/* Font file name and it's path.
   Can be redefined manually when compiling.
*/
#ifndef FONT_FN
#   define FONT_FN "font.ttf"
#endif
#ifndef SHARE_DIR
#   define SHARE_DIR "/usr/share/bnagen/"
#endif // SHARE_DIR

/* Font size to be rendered */
#define FONT_SIZE 12

/* State variables (see state.h for details) */
int genState = GEN_NOTHING;
int progbar = 0;
int progbar_max = 0;

/* Setting up video subsystem. Returns 0 on success. */
int initGfx()
{
    // Starting SDL...
    if(SDL_Init(SDL_INIT_EVERYTHING))
        FATAL_ERROR("Failed to initialize SDL\n");

    // Setting up SCRWxSCRH window
    SDL_SetVideoMode(SCRW, SCRH, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);

    // Window caption
    SDL_WM_SetCaption("bnagen dummy window", 0);
    return 0;
}

static TTF_Font * font = NULL; // Storing the font here

/* Initializing TTF — needed to render text stuff on the window */
int initTTF()
{
    if(TTF_Init())
        FATAL_ERROR("TTF_Init returned an error state.\n");

    // Loading the font from file
    font = TTF_OpenFont(FONT_FN, FONT_SIZE);
    if(!font)
    {
        // Keep trying... looking in alternative directory
        font = TTF_OpenFont(SHARE_DIR FONT_FN, FONT_SIZE);
        if(/* still */ !font)
            FATAL_ERROR("Font file couldn't be loaded.\n");
    }

    return 0;
}

/* Create, draw, then destroy a text surface with 'str' as a string.
   Made only to be used inside of drawState_Thread.
*/
#define MKTEXTSURF(str)                                                 \
    do {                                                                \
        text_surf = TTF_RenderText_Blended(font, str, col);             \
        SDL_BlitSurface(text_surf, NULL, SDL_GetVideoSurface(), NULL);  \
        SDL_FreeSurface(text_surf);                                     \
    } while(0)

static SDL_Rect pbr; // Progress bar rectangle

/* Draw the progress bar */
#define PBAR_DRAW           do {                                            \
    pbr.w = (int)(((float)SCRW/(float)progbar_max)*(float)progbar)+1;       \
    SDL_FillRect(SDL_GetVideoSurface(), &pbr, 0x900000 /* it's color*/ );   \
} while(0)

/* Delay between refreshing the window.
   Also, it is a delay between event querying
   in eventsThread.
*/
#ifndef DRAW_DELAY
#   define DRAW_DELAY 0x20
#endif // DRAW_DELAY

/* Window rendering thread.
   The window shows such information like current application
   state and the progress bar.
*/
int drawState_Thread(void * unused)
{
    // Font color
    SDL_Color col;
    col.r = 200;
    col.g = 200;
    col.b = 200;

    // Progress bar rectangle
    pbr.x = 0;
    pbr.y = 160;
    pbr.h = 20;

    char * text_buf = malloc(0x1000);
    SDL_Surface * text_surf = NULL;

    // Running 'til shutdown state will not occur
    while(genState != GEN_SHUTDOWN)
    {
        SDL_FillRect(SDL_GetVideoSurface(), NULL, 0x40); // clear screen

        // Showing different information on different states
        switch(genState)
        {

            case GEN_ARGS:

                MKTEXTSURF("Parsing args...");
                break;

            case GEN_SPRITELOAD:

                MKTEXTSURF("Loading sprites...");
                break;

            case GEN_PREPARING:

                MKTEXTSURF("Allocating $$$...");
                break;

            case GEN_WORKING:

                // Cooking text surface
                sprintf(text_buf, "Rendering: %d/%d", progbar, progbar_max);
                MKTEXTSURF(text_buf);
                PBAR_DRAW; // progress bar
                break;

            case GEN_SAVING:

                MKTEXTSURF("Saving...");
                PBAR_DRAW;
                break;

        }

        // Drawing
        SDL_Flip(SDL_GetVideoSurface());
        // Waiting
        SDL_Delay(DRAW_DELAY);
    }

    free(text_buf);

    // Shutting down SDL/TTF
    TTF_Quit();
    SDL_Quit();

    return 0;
}

static SDL_Event ev; // events here

/* This thread is waiting for SDL_QUIT signal
   and sets the shutdown state when catching it.
*/
int eventsThread(void * unused)
{
    while(genState != GEN_SHUTDOWN)
    {
        if(SDL_PollEvent(&ev)) // Querying...
        {
            if(ev.type == SDL_QUIT)
            {
                genState = GEN_SHUTDOWN;
                printf("Caught quit request.\n");
                exit(0);
            }
        }

        else SDL_Delay(DRAW_DELAY); // Waiting if nothing happens this time
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // Looking for '-?' or '--help' argument first
    int a; for(a = 0; a < argc; a++)
    {
        if(!strcmp(argv[a], "-?") || !strcmp(argv[a], "--help"))
        {
            SHOWHELP;
            return 0;
        }

        // And for '--version'
        else if(!strcmp(argv[a], "--version"))
        {
            printf("bnagen " BNAGEN_VERSION "\n");
            return 0;
        }
    }

    // Intro message
    printf("bnagen " BNAGEN_VERSION "\n");

    // Parsing commandline arguments
    t_genParams *parm = genParseArgs(argc, argv);
    if(parm == NULL)
        FATAL_ERROR("Failure occured while parsing args.\n");

    // Starting up graphics and TTF subsystems
    if(initGfx() | initTTF()) return 1;

    // Creating threads
    SDL_CreateThread(drawState_Thread, NULL);
    SDL_CreateThread(eventsThread, NULL);

    // Loading sprites
    t_genSprites * spr = genLoadSprites(parm->list_fn);
    if(spr == NULL)
        FATAL_ERROR("Fatal error occured while loading sprites.\n");

    // If it's allight, rendering resulting BnA map
    SDL_Surface * themap = genCreateBnAMap(parm, spr);

    if(themap == NULL)
        FATAL_ERROR("Fatal: the map was not created.\n");

    // And saving it into BMP.
    genState = GEN_SAVING;
    if(SDL_SaveBMP(themap, parm->out_fn))
        FATAL_ERROR("Fatal: unable to write the map file.\n");

    // Freeing, etc.
    SDL_FreeSurface(themap);

    genState = GEN_SHUTDOWN;
    printf("Done.\n");
    return 0;
}
