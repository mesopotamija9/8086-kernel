
#ifndef H_GLOBAL_H_
#define H_GLOBAL_H_

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.h"
#include "PCB.h"
#include "thread.h"
#include <stdlib.h>
#include "semaphor.h"
#include "event.h"
#include "IVTEntry.h"

#define lock asm pushf;\
asm cli;
#define unlock asm popf;


void inic();
void restore();
void interrupt timer();
void dispatch();
int userMain(int argc, char* argv[]);
void tick ();

#endif /* H_GLOBAL_H_ */
