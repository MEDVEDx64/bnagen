/** BnA map generator
    MEDVEDx64, 2013.03.10-12
    GPLv2.
**/

#define BNAGEN_VERSION "0.03"

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "params.h"
#include "loader.h"
#include "drawthemap.h"
#include "state.h"
#include "help_message.h"

#include <stdio.h>
#include <string.h>

#define FATAL_ERROR(mesg) do {      \
    printf(mesg);                   \
    return 1;                       \
}                                   \
while(0)

#define SCRW 300
#define SCRH 200

#ifndef FONT_FN
#   define FONT_FN "font.ttf"
#endif
#ifndef SHARE_DIR
#   define SHARE_DIR "/usr/share/bnagen/"
#endif // SHARE_DIR
#define FONT_SIZE 12

// State variables
int genState = GEN_NOTHING;
int progbar = 0;
int progbar_max = 0;

// Set up video subsystem
int initGfx()
{
    if(SDL_Init(SDL_INIT_EVERYTHING))
        FATAL_ERROR("Ouch, failed to init SDL\n");
    SDL_SetVideoMode(SCRW, SCRH, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);

    SDL_WM_SetCaption("bnagen dummy window", 0);
    return 0;
}

static TTF_Font * font = NULL;

int initTTF()
{
    if(TTF_Init())
        FATAL_ERROR("TTF_Init returned an error state.\n");

    // Loading the font from file
    font = TTF_OpenFont(FONT_FN, FONT_SIZE);
    if(!font)
    {
        // Keep trying...
        font = TTF_OpenFont(SHARE_DIR FONT_FN, FONT_SIZE);
        if(/* still */ !font)
            FATAL_ERROR("Font file couldn't be loaded.\n");
    }

    return 0;
}

#define MKTEXTSURF(str)                                             \
    do {                                                            \
        text_surf = TTF_RenderText_Blended(font, str, col);             \
        SDL_BlitSurface(text_surf, NULL, SDL_GetVideoSurface(), NULL);  \
        SDL_FreeSurface(text_surf);                                     \
    } while(0)

static SDL_Rect pbr; // Progress bar rectangle

#define PBAR_DRAW           do {                                        \
    pbr.w = (int)(((float)SCRW/(float)progbar_max)*(float)progbar)+1;   \
    SDL_FillRect(SDL_GetVideoSurface(), &pbr, 0x800000);                \
} while(0)

#ifndef DRAW_DELAY
#   define DRAW_DELAY 0x20
#endif // DRAW_DELAY

// Showin' some stuff into the main window
int drawState_Thread(void * unused)
{
    SDL_Color col;
    col.r = 200;
    col.g = 200;
    col.b = 200;

    pbr.x = 0;
    pbr.y = 160;
    pbr.h = 20;

    char * text_buf = malloc(0x1000);
    SDL_Surface * text_surf = NULL;

    while(genState != GEN_SHUTDOWN)
    {
        SDL_FillRect(SDL_GetVideoSurface(), NULL, 0x40); // clear screen

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
                PBAR_DRAW;
                break;

            case GEN_SAVING:

                MKTEXTSURF("Saving...");
                PBAR_DRAW;
                break;

        }

        // Drawing
        SDL_Flip(SDL_GetVideoSurface());
        SDL_Delay(DRAW_DELAY);
    }

    free(text_buf);

    TTF_Quit();
    SDL_Quit();

    return 0;
}

static SDL_Event ev;

int eventsThread(void * unused)
{
    while(genState != GEN_SHUTDOWN)
    {
        if(SDL_PollEvent(&ev))
        {
            if(ev.type == SDL_QUIT)
            {
                genState = GEN_SHUTDOWN;
                printf("Caught quit request.\n");
                exit(0);
            }
        }

        else SDL_Delay(DRAW_DELAY);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    /// Looking for '-?' or '--help' argument first
    int a; for(a = 0; a < argc; a++)
    {
        if(!strcmp(argv[a], "-?") || !strcmp(argv[a], "--help"))
        {
            SHOWHELP;
            return 0;
        }

        /// And for '--version'
        else if(!strcmp(argv[a], "--version"))
        {
            printf("bnagen " BNAGEN_VERSION "\n");
            return 0;
        }
    }

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
