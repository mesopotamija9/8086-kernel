//pretpostavljeni memorijski model: huge

#include "global.h"
#include "akernSem.h"

//pomocne promenljive za prekid tajmera
unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;

volatile unsigned int lockFlag = 1;

//void tick (){
//
//}

void interrupt timer(){	// prekidna rutina
	int dummy =0;
	if (!zahtevana_promena_konteksta) {
		tick();

		// Proveri da li postoje semafori gde je maxTimeToWait > 0
		int allSemLength = KernelSem::listOfAllSemaphores->getLength();

#ifndef BCC_BLOCK_IGNORE
		for (int i = 0; i < allSemLength; i++){
			KernelSem* nextSemaphore = KernelSem::listOfAllSemaphores->removeFirst();
			ListPCB* blockedLimited = nextSemaphore->getBlockedLimited();
			int blockedLimitedLength = blockedLimited->getLength();

			for(int j = 0; j < blockedLimitedLength; j++){
				PCB* nextPCB = blockedLimited->removeFirst();
				if (nextPCB->maxTimeToWait == 1){
					nextPCB->setState(PCB::READY);
					Scheduler::put(nextPCB);
				}
				else {
					nextPCB->maxTimeToWait--;
					blockedLimited->append(nextPCB);
				}

				blockedLimitedLength = blockedLimited->getLength();
			}

			KernelSem::listOfAllSemaphores->append(nextSemaphore);
		}
#endif



		brojac--;
	}
	if ((brojac == 0 && PCB::running->getTimeSlice() != 0 ) || zahtevana_promena_konteksta) {
		if (lockFlag){
			zahtevana_promena_konteksta = 0;
			asm {
				// cuva sp i bp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			
			if (PCB::running->getState() == PCB::READY) Scheduler::put(PCB::running);
			PCB::running = Scheduler::get();
			if (PCB::running == 0 && PCB::listOfNotDonePCB->getLength() > 2)
				PCB::running = PCB::idleThread;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			brojac = PCB::running->getTimeSlice();

			asm {
				mov sp, tsp   // restore sp
				mov ss, tss
				mov bp, tbp  //izmena
			}
		}
		else zahtevana_promena_konteksta = 1;
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
	if(!zahtevana_promena_konteksta) asm int 60h;
}

void dispatch(){ // sinhrona promena konteksta
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}
