
#include "global.h"
#include "thread.h"


class MainThread: public Thread{
public:
	int argc;
	char** argv;
	int retVal;
	MainThread(int argc, char** argv): Thread(){
		this->argc = argc;
		this->argv = argv;
	}
	~MainThread(){
		waitToComplete();
	}
	void run() {
		retVal = userMain(argc, argv);
	}
};

class IdleThread:public Thread{
public:
	IdleThread(): Thread(1024, 1){}
	~IdleThread(){
		waitToComplete();
	}
	void run();
};

void IdleThread::run(){
	while(1);
}

int main(int argc, char* argv[]){
	inic();

	MainThread* mainThread = new MainThread(argc, argv);
	mainThread->start();
	IdleThread* idleThread = new IdleThread();
	idleThread->getMyPCB()->setState(PCB::DONE);
	PCB::idleThread = idleThread->getMyPCB();

	mainThread->waitToComplete();
	PCB::running = mainThread->getMyPCB();
	
	restore();

	delete mainThread;
	delete idleThread;

	cout<<"Happy End"<<endl;
	return 0;
}
