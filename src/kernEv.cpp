#include "kernEv.h"
#include "IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtNo){
	this->ivtNo = ivtNo;
	this->value = 0;
	this->eventPCB = PCB::running;
	this->blocked = 0;
	IVTEntry::IVTable[ivtNo]->setKernelEv(this);
}

KernelEv::~KernelEv(){
	signal();
	IVTEntry::IVTable[ivtNo]->removeKernelEv();
}

void KernelEv::signal(){
	if(blocked == 0) value = 1;
	else {
		eventPCB->setState(PCB::READY);
		Scheduler::put(eventPCB);
		blocked = 0;
		dispatch();
	}
}

void KernelEv::wait() {
	if(PCB::running != eventPCB) return;
	if(value == 1) value = 0;
	else {
		PCB::running->setState(PCB::BLOCKED);
		blocked = 1;
		dispatch();
	}
}
