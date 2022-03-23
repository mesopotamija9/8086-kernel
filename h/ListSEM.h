#ifndef H_LISTSEM_H_
#define H_LISTSEM_H_

#include "akernSem.h"

class ListSEM{
public:
	struct Elem{
		KernelSem* sem;
		Elem* next;
		Elem(KernelSem*);
	};

	Elem* head;
	Elem* tail;
	int length;

	ListSEM();
	~ListSEM();
	int getLength();
	int isEmpty();

	void append(KernelSem*);
	void remove(KernelSem*);

	KernelSem* removeFirst();
};

#endif /* H_LISTSEM_H_ */
