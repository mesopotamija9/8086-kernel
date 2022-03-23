#ifndef H_LISTPCB_H_
#define H_LISTPCB_H_

#include "PCB.h"

class ListPCB{
public:
	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB*);
	};

	Elem* head;
	Elem* tail;
	int length;

	ListPCB();
	~ListPCB();
	int isEmpty();
	int getLength();
	void append(PCB*);
	void remove(PCB*);
	PCB* removeLast();
	PCB* removeFirst();
};



#endif /* H_LISTPCB_H_ */
