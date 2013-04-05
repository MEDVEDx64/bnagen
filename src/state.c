#include "state.h"
#include "logging.h"

#define STAGES "5"
#define STAGEOF(s) "(stage " s " of " STAGES ")"

void genSetState(char newst){
    switch(newst)
    {
        //case GEN_ARGS:          genPrint("Parsing args " STAGEOF("0") "\n");           break;
        case GEN_SPRITELOAD:    genPrint("Loading sprites " STAGEOF("1") "\n");        break;
        case GEN_PREPARING:     genPrint("Creating a blank map " STAGEOF("2") "\n");   break;
        case GEN_WORKING:       genPrint("Rendering " STAGEOF("3") "\n");              break;
        case GEN_INDEXING:      genPrint("Indexing " STAGEOF("4") "\n");               break;
        case GEN_SAVING:        genPrint("Saving " STAGEOF("5") "\n");                 break;

        /* Some state changes isn't needed to be broadcasted */
        case GEN_NOTHING:   break;
        case GEN_SHUTDOWN:  break;

        default: genPrint("Warning: attempted to set up an unexcepted application state\n");
    }

    if(newst <= GEN_SHUTDOWN)
        genState = newst;
}
