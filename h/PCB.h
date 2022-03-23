
#ifndef H_PCB_H_
#define H_PCB_H_

#include <iostream.h>
#include <dos.h>
#include "thread.h"
#include "global.h"
#include "ListPCB.h"
#include "semaphore.h"

class ListPCB;

class PCB{
public:
	static enum State {CREATED, READY, BLOCKED, DONE};
	static ID idGenerator;
	static PCB* running;
	static PCB* mainThread;
	static PCB* idleThread;
	static ListPCB* listOfAllPCB;
	static ListPCB* listOfNotDonePCB;

	PCB(StackSize stackSize, Time timeSlice, Thread* thread);
	~PCB();

	static ID getRunningId();
	static PCB* getThreadById(ID);

	ID getId();
	Thread* getThread();
	State getState();
	void setState(State);
	Time getTimeSlice();
	void addToBlockedList(PCB*);
	ListPCB* getListOfAllBlockedPCB();

	void printPCB() {
		cout << "id: " << id << " timeSlice: " << timeSlice <<
				" stackSize: " << stackSize << "\n";
	}

	static void wrapper();
	static void exitThread();

	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned zavrsio;

	Time timeSlice;
	Time maxTimeToWait;
	KernelSem* sem;
	int signal;

protected:
	ID id;
	State state;
	unsigned* stack;
	StackSize stackSize;

	Thread* thread;
	ListPCB* listOfAllBlockedPCB;
};


#endif /* H_PCB_H_ */
