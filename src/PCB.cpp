#include "PCB.h"

ID PCB::idGenerator = 0;
PCB* PCB::running = 0;
PCB* PCB::mainThread = 0;
PCB* PCB::idleThread = 0;

ListPCB* PCB::listOfAllPCB = new ListPCB();
ListPCB* PCB::listOfNotDonePCB = new ListPCB();

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread){
	if (stackSize < 1024) stackSize = 1024;
	else if (stackSize > 65535) stackSize = 65535; // 65536 B = 64 KB (maximum stack size) [index 0 - 65535]

	this->id = idGenerator++;
	this->state = CREATED;
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	this->thread = thread;
	this->listOfAllBlockedPCB = new ListPCB();

	listOfAllPCB->append(this);
	listOfNotDonePCB->append(this);

	signal = 0;

	int size = stackSize / sizeof(unsigned);
	stack = new unsigned[size];

	#ifndef BCC_BLOCK_IGNORE
	stack[size - 1] =0x200;//setovan I fleg u
	                      // pocetnom PSW-u za nit
	stack[size - 2] = FP_SEG(&wrapper);
	stack[size - 3] = FP_OFF(&wrapper);

		sp = FP_OFF(stack + size - 12); //svi sacuvani registri

	 //pri ulasku u interrupt
	 //rutinu
		ss = FP_SEG(stack + size - 12);
		bp = FP_OFF(stack + size - 12);
	#endif
		zavrsio = 0;
}

PCB::~PCB(){
	listOfAllPCB->remove(this);

	delete listOfAllBlockedPCB;
	delete [] stack;
}

void PCB::exitThread(){
	lock
	while (running->getListOfAllBlockedPCB()->isEmpty() == 0){
		PCB* pcb = running->getListOfAllBlockedPCB()->removeLast();
		pcb->setState(READY);
		Scheduler::put(pcb);
	}

	running->setState(DONE);
	listOfNotDonePCB->remove(running);
	unlock

	dispatch();

}

void PCB::wrapper(){
	running->getThread()->run();
	exitThread();
}

ID PCB::getRunningId() { return running->getId(); }
PCB* PCB::getThreadById(ID id) {
	lock
	ListPCB::Elem* curr = listOfAllPCB->head;
	while (curr != 0){
		if (curr->pcb->getId() == id){
			unlock
			return curr->pcb;
		}
		curr = curr->next;
	}
	unlock
	return 0;
}

ID PCB::getId(){ return id; }
Thread* PCB::getThread() { return thread; }
PCB::State PCB::getState() { return state; }
void PCB::setState(State s) { state = s; }
Time PCB::getTimeSlice() { return timeSlice; }
void PCB::addToBlockedList(PCB* pcb) { listOfAllBlockedPCB->append(pcb); }
ListPCB* PCB::getListOfAllBlockedPCB() { return listOfAllBlockedPCB; }
