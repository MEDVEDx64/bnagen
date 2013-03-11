#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

#define DEFAULT_W 1920
#define DEFAULT_H 696
#define DEFAULT_FILENAME "sprites.txt"

#define DEFAULT_SEED        128
#define DEFAULT_INTENSITY   38600

#define DEFAULT_OUT_FN "out.bmp"

typedef struct {
    /* Graphics list file name */
    char      * list_fn;
    char      * out_fn;
    int         seed;
    unsigned
         int    width;
    unsigned
         int    height;
    unsigned
         int    intensity;
}
t_genParams;

/* null on failure */
t_genParams * genParseArgs(int argc, char *argv[]);

#endif // PARAMS_H_INCLUDED
