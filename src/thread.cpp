
#include "thread.h"
#include "PCB.h"
#include "SCHEDULE.h"
#include "global.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
}

Thread::~Thread(){
	lock
	//waitToComplete();
	delete myPCB;
	unlock
}

ID Thread::getId(){
	return myPCB->getId();
}

ID Thread::getRunningId(){
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id){
	return PCB::getThreadById(id)->getThread();
}

void Thread::start(){
	// If start is called first time do this, otherwise ignore
	if (myPCB->getState() == PCB::CREATED){
		// Set state to READY
		myPCB->setState(PCB::READY);
		// Idle thread shouldn't be put in scheduler
		if (myPCB != PCB::idleThread){
			Scheduler::put(myPCB);
		}
	}
}

void Thread::waitToComplete(){
	if(myPCB->getState() == PCB::CREATED || myPCB->getState() == PCB::DONE) return;
	if(myPCB == PCB::idleThread || myPCB == PCB::running) return;
	PCB::running->setState(PCB::BLOCKED);
	myPCB->addToBlockedList(PCB::running);
	
	dispatch();
}

