#include <malloc.h>
#include <stdio.h>
#include <SDL/SDL_image.h>

#include "loader.h"
#include "state.h"

t_genSprites * genLoadSprites(const char * list_fn)
{
    genState = GEN_SPRITELOAD;

    FILE *f;
    if(!(f = fopen(list_fn,"r")))
    {
        printf("%s: fatal: can't open list file.\n", __FUNCTION__);
        return NULL;
    }

    t_genSprites *spr = malloc(sizeof(t_genSprites));
    memset(spr, 0, sizeof(t_genSprites));

    char * buf = malloc(0x1000);
    while(!feof(f))
    {
        /* Check for limits first */
        if(spr->count >= MAX_SPRITES) break;

        /* Line-by-line reading */
        if(fscanf(f, "%s\n", buf) != 1) continue;

        /* Looking for sharp */
        if(*buf == '#') continue;

        /* Attempting to load a sprite */
        //spr->surf[spr->count] = malloc(sizeof(SDL_Surface));
        spr->surf[spr->count] = IMG_Load(buf);
        if(spr->surf[spr->count] == NULL) continue;

        spr->count++;
    }

    printf("%s: successfully loaded %d sprites.\n",
           __FUNCTION__, spr->count);
    free(buf);
    return spr;
}
