#include "ListSEM.h"

ListSEM::Elem::Elem(KernelSem* sem){
	lock
	this->sem = sem;
	next = 0;
	unlock
}

ListSEM::ListSEM(){
	lock
	head = tail = 0;
	length = 0;
	unlock
}

ListSEM::~ListSEM(){
	lock
	while (head != 0){
		Elem* old = head;
		head = head->next;
		delete old;
	}
	unlock
}

int ListSEM::getLength(){
	return length;
}

int ListSEM::isEmpty(){
	return length == 0;
}

void ListSEM::append(KernelSem* sem){

	lock
	Elem* newElem = new Elem(sem);
	if (isEmpty()){
		head = newElem;
	} else {
		tail->next = newElem;
	}

	tail = newElem;
	length++;
	unlock
}

void ListSEM::remove(KernelSem* sem){
	lock
	Elem* curr = head;
	Elem* prev = 0;

	while (curr->sem != 0 && curr->sem != sem){
		prev = curr;
		curr = curr->next;
	}
	if (curr->sem == sem){
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

KernelSem* ListSEM::removeFirst(){
	lock
	if (head == 0){
		unlock
		return 0;
	}

	Elem* tmp = head;
	if (head->next != 0) head = head->next;
	else head = tail = 0;

	KernelSem* toReturn = tmp->sem;
	delete tmp;
	length--;
	unlock

	return toReturn;
}
