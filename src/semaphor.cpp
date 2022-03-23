#include "semaphor.h"
#include "akernSem.h"

Semaphore::Semaphore (int init){
	lock
	myImpl = new KernelSem(init);
	unlock
}

Semaphore::~Semaphore (){
	lock
	delete myImpl;
	unlock
}

int Semaphore::wait (Time maxTimeToWait){
	lock
	int toReturn = myImpl->wait(maxTimeToWait);
	unlock
	return toReturn;
}
void Semaphore::signal(){
	lock
	myImpl->signal();
	unlock
}

int Semaphore::val () const {
	lock
	myImpl->val();
	unlock
}
