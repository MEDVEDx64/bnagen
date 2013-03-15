#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

/* Default params */
#define DEFAULT_W 1920
#define DEFAULT_H 696
#define DEFAULT_FILENAME "sprites.txt"

#define DEFAULT_SEED        0
#define DEFAULT_INTENSITY   38600

#define DEFAULT_OUT_FN "out.bmp"
//#define DEFAULT_DEADZONE 16

typedef struct {
    /* Graphics list file name */
    char      * list_fn;    // List file name
    char      * out_fn;     // Output map file name
    int         seed;
    unsigned
         int    width;      // Map width
    unsigned
         int    height;     // Map height
    unsigned
         int    intensity;
    int         deadzone;
    int         rotate;     // random sprite rotation toggler
}
t_genParams;

/* Returns null on failure */
t_genParams * genParseArgs(int argc, char *argv[]);

#endif // PARAMS_H_INCLUDED
