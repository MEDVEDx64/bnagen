/** logging system **/

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

/* Starts logging thread, returns non-zero on failure. */
int  genStartLogging();
void genPrint(const char * mesg); // '\n' is not needed

#endif // LOG_H_INCLUDED
