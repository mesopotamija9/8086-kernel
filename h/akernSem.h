#ifndef H_AKERNSEM_H_
#define H_AKERNSEM_H_

#include "ListPCB.h"
#include "SCHEDULE.h"
#include "global.h"
#include "ListSEM.h"

class ListSEM;

class KernelSem{
public:
	static ListSEM* listOfAllSemaphores;

	KernelSem (int init = 1);
	~KernelSem();

	virtual int wait (Time maxTimeToWait);
	virtual void signal();

	int val () const;

	ListPCB* getBlockedLimited();
	ListPCB* getBlocked();

private:
	int value;
	ListPCB* blocked;
	ListPCB* blockedLimited;
};

#endif /* H_AKERNSEM_H_ */
