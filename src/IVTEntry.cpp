#include "IVTEntry.h"

IVTEntry* IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, IntRoutPtr intRoutPtr){
	lock
	this->ivtNo = ivtNo;
#ifndef BCC_BLOCK_IGNORE
	this->oldRout = getvect(ivtNo);
	setvect(ivtNo, intRoutPtr);
#endif
	this->kernelEv = 0;
	IVTable[ivtNo] = this;
	unlock
}

IVTEntry::~IVTEntry(){
	lock
	kernelEv = 0;
	IVTable[ivtNo] = 0;
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRout);
#endif
	oldRoutine();
	unlock
}

void IVTEntry::oldRoutine(){
	lock
	if (oldRout) oldRout();
	unlock
}

void IVTEntry::setKernelEv(KernelEv* kernelEv){
	lock
	this->kernelEv = kernelEv;
	unlock
}

void IVTEntry::removeKernelEv(){
	lock
	this->kernelEv = 0;
	unlock
}

IVTEntry* IVTEntry::getIVTEntry (IVTNo ivtNo){
	lock
	IVTEntry* ivtEntry = IVTable[ivtNo];
	unlock

	return ivtEntry;
}

void IVTEntry::signal(){
	lock
	if(kernelEv != 0)
		kernelEv->signal();
	unlock
}
