#include "state.h"
#include "logging.h"

void genSetState(char newst){
    switch(newst)
    {
        case GEN_ARGS:          genPrint("Parsing args");           break;
        case GEN_SPRITELOAD:    genPrint("Loading sprites");        break;
        case GEN_PREPARING:     genPrint("Creating a blank map");   break;
        case GEN_WORKING:       genPrint("Rendering");              break;
        case GEN_INDEXING:      genPrint("Indexing");               break;
        case GEN_SAVING:        genPrint("Saving");                 break;

        /* Some state changes isn't needed to be broadcasted */
        case GEN_NOTHING:   break;
        case GEN_SHUTDOWN:  break;

        default: genPrint("Warning: attempted to set up an unexcepted application state");
    }

    if(newst <= GEN_SHUTDOWN)
        genState = newst;
}
