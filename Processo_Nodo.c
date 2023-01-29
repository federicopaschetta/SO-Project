	#include "Utils.h"
	/*------------------------------------------------------------------------
	HANDLER
	-------------------------------------------------------------------------*/
	static int control=0;
	static int invio=0;

	void handle_signal(int signum){
		switch(signum){
			case SIGTERM:
					control=1;
				break;
			case SIGALRM:
					invio=1;
				break;
		}
		
	}
		    
	/*------------------------------------------------------------------------
	
	
	M A I N   P R O C E S S O   N O D O
	
	
	-------------------------------------------------------------------------*/


	int main(int argc, char *argv[]) {



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
		int indexc;
		int msgstat;
		int i;
		int starttime;
		int lm_id;
		int s_id;
		int q_id;
		int mp_id;
		int tp;
		int miapos;
		pid_t mypid;
		struct timespec timestamp;
		struct timespec waitgen;
	
		struct mpid *MP;
		struct Transazione transaction_pool[10000];
		struct LibroMastro *LM;
		struct sembuf sops[3];
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
		lm_id = atoi(argv[1]);
		s_id = atoi(argv[2]);
		q_id = atoi(argv[3]);
		mp_id = atoi(argv[4]);
		mypid=getpid();
		MP= shmat(mp_id, NULL, 0);
		LM = shmat(lm_id, NULL, 0);
		sops[0].sem_num = 0;
		sops[0].sem_op= -1;
		sops[0].sem_flg = 0; 
		semop(s_id, &sops[0], 1);
		my_message.tmessage.hops=0;
		msgstat=0;
		i=0;
		tp=0;
		clock_gettime( CLOCK_REALTIME, &timestamp);
		starttime=timestamp.tv_sec;
		miapos=0;
		/*------------------------------------------------------------------------
		DETERMINAZIONE DI MIAPOS, HANDLER TRANSAZIONI PERIODICHE A AMICI
		-------------------------------------------------------------------------*/
		while(mypid!=MP->PID_NODES[miapos]){
			miapos++;
		}
		
		alarm(SO_SIM_SEC/5);
		/*------------------------------------------------------------------------
		CODICE ITERATO PROCESSO NODO
		-------------------------------------------------------------------------*/
		
		while (control!=1){
			MP->TP_REMAINING[miapos]=tp;
			if (invio==1){
				srand(mypid);
				my_message.tmessage.timestamp= transaction_pool[tp].timestamp;
				my_message.tmessage.sender = transaction_pool[tp].sender;
				my_message.tmessage.receiver = transaction_pool[tp].receiver;
				my_message.tmessage.quantita = transaction_pool[tp].quantita;
				my_message.tmessage.reward = transaction_pool[tp].reward; 
				my_message.mtype = MP->FRIENDS[miapos][rand() % SO_FRIENDS_NUM];
				if (msgsnd(q_id, &my_message, MSG_SIZE, 0)==-1){
					TEST_ERROR;
				}
				tp--;
				invio=0;

				alarm(SO_SIM_SEC/5);
							
			}
			signal(SIGALRM, handle_signal);
			signal(SIGTERM,handle_signal);
			
			i++;
			if (msgrcv(q_id, &my_message, MSG_SIZE, mypid, 0)!=-1 && tp<SO_TP_SIZE){
				transaction_pool[tp].timestamp = my_message.tmessage.timestamp;
				transaction_pool[tp].sender = my_message.tmessage.sender;
				transaction_pool[tp].receiver = my_message.tmessage.receiver;
				transaction_pool[tp].quantita = my_message.tmessage.quantita;
				transaction_pool[tp].reward = my_message.tmessage.reward; 
				tp++;
			}
			else if (msgrcv(q_id, &my_message, MSG_SIZE, mypid, 0)!=-1 && tp>=SO_TP_SIZE){
				if (my_message.tmessage.hops!=SO_HOPS){
					my_message.tmessage.hops++;
					my_message.mtype = MP->FRIENDS[miapos][rand() % SO_FRIENDS_NUM];
					if (msgsnd(q_id, &my_message, MSG_SIZE, 0)==-1){
						TEST_ERROR;
					}
					printf("Nodo %d ha inviato transazione a nodo per tp piena %d\n",mypid,my_message.tmessage.hops);
				}
				else{
					my_message.mtype = getppid();
					if (msgsnd(q_id, &my_message, MSG_SIZE, 0)==-1){
						TEST_ERROR;
					}
					printf("Nodo %d ha inviato transazione a master per tp piena\n",mypid);
				}
				
			}

			if (SO_REGISTRY_SIZE!=LM->posizione){
				
				if(tp>=SO_BLOCK_SIZE && control!=1){
					waitgen.tv_nsec = (rand() % (SO_MAX_TRANS_PROC_NSEC-SO_MIN_TRANS_PROC_NSEC))+SO_MIN_TRANS_PROC_NSEC;
					nanosleep(&waitgen,NULL);
					
					/*LOCK*/
					sops[2].sem_num = 2;			
					sops[2].sem_op = -1;			
					sops[2].sem_flg = 0;			
					semop(s_id, &sops[2], 1);
					if(SO_REGISTRY_SIZE!=LM->posizione){
						int budgetReward=0;
						for (indexc=0;indexc<SO_BLOCK_SIZE-1;indexc++){
							LM->t[LM->posizione+1][indexc].timestamp=transaction_pool[indexc].timestamp;
							LM->t[LM->posizione+1][indexc].sender=transaction_pool[indexc].sender;
							LM->t[LM->posizione+1][indexc].receiver=transaction_pool[indexc].receiver;
							LM->t[LM->posizione+1][indexc].quantita=transaction_pool[indexc].quantita;
							LM->t[LM->posizione+1][indexc].reward=transaction_pool[indexc].reward;

							budgetReward=budgetReward+transaction_pool[indexc].reward;

						}


						clock_gettime( CLOCK_REALTIME, &timestamp);		
						LM->t[LM->posizione+1][SO_BLOCK_SIZE-1].timestamp=timestamp.tv_sec-starttime;
						LM->t[LM->posizione+1][SO_BLOCK_SIZE-1].sender=NODE_SENDER;
						LM->t[LM->posizione+1][SO_BLOCK_SIZE-1].receiver=mypid;					
						LM->t[LM->posizione+1][SO_BLOCK_SIZE-1].quantita=budgetReward;
						MP->BUDGET_NODES[miapos]=MP->BUDGET_NODES[miapos]+budgetReward;
						LM->t[LM->posizione+1][SO_BLOCK_SIZE-1].reward=0;
						LM->posizione++;

					}
					/*UNLOCK*/
					sops[2].sem_num = 2;			
					sops[2].sem_op = 1;			
					sops[2].sem_flg = 0;			
					semop(s_id, &sops[2], 1);
					
					
					
					for (indexc=0;indexc<tp-1;indexc++){
						transaction_pool[indexc] = transaction_pool[SO_BLOCK_SIZE-1+indexc];
					}
					tp=tp-SO_BLOCK_SIZE+1;
					
				}
			}

		}
			
				MP->TP_REMAINING[miapos]=tp;
				
				exit(EXIT_SUCCESS);
	}
