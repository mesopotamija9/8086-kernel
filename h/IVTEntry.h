#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include <dos.h>
#include "global.h"
#include "kernEv.h"

typedef unsigned char IVTNo;
typedef void interrupt (*IntRoutPtr)(...);

class IVTEntry{
public:
	static IVTEntry *IVTable[];
	static IVTEntry* getIVTEntry (IVTNo);

	IVTEntry(IVTNo, IntRoutPtr);
	~IVTEntry();

	void oldRoutine();
	void signal();

	void setKernelEv(KernelEv*);
	void removeKernelEv();

private:
	IVTNo ivtNo;
	KernelEv *kernelEv;
	IntRoutPtr oldRout;
};


#endif /* H_IVTENTRY_H_ */
