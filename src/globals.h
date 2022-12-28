#ifndef GLOBALS_H
#define GLOBALS_H

// This is only for use during debugging in absence of a console.  To use, include the .h file that you want to implement a breakpoint in
// then in the class, add the following block

/*
if (keyboardbreak==true){
    keyboardbreak=false; // add breakpoint here
    // do some manual parameter settings here if you wish for the method you want to debug
}
*/

// currently this is implemented as the "USE" command, but maybe I'll change to it's own "BREAK" command later
extern bool keyboardbreak;

#endif