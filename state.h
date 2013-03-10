#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED

/* State storage */

extern int genState;        // current application state
extern int progbar;         // progressbar position
extern int progbar_max;     // max progressbar value

/* Possible states */
enum
{
    GEN_NOTHING,
    GEN_ARGS, GEN_SPRITELOAD, GEN_MKMASK,\
    GEN_WORKING, GEN_SAVING,
    GEN_SHUTDOWN
};

#endif // STATE_H_INCLUDED
