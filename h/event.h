// File: event.h
#ifndef _event_h_
#define _event_h_

#define PREPAREENTRY(ivtNo, callOldRoutFlag) \
void interrupt inter##ivtNo(...); \
IVTEntry newEntry##ivtNo(ivtNo, inter##ivtNo); \
void interrupt inter##ivtNo(...) { \
	if (callOldRoutFlag == 1) \
		newEntry##ivtNo.oldRoutine(); \
	newEntry##ivtNo.signal(); \
}



typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
KernelEv* myImpl;
};
#endif
