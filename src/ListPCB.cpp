#include "ListPCB.h"

ListPCB::Elem::Elem(PCB* pcb){
	lock
	this->pcb = pcb;
	next = 0;
	unlock
}

ListPCB::ListPCB(){
	lock
	head = tail = 0;
	length = 0;
	unlock
}

ListPCB::~ListPCB(){
	lock
	while (head != 0){
		Elem* old = head;
		head = head->next;
		delete old;
	}
	unlock
}

int ListPCB::isEmpty(){
	return length == 0;
}

int ListPCB::getLength(){
	//lock
	return length;
//	unlock
}

void ListPCB::append(PCB* pcb){
	lock
	Elem* newElem = new Elem(pcb);
	if (isEmpty()){
		head = newElem;
	} else {
		tail->next = newElem;
	}
	tail = newElem;
	length++;
	unlock
}

void ListPCB::remove(PCB* pcb){
	lock
	Elem* curr = head;
	Elem* prev = 0;

	while (curr->next != 0 && curr->pcb != pcb){
		prev = curr;
		curr = curr->next;
	}
	if (curr->pcb == pcb){
		if (curr == head && curr == tail){
			head = tail = 0;
		} else if (curr == head){
			head = head->next;
		} else if (curr == tail){
			tail = prev;
			tail->next = 0;
		} else {
			prev->next = curr->next;
		}
		delete curr;
		length--;
	}
	unlock
}

PCB* ListPCB::removeLast(){
	lock
	if (head == 0) {
		unlock
		return 0;
	}
	if (head->next == 0) {
		Elem* tmp = head;
		PCB* toReturn = tmp->pcb;
		delete head;
		head = tail = 0;
		length--;
		unlock
		return toReturn;
	}
	// Find the second last node
	Elem* secondLast = head;
	while (secondLast->next->next != 0)
		secondLast = secondLast->next;
	Elem* tmp = secondLast->next;
	PCB* toReturn = tmp->pcb;
	delete secondLast->next;
	tail = secondLast;
	secondLast->next = 0;
	length--;
	unlock
	return toReturn;
}

PCB* ListPCB::removeFirst(){
	lock
	if (head == 0){
		unlock
		return 0;
	}

	Elem* tmp = head;
	if (head->next != 0) head = head->next;
	else head = tail = 0;

	PCB* toReturn = tmp->pcb;
	delete tmp;
	length--;
	unlock

	return toReturn;
}
