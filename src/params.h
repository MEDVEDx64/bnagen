#ifndef PARAMS_H_INCLUDED
#define PARAMS_H_INCLUDED

/* Default params */
#define DEFAULT_W 1920
#define DEFAULT_H 696
#define DEFAULT_FILENAME "sprites.txt"

#define DEFAULT_SEED        0
#define DEFAULT_INTENSITY   38600

#define DEFAULT_OUT_FN "out.png"
#define DEFAULT_DEADZONE 78
#define DEFAULT_COLORS 63

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
    unsigned
        char    colors;     // Max. colors in palette
}
t_genParams;

/* Returns null on failure */
t_genParams * genParseArgs(int argc, char *argv[]);

void genPrintParams(t_genParams * params);
int  genIsFileCanBeWritten(const char * fname);

#endif // PARAMS_H_INCLUDED
