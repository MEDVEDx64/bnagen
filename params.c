#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "params.h"
#include "state.h"

t_genParams * genParseArgs(int argc, char *argv[])
{
    /*if(argc < 2)
    {
        printf("%s: too few args.\n", __FUNCTION__);
        return NULL;
    }*/

    genState = GEN_ARGS;

    t_genParams * params = malloc(sizeof(t_genParams));
    memset(params, 0, sizeof(t_genParams));

    int i;
    for(i = 0; i < argc-1; i++)
    {
        /* w/h */
        if(!strcmp(argv[i], "-w"))
            params->width = atoi(argv[i+1]);

        if(!strcmp(argv[i], "-h"))
            params->height = atoi(argv[i+1]);

        /* Seed and intensity */
        if(!strcmp(argv[i], "-s"))
            params->seed = atoi(argv[i+1]);

        if(!strcmp(argv[i], "-I"))
            params->intensity = atoi(argv[i+1]);

        /* .. and filename */
        if(!strcmp(argv[i], "-i"))
        {
            params->list_fn = malloc(strlen(argv[i+1]));
            strcpy(params->list_fn, argv[i+1]);
        }
    }

    /* Fixing incorrect values */
    if(!params->width) params->width        = DEFAULT_W;
    if(!params->height) params->height      = DEFAULT_H;
    if(!params->list_fn) params->list_fn    = DEFAULT_FILENAME;
    if(!params->seed) params->seed              = DEFAULT_SEED;
    if(!params->intensity) params->intensity    = DEFAULT_FILENAME;

    /* Okay. */
    return params;
}