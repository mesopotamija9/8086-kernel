#include "akernSem.h"

ListSEM* KernelSem::listOfAllSemaphores = new ListSEM();

KernelSem::KernelSem (int init){
	this->value = init;
	this->blocked = new ListPCB();
	this->blockedLimited = new ListPCB();

	listOfAllSemaphores->append(this);
}

KernelSem::~KernelSem(){
	listOfAllSemaphores->remove(this);

	delete blocked;
	delete blockedLimited;
}

int KernelSem::val () const {
	return value;
}

int KernelSem::wait (Time maxTimeToWait){
	PCB* savedPCB;

	if (maxTimeToWait == 0){
		if (--value < 0) {
			PCB::running->setState(PCB::BLOCKED);
			blocked->append(PCB::running);
			PCB::running->signal = 0;
			PCB::running->sem = this;
			savedPCB = PCB::running;

			dispatch();

			return savedPCB->signal;
		}
	} else {
		if (--value < 0) {
			lock

			unlock
			PCB::running->maxTimeToWait = maxTimeToWait;
			PCB::running->setState(PCB::BLOCKED);
			blockedLimited->append(PCB::running);
			PCB::running->signal = 0;
			PCB::running->sem = this;
			savedPCB = PCB::running;

			dispatch();

			return savedPCB->signal;
		}
	}


	return 1;
}

void KernelSem::signal(){

	if (++value <= 0){
		if (blocked->isEmpty() == 0){
			PCB* next = blocked->removeFirst();
			if (next != 0){
				next->setState(PCB::READY);
				Scheduler::put(next);
			}

			if (next->sem == this) next->signal = 1;
		}
		else if (blockedLimited->isEmpty() == 0){
			PCB* next = blockedLimited->removeFirst();
			if (next != 0){
				next->setState(PCB::READY);
				Scheduler::put(next);
			}

			if (next->sem == this) next->signal = 1;
		}
	}

}

ListPCB* KernelSem::getBlockedLimited(){
	return blockedLimited;
}

ListPCB* KernelSem::getBlocked(){
	return blocked;
}

