#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include "params.h"
#include "state.h"
//#include "logging.h"

// Is destination file is available to write?
int genIsFileCanBeWritten(const char * fname)
{
    FILE *f = NULL;
    if((f = fopen(fname, "w")) == NULL)
        return 0;
    else
    {
        fclose(f);
        return 1;
    }
}

t_genParams * genParseArgs(int argc, char *argv[])
{
    /// Switching application's state here
    genState = GEN_ARGS; // Calling of genSetState will not make effect

    t_genParams * params = malloc(sizeof(t_genParams));
    memset(params, 0, sizeof(t_genParams));

    params->deadzone = DEFAULT_DEADZONE;
    params->colors = DEFAULT_COLORS;

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

        /* Deadzone */
        if(!strcmp(argv[i], "-d"))
            params->deadzone = atoi(argv[i+1]);

        /* Rotate flag */
        if(!strcmp(argv[i], "-r"))
            params->rotate = atoi(argv[i+1]);

        /* Palette colors */
        if(!strcmp(argv[i], "-c"))
            params->colors = atoi(argv[i+1]);

        /* .. and filenames */
        if(!strcmp(argv[i], "-i"))
        {
            params->list_fn = malloc(strlen(argv[i+1]));
            strcpy(params->list_fn, argv[i+1]);
        }

        if(!strcmp(argv[i], "-o"))
        {
            params->out_fn = malloc(strlen(argv[i+1]));
            strcpy(params->out_fn, argv[i+1]);
        }
    }

    /* Fixing incorrect values */
    if(!params->width) params->width        = DEFAULT_W;
    if(!params->height) params->height      = DEFAULT_H;
    if(!params->list_fn) params->list_fn    = DEFAULT_FILENAME;
    if(!params->seed) params->seed              = DEFAULT_SEED;
    if(!params->intensity) params->intensity    = DEFAULT_INTENSITY;
    if(!params->out_fn) params->out_fn          = DEFAULT_OUT_FN;
    //if(!params->deadzone) params->deadzone          = DEFAULT_DEADZONE;
    if(!params->colors) params->colors          = DEFAULT_COLORS;

    /* Okay. */
    return params;
}

void genPrintParams(t_genParams * params)
{
    printf("\n=== [Settings:] =========================\n"
           "= Width:\t\t%u\n= Height:\t\t%u\n"
           "= List file name:\t%s\n"
           "= Seed:\t\t\t%d\n= Intensity:\t\t%d\n"
           "= Deadzone:\t\t%u\n= Rotated?:\t\t%s\n"
           "= Output file name:\t%s\n"
           "= Palette colors:\t%u\n"
           "=========================================\n\n",

           params->width, params->height, params->list_fn,
           params->seed, params->intensity, params->deadzone,
           params->rotate ? "yes" : "no", params->out_fn,
           params->colors);
}
