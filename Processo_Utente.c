

	#include "Utils.h"

	
	/*------------------------------------------------------------------------
	HANDLER, VARIBILI GLOBALI
	-------------------------------------------------------------------------*/
	int pidcontrollo=0;
	void handle_signal(int signum){
		switch(signum){
			case SIGTERM:
				exit(EXIT_SUCCESS);
				break;
			case SIGUSR1:
				printf("Transazione accolta e gestita\n");
				pidcontrollo=getpid();
				break;
		}
		
	}

	int SO_USERS_NUM ;
	int SO_NODES_NUM ;
	int SO_RETRY ;
	int SO_REWARD;
	int SO_TP_SIZE; 
	int SO_MIN_TRANS_GEN_NSEC ;
	int SO_MAX_TRANS_GEN_NSEC ;
	int SO_MIN_TRANS_PROC_NSEC ;
	int SO_MAX_TRANS_PROC_NSEC ;
	int SO_BUDGET_INIT ;
	int SO_SIM_SEC ;
	int SO_FRIENDS_NUM; 
	int SO_HOPS ;
	int k;
	int starttime;
	int miapos;
	int randomm;
	int lmind;
	int i,m,tempo;
	int tot;
	/*------------------------------------------------------------------------

	
	M A I N   P R O C E S S O   U T E N T E		
	
	
	-------------------------------------------------------------------------*/

	int main(int argc, char *argv[]) {
		int mybudget;
		int limit=0;
		int mp_id, lm_id,s_id,q_id;
		pid_t mypid;
		struct timespec timestamp;
		struct timespec waitgen;
		struct mpid *MP;
		struct sembuf sops[3];
		struct LibroMastro *LM;
		struct msgbuffer {
			long mtype;  
			struct Transazione tmessage;
		};
		struct msgbuffer my_message;
		SO_USERS_NUM =atoi(argv[5]);
		SO_NODES_NUM =atoi(argv[6]);
		SO_RETRY =atoi(argv[7]);
		SO_REWARD=atoi(argv[8]);
		SO_TP_SIZE=atoi(argv[9]);
		SO_MIN_TRANS_GEN_NSEC=atoi(argv[10]);
		SO_MAX_TRANS_GEN_NSEC=atoi(argv[11]); 
		SO_MIN_TRANS_PROC_NSEC=atoi(argv[12]);
		SO_MAX_TRANS_PROC_NSEC=atoi(argv[13]);
		SO_BUDGET_INIT =atoi(argv[14]);
		SO_SIM_SEC =atoi(argv[15]);
		SO_FRIENDS_NUM =atoi(argv[16]);
		SO_HOPS =atoi(argv[17]);
		mp_id = atoi(argv[1]);
		lm_id = atoi(argv[2]);
		s_id = atoi(argv[3]);
		q_id = atoi(argv[4]);
		MP= shmat(mp_id, NULL, 0);
		LM = shmat(lm_id, NULL, 0);
		srand (getpid());
		mypid=getpid();
		clock_gettime( CLOCK_REALTIME, &timestamp);
		starttime=timestamp.tv_sec;
		sops[1].sem_num = 1;
		sops[1].sem_op = -1;
		sops[1].sem_flg = 0; 
		semop(s_id, &sops[1], 1);
		miapos=0;
		my_message.tmessage.hops=0;
		/*------------------------------------------------------------------------
		MIA POSIZIONE E INIZIALIZZAZIONE VARIABILI LOCALI
		-------------------------------------------------------------------------*/
		while(mypid!=MP->PID_USERS[miapos]){
			miapos++;
		}
		randomm=0;
		lmind=0;
		i,m,tempo;
		tot=0;
		waitgen.tv_sec=0;
		/*------------------------------------------------------------------------
		CODICE PROCESSO UTENTE ITERATO
		-------------------------------------------------------------------------*/
		while(limit<=SO_RETRY){
	

			mybudget=MP->BUDGET_USERS[miapos];
			signal(SIGUSR1,handle_signal);
			signal(SIGTERM,handle_signal);
			
			if(pidcontrollo==mypid){
				if(mybudget<=2){
					MP->BUDGET_USERS[miapos]=mybudget;
					limit++;
				}else{	
					printf("sto facendo la transazione su richiesta..\n ");
					tot=(rand() % mybudget-1)+2;
					mybudget=mybudget-tot;
					MP->BUDGET_USERS[miapos]=mybudget;
					if ((SO_REWARD*tot/100)>=1)
						my_message.tmessage.reward=(int)(SO_REWARD*tot/100.0); 
					else 
						my_message.tmessage.reward=1;
					my_message.tmessage.quantita=tot-my_message.tmessage.reward;
					clock_gettime( CLOCK_REALTIME, &timestamp);
					my_message.tmessage.timestamp=timestamp.tv_sec-starttime;
					i = rand() % MP->numeronodi;
					my_message.mtype=MP->PID_NODES[i];
					m = rand() % SO_USERS_NUM;
					my_message.tmessage.sender=mypid;
					my_message.tmessage.receiver=MP->PID_USERS[m];
					limit=0;
					if (msgsnd(q_id, &my_message, MSG_SIZE, 0)==-1){
						MP->BUDGET_USERS[miapos]=mybudget+tot;
						limit++;
					}
					pidcontrollo=0;

				}
			}
			mybudget=MP->BUDGET_USERS[miapos];
			while(lmind<=LM->posizione){

				for(k=0; k < SO_BLOCK_SIZE; k++ ){
					if(LM->t[lmind][k].receiver==mypid){
						mybudget = mybudget + LM->t[lmind][k].quantita;
					}
				}

				lmind++;
			}

			if(mybudget<=2){
				MP->BUDGET_USERS[miapos]=mybudget;
				limit++;
			}
			else{	
				tot=(rand() % mybudget-1)+2;
				mybudget=mybudget-tot;
				MP->BUDGET_USERS[miapos]=mybudget;
				if ((SO_REWARD*tot/100)>=1)
					my_message.tmessage.reward=(int)(SO_REWARD*tot/100.0); 
				else 
					my_message.tmessage.reward=1;
				my_message.tmessage.quantita=tot-my_message.tmessage.reward;
				clock_gettime( CLOCK_REALTIME, &timestamp);
				my_message.tmessage.timestamp=timestamp.tv_sec-starttime;
				i = rand() % MP->numeronodi;
				my_message.mtype=MP->PID_NODES[i];
				m = rand() % SO_USERS_NUM;
				my_message.tmessage.sender=mypid;
				my_message.tmessage.receiver=MP->PID_USERS[m];
				limit=0;


				if (msgsnd(q_id, &my_message, MSG_SIZE, 0)==-1){
					MP->BUDGET_USERS[miapos]=mybudget+tot;
					limit++;
				}
				waitgen.tv_nsec = (rand() % (SO_MAX_TRANS_GEN_NSEC-SO_MIN_TRANS_GEN_NSEC))+SO_MIN_TRANS_GEN_NSEC;
				nanosleep(&waitgen,NULL);

			}
		}
		

		/*printf("------>UTENTE MORTO PREMATURAMENTE\n");*/
		MP->DEAD_USERS[miapos]=1;
		exit(EXIT_SUCCESS); 


	}

