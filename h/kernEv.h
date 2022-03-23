#ifndef H_KERNEV_H_
#define H_KERNEV_H_

#include "PCB.h"


typedef unsigned char IVTNo;

class IVTEntry;

class KernelEv{
public:
	KernelEv(IVTNo);
	~KernelEv();
	void wait();
	void signal();

private:
	IVTNo ivtNo;
	int value;
	PCB *eventPCB;
	int blocked;
};


#endif /* H_KERNEV_H_ */
