#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

/* Application state storage */

extern int genState;        // current application state
extern int progbar;         // progressbar position
extern int progbar_max;     // max progressbar value

/* Possible states */
enum
{
    GEN_NOTHING,
    GEN_ARGS,           /// Parsing args
    GEN_SPRITELOAD,     /// Loading sprites
    GEN_PREPARING,      /// Allocating a space for the map
    GEN_WORKING,        /// Rendering
    GEN_INDEXING,       /// Converting into palettized format
    GEN_SAVING,         /// Saving in progress
    GEN_SHUTDOWN        /// It's the time to shut down
};

void genSetState(char newst); /// Switch state and print it into the log

#endif // STATE_H_INCLUDED
