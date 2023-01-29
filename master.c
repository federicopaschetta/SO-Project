		
	#include "Utils.h"


	#define NUM_SEMS 3
	static int control=0;
	static int utentimorti=0;


	void handle_signal(int signum) {
	    switch (signum) {
	    	case SIGCHLD:
	    		if (control==0){
	     			/* printf("Utenti morti attualmente: %d \n",utentimorti);*/
	    		}
	    	break;
	    	case SIGALRM:
	      		control=1;
	      		printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			printf("+         RESOCONTO FINALE:                                    +\n");
			printf("+         MOTIVO TERMINAZIONE=fine tempo simulazione           +\n");
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
	 int kilo;
	 int SO_HOPS ;
	 int indexi;
	 int indexj;
	 int indexk;
	 int val;
	 int avvio_rapido;
	 int budgetmassimi[5];
	 pid_t corrispondenti[5];
	 int budgetminimi[5];
	 pid_t corrispondentim[5];
	 int budgetmassiminode[5];
	 pid_t corrispondentinode[5];
	 int budgetminiminode[5];
	 pid_t corrispondentimnode[5];
	    
	    
	    
	      
	    /*------------------------------------------------------------------------
	    
	    
	    C O D I C E        M A I N      M A S T E R
	    
	    -------------------------------------------------------------------------*/

	    
	    
	   int main () {
	    
	    
	    /*------------------------------------------------------------------------
	      INIZIALIZZAZIONE IPC E VARIABILI D'APPOGGIO
	    -------------------------------------------------------------------------*/

		struct timespec tempo ;
		struct timespec rimanente;
	
		struct mpid *MP;
		int q_id;
		int s_id;
		int mp_id;
		int lm_id;
		int tp_id;
		int a,b;
		char *argvs[19];
		char *argv[19];
		struct sembuf sops[3];
		char q_id_str[3*sizeof(s_id)+1];
		char s_id_str[3*sizeof(s_id)+1];
		char mp_id_str[3*sizeof(mp_id)+1];
		char lm_id_str[3*sizeof(lm_id)+1];
		char cSO_USERS_NUM[3*sizeof(SO_USERS_NUM)+1];
		char cSO_NODES_NUM[3*sizeof(SO_NODES_NUM)+1];
		char cSO_RETRY[3*sizeof(SO_RETRY)+1];
		char cSO_REWARD[3*sizeof(SO_REWARD)+1];
		char cSO_TP_SIZE[3*sizeof(SO_TP_SIZE)+1];
		char cSO_MIN_TRANS_GEN_NSEC[3*sizeof(SO_MIN_TRANS_GEN_NSEC)+1];
		char cSO_MAX_TRANS_GEN_NSEC[3*sizeof(SO_MAX_TRANS_GEN_NSEC)+1];
		char cSO_MIN_TRANS_PROC_NSEC[3*sizeof(SO_MIN_TRANS_PROC_NSEC)+1];
		char cSO_MAX_TRANS_PROC_NSEC[3*sizeof(SO_MAX_TRANS_PROC_NSEC)+1];
		char cSO_BUDGET_INIT[3*sizeof(SO_BUDGET_INIT)+1];
		char cSO_SIM_SEC[3*sizeof(SO_SIM_SEC)+1];
		char cSO_FRIENDS_NUM[3*sizeof(SO_FRIENDS_NUM)+1];
		char cSO_HOPS[3*sizeof(SO_HOPS)+1];
		struct msgbuffer {
			long mtype;  
			struct Transazione tmessage;
		};
		struct msgbuffer my_message;

		pid_t PID_MASTER;

		struct LibroMastro *LM;

	    /*------------------------------------------------------------------------
	    DISPOSIZIONE GESTIONE SEGNALI
	    -------------------------------------------------------------------------*/

		signal(SIGALRM, handle_signal);
		signal(SIGCHLD, handle_signal);
	  
	  
	    /*------------------------------------------------------------------------
	    DISPOSIZIONE PRESET
	    -------------------------------------------------------------------------*/

		PID_MASTER = getpid();
		printf("DIGITARE '1' PER PRIMO PRESET,'2' PER SECONDO PRESET,'3' PER TERZO PRESET,'4' PER QUARTO PRESET, altro PER INSERIMENTO MANUALE \n");
		scanf("%d",&avvio_rapido);
		switch (avvio_rapido){
		case 1:
			SO_USERS_NUM=20;
			SO_NODES_NUM=15;
			SO_FRIENDS_NUM=3;
			SO_RETRY=4;
			SO_REWARD=1;
			SO_TP_SIZE=10;
			SO_MIN_TRANS_GEN_NSEC=123456789;
			SO_MAX_TRANS_GEN_NSEC=199999999;
			SO_MIN_TRANS_PROC_NSEC=12345678;
			SO_MAX_TRANS_PROC_NSEC=19999999;
			SO_BUDGET_INIT=500;
			SO_SIM_SEC=5;
			SO_HOPS=1;
		break;
		case 2:
			SO_USERS_NUM=50;
			SO_NODES_NUM=2;
			SO_FRIENDS_NUM=1;
			SO_RETRY=4;
			SO_REWARD=1;
			SO_TP_SIZE=10;
			SO_MIN_TRANS_GEN_NSEC=123456789;
			SO_MAX_TRANS_GEN_NSEC=199999999;
			SO_MIN_TRANS_PROC_NSEC=12345678;
			SO_MAX_TRANS_PROC_NSEC=19999999;
			SO_BUDGET_INIT=400;
			SO_SIM_SEC=20;
			SO_HOPS=1;
		break;
		case 3:
			SO_USERS_NUM=7;
			SO_NODES_NUM=6;
			SO_FRIENDS_NUM=3;
			SO_RETRY=4;
			SO_REWARD=10;
			SO_TP_SIZE=10;
			SO_MIN_TRANS_GEN_NSEC=123456789;
			SO_MAX_TRANS_GEN_NSEC=199999999;
			SO_MIN_TRANS_PROC_NSEC=12345678;
			SO_MAX_TRANS_PROC_NSEC=19999999;
			SO_BUDGET_INIT=600;
			SO_SIM_SEC=10;
			SO_HOPS=1;
		break;
		case 4:
			SO_USERS_NUM=100;
			SO_NODES_NUM=10;
			SO_FRIENDS_NUM=5;
			SO_RETRY=4;
			SO_REWARD=1;
			SO_TP_SIZE=1;
			SO_MIN_TRANS_GEN_NSEC=123456789;
			SO_MAX_TRANS_GEN_NSEC=199999999;
			SO_MIN_TRANS_PROC_NSEC=123456780;
			SO_MAX_TRANS_PROC_NSEC=199999990;
			SO_BUDGET_INIT=700;
			SO_SIM_SEC=30;
			SO_HOPS=1;
		break;

		default:
	 
			do{
			   printf("Inserire Valore di SO_USERS_NUM \n");
			    scanf("%d",&SO_USERS_NUM);}while(SO_USERS_NUM<0);
			do{    
				printf("Inserire Valore di SO_NODES_NUM \n");
			    scanf("%d",&SO_NODES_NUM);}while(SO_NODES_NUM<0);
			do{    
				printf("Inserire Valore di SO_FRIENDS_NUM \n");
			    scanf("%d",&SO_FRIENDS_NUM);}while(SO_FRIENDS_NUM<0||SO_FRIENDS_NUM>SO_NODES_NUM);
			do{    
				printf("Inserire Valore di SO_RETRY \n");
			    scanf("%d",&SO_RETRY);}while(SO_RETRY<1);
			do{    
				printf("Inserire Valore di SO_REWARD \n");
			    scanf("%d",&SO_REWARD);}while(SO_REWARD<0);
			do{    
				printf("Inserire Valore di SO_TP_SIZE \n");
			    scanf("%d",&SO_TP_SIZE);}while(SO_TP_SIZE<0);
			do{    
				printf("Inserire Valore di SO_MIN_TRANS_GEN_NSEC \n");
			    scanf("%d",&SO_MIN_TRANS_GEN_NSEC);}while(SO_MIN_TRANS_GEN_NSEC<0);
			do{    
				printf("Inserire Valore di SO_MAX_TRANS_GEN_NSEC \n");
			    scanf("%d",&SO_MAX_TRANS_GEN_NSEC);}while(SO_MAX_TRANS_GEN_NSEC<0||SO_MIN_TRANS_GEN_NSEC>SO_MAX_TRANS_GEN_NSEC);
			do{    
				printf("Inserire Valore di SO_MIN_TRANS_PROC_NSEC \n");
			    scanf("%d",&SO_MIN_TRANS_PROC_NSEC);}while(SO_MIN_TRANS_PROC_NSEC<0);
			do{    
				 printf("Inserire Valore di SO_MAX_TRANS_PROC_NSEC \n");
			    scanf("%d",&SO_MAX_TRANS_PROC_NSEC);}while(SO_MAX_TRANS_PROC_NSEC<0||SO_MIN_TRANS_PROC_NSEC>SO_MAX_TRANS_PROC_NSEC);
			do{    
				printf("Inserire Valore di SO_BUDGET_INIT \n");
			    scanf("%d",&SO_BUDGET_INIT);}while(SO_BUDGET_INIT<0);
			do{    
				printf("Inserire Valore di SO_SIM_SEC \n");
			    scanf("%d",&SO_SIM_SEC);}while(SO_SIM_SEC<0);
			do{       
				printf("Inserire Valore di SO_HOPS \n");
			    scanf("%d",&SO_HOPS);}while(SO_HOPS<0);
		break;
	    }
	    
	    /*------------------------------------------------------------------------
	    ALARM, CODE DI MESSAGGI, MEMORIE CONDIVISE, SEMAFORI E PASSAGGIO DELLE VARIABILI TRAMITE ARGV
	    -------------------------------------------------------------------------*/

		alarm(SO_SIM_SEC+2);
		q_id = msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL);
		TEST_ERROR;
		s_id = semget(IPC_PRIVATE, NUM_SEMS, 0600);
		TEST_ERROR;
		mp_id=shmget(IPC_PRIVATE, sizeof(struct mpid), 0600);
		TEST_ERROR;
		lm_id=shmget(IPC_PRIVATE, sizeof(struct LibroMastro), 0600);
		TEST_ERROR;
		//tp_id=shmget(IPC_PRIVATE, sizeof(int), 0600);
		TEST_ERROR;
		LM=shmat(lm_id,NULL,0);


		sprintf(q_id_str, "%d", q_id);
		sprintf(mp_id_str, "%d", mp_id);
		sprintf(lm_id_str, "%d", lm_id);
		sprintf(s_id_str, "%d", s_id);
		sprintf(cSO_USERS_NUM, "%d", SO_USERS_NUM);
		sprintf(cSO_NODES_NUM, "%d", SO_NODES_NUM);
		sprintf(cSO_RETRY, "%d", SO_RETRY);
		sprintf(cSO_REWARD, "%d", SO_REWARD);
		sprintf(cSO_TP_SIZE, "%d", SO_TP_SIZE);
		sprintf(cSO_MIN_TRANS_GEN_NSEC, "%d", SO_MIN_TRANS_GEN_NSEC);
		sprintf(cSO_MAX_TRANS_GEN_NSEC, "%d", SO_MAX_TRANS_GEN_NSEC);
		sprintf(cSO_MIN_TRANS_PROC_NSEC, "%d", SO_MIN_TRANS_PROC_NSEC);
		sprintf(cSO_MAX_TRANS_PROC_NSEC, "%d", SO_MAX_TRANS_PROC_NSEC);
		sprintf(cSO_BUDGET_INIT, "%d", SO_BUDGET_INIT);
		sprintf(cSO_SIM_SEC, "%d", SO_SIM_SEC);
		sprintf(cSO_FRIENDS_NUM, "%d", SO_FRIENDS_NUM);
		sprintf(cSO_HOPS, "%d", SO_HOPS);

		semctl(s_id, 2, SETVAL, 1);
		
		LM->posizione=-1;


		sops[1].sem_num = 1;
		sops[1].sem_op =  SO_USERS_NUM;
		sops[1].sem_flg = 0;
		sops[0].sem_num = 0; 
		sops[0].sem_op =  SO_NODES_NUM;
		sops[0].sem_flg = 0; 

	    

	    
	    /*------------------------------------------------------------------------
	      ALLOCAZIONE PROCESSI
		-------------------------------------------------------------------------*/

		MP=shmat(mp_id,NULL,0);

		MP->numeronodi=SO_NODES_NUM;    

		argvs[0]="Processo_Nodo";
		argvs[1]= lm_id_str;
		argvs[2]= s_id_str;
		argvs[3]= q_id_str;
		argvs[4] = mp_id_str;
		argvs[5] =cSO_USERS_NUM;
		argvs[6] =cSO_NODES_NUM;
		argvs[7] =cSO_RETRY;
		argvs[8] =cSO_REWARD;
		argvs[9] =cSO_TP_SIZE;
		argvs[10] =cSO_MIN_TRANS_GEN_NSEC;
		argvs[11] =cSO_MAX_TRANS_GEN_NSEC;
		argvs[12] =cSO_MIN_TRANS_PROC_NSEC;
		argvs[13] =cSO_MAX_TRANS_PROC_NSEC;
		argvs[14] =cSO_BUDGET_INIT;
		argvs[15] =cSO_SIM_SEC;
		argvs[16] =cSO_FRIENDS_NUM;
		argvs[17] =cSO_HOPS;
		argvs[18]=NULL;
		for ( indexi = 0; indexi<SO_NODES_NUM; indexi++) {

			MP->BUDGET_NODES[indexi]=0; 

			switch(val= fork()) {
				case -1:
				    perror("Fork failed");
				    exit(1);
				case 0:
				    execve("./Processo_Nodo",argvs,NULL);
				default:

				    MP->PID_NODES[indexi]=val;
				break;
			}



		}

		for (indexi=0;indexi<SO_NODES_NUM;indexi++){
			for (indexj=0;indexj<=SO_FRIENDS_NUM;indexj++){
			  srand(PID_MASTER+indexi);
			  MP->FRIENDS[indexi][indexj]=MP->PID_NODES[rand() % SO_NODES_NUM];
			}
		}


		argv[0]="Processo_Utente";
		argv[1] = mp_id_str;
		argv[2] =lm_id_str;
		argv[3] =s_id_str;
		argv[4]= q_id_str;
		argv[5] =cSO_USERS_NUM;
		argv[6] =cSO_NODES_NUM;
		argv[7] =cSO_RETRY;
		argv[8] =cSO_REWARD;
		argv[9] =cSO_TP_SIZE;
		argv[10] =cSO_MIN_TRANS_GEN_NSEC;
		argv[11] =cSO_MAX_TRANS_GEN_NSEC;
		argv[12] =cSO_MIN_TRANS_PROC_NSEC;
		argv[13] =cSO_MAX_TRANS_PROC_NSEC;
		argv[14] =cSO_BUDGET_INIT;
		argv[15] =cSO_SIM_SEC;
		argv[16] =cSO_FRIENDS_NUM;
		argv[17] =cSO_HOPS;
		argv[18] = NULL;
		for ( indexj = 0; indexj<SO_USERS_NUM; indexj++) {
			MP->BUDGET_USERS[indexj]=SO_BUDGET_INIT;
			switch(val = fork()) {
				case -1:
				    perror("Fork failed");
				    exit(EXIT_FAILURE);
				case 0:

				execve("./Processo_Utente",argv,NULL);
				  break;

				default:
				    MP->PID_USERS[indexj]=val;
				break;
			      
		}
			}

			
		shmctl(mp_id, IPC_RMID, NULL);
		shmctl(lm_id, IPC_RMID, NULL);
		printf("pronti\n");
		sleep(1);
		printf("via\n");
		semop(s_id, &sops[0], 1);
		sleep(1);
		semop(s_id, &sops[1], 1);

	    /*------------------------------------------------------------------------
	    INIZIO CODICE ITERATO MASTER
	    -------------------------------------------------------------------------*/
	    
	    while(SO_REGISTRY_SIZE>LM->posizione && control!=1 && utentimorti!=SO_USERS_NUM){
	    
	    /*-----------------------------------------------------------------------
	    VERIFICA E TERMINAZIONE PER UTENTI MORTI
	    -------------------------------------------------------------------------*/
	    
		    for(indexi=0,utentimorti=0;indexi<SO_USERS_NUM;indexi++){
		    	utentimorti=utentimorti+MP->DEAD_USERS[indexi];
		    }
		    if(utentimorti==SO_USERS_NUM){
		    	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				printf("+         RESOCONTO FINALE:                                    +\n");
				printf("+         MOTIVO TERMINAZIONE= Tutti gli utenti sono morti     +\n");
				for (indexk=0; indexk<SO_USERS_NUM; indexk++){
		  			kill(MP->PID_USERS[indexk],SIGTERM);
				}
			
				for (indexk=0; indexk<MP->numeronodi; indexk++){
					kill(MP->PID_NODES[indexk],SIGTERM);
				}	
				semctl(s_id, 0, IPC_RMID);
				
				
				printf("+         BUDGET UTENTI:                                       +\n");
				for (indexk=0; indexk<SO_USERS_NUM; indexk++){
					printf("+  - Utente %d ha budget %d                                    \n",MP->PID_USERS[indexk],MP->BUDGET_USERS[indexk]);
				}

				printf("+      BUDGET NODI E NUMERO DI TRANSAZIONI IN TRANSACTION POOL: +\n");;
				for (indexk=0; indexk<MP->numeronodi; indexk++){
					printf("+  - Nodo %d ha budget %d  e %d transazioni in tp               \n",MP->PID_NODES[indexk],MP->BUDGET_NODES[indexk],MP->TP_REMAINING[indexk]);
				}
				
				printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
				exit(EXIT_SUCCESS);
		    }
	    
	    
	    
	    
	    /*------------------------------------------------------------------------
	    GESTIONE CREZIONE PROCESSI PER TP PIENE
	    -------------------------------------------------------------------------*/

		if (msgrcv(q_id, &my_message, MSG_SIZE, getpid(), 0)!=-1){
			MP->BUDGET_NODES[MP->numeronodi]=0; 
			switch(val = fork()) {
				case -1:
					perror("Fork failed");
					exit(1);
				case 0:
					printf("Nodo creato\n");
					execve("./Processo_Nodo",argvs,NULL);
				default:
					MP->PID_NODES[MP->numeronodi]=val;
					MP->numeronodi++;
					for (indexi=0;indexi<MP->numeronodi;indexi++){
						for (indexj=0;indexj<=SO_FRIENDS_NUM;indexj++){
						 	srand(PID_MASTER+indexi);
							MP->FRIENDS[indexi][indexj]=MP->PID_NODES[rand() % MP->numeronodi];
						}
					}

					printf("------>NODO %d CREATO",val);
					my_message.mtype = val;
					if (msgsnd(q_id, &my_message, MSG_SIZE, 0)==-1)
					TEST_ERROR;
					
				break;
			}
		}
	    /*------------------------------------------------------------------------
	     STAMPA OGNI SECONDO
	    -------------------------------------------------------------------------*/
		printf("----------------------------------------------------------------\n");

		if(SO_USERS_NUM>10){

			for(indexi=0;indexi<5;indexi++){
				budgetmassimi[indexi]=-1;
			}
			for (indexi = 0; indexi<SO_USERS_NUM; indexi++) {
					for( kilo = 0; kilo<5; kilo++) {
						if(MP->BUDGET_USERS[indexi]<=budgetmassimi[kilo]) {
						break;
					}
				}
				for(indexk = 1; indexk<kilo && indexk<5; indexk++) {
					budgetmassimi[indexk-1] = budgetmassimi[indexk];
					corrispondenti[indexk-1]= corrispondenti[indexk];
				}
				if(kilo>0) {
					budgetmassimi[kilo-1] = MP->BUDGET_USERS[indexi];
					corrispondenti[kilo-1]= MP->PID_USERS[indexi];
				}   
			}
			for( indexi=0;indexi<5;indexi++){
				budgetminimi[indexi]=123456789;
			}
			for (indexi = 0; indexi<SO_USERS_NUM; indexi++) {
				for(kilo = 0; kilo<5; kilo++) {
					if(MP->BUDGET_USERS[indexi]>=budgetminimi[kilo]) {
					break;
				}
				}
				for(indexk= 1; indexk<kilo && indexk<5; indexk++) {
					budgetminimi[indexk-1] = budgetminimi[indexk];
					corrispondentim[indexk-1]= corrispondentim[indexk];
				}
				if(kilo>0) {
					budgetminimi[kilo-1] = MP->BUDGET_USERS[indexi];
					corrispondentim[kilo-1]= MP->PID_USERS[indexi];
				}   
			}
			
			for (indexk=4; indexk>=0; indexk--){
			printf("- Utente %d ha budget %d\n",corrispondenti[indexk],budgetmassimi[indexk]);

			}
			for (indexk=4; indexk>=0; indexk--){
				printf("- Utente %d ha budget %d\n",corrispondentim[indexk],budgetminimi[indexk]);
			}
			
			
		}else{
			
			for(indexk=0;indexk<SO_USERS_NUM;indexk++){
				printf("- Utente %d ha budget %d\n",MP->PID_USERS[indexk], MP->BUDGET_USERS[indexk]);
			}
		
		}
		
		
		
		
		
		
		
		
		if(SO_NODES_NUM>10){

			for( indexi=0;indexi<5;indexi++){
				budgetmassiminode[indexi]=-1;
			}
			for (indexi = 0; indexi<SO_NODES_NUM; indexi++) {
				for( kilo= 0; kilo<5; kilo++) {
					if(MP->BUDGET_NODES[indexi]<=budgetmassiminode[kilo]) {
					break;
					}
				}
				for(indexk = 1; indexk<kilo && indexk<5; indexk++) {
					budgetmassiminode[indexk-1] = budgetmassiminode[indexk];
					corrispondentinode[indexk-1]= corrispondentinode[indexk];
				}
					if(kilo>0) {
						budgetmassiminode[kilo-1] = MP->BUDGET_NODES[indexi];
						corrispondentinode[kilo-1]= MP->PID_NODES[indexi];
					}   
			}
			for( indexi=0;indexi<5;indexi++){
				budgetminiminode[indexi]=123456789;
			}
			for (indexi = 0; indexi<SO_NODES_NUM; indexi++) {
				for(kilo= 0; kilo<5; kilo++) {
					if(MP->BUDGET_NODES[indexi]>=budgetminiminode[kilo]) {
					break;
					}
				}
				for(indexk = 1; indexk<kilo && indexk<5; indexk++) {
					budgetminiminode[indexk-1] = budgetminiminode[indexk];
					corrispondentimnode[indexk-1]= corrispondentimnode[indexk];
				}
				if(kilo>0) {
					budgetminiminode[kilo-1] = MP->BUDGET_NODES[indexi];
					corrispondentimnode[kilo-1]= MP->PID_NODES[indexi];
				}   
			}
			for (indexk=4; indexk>=0; indexk--){
				printf("- Nodo %d ha budget %d\n",corrispondentinode[indexk],budgetmassiminode[indexk]);

			}
			for (indexk=4; indexk>=0; indexk--){
				printf("- Nodo %d ha budget %d\n",corrispondentimnode[indexk],budgetminiminode[indexk]);

			}
		
		}else{
		
			for(indexk=0;indexk<SO_NODES_NUM;indexk++){
				printf("- Nodo %d ha budget %d\n",MP->PID_NODES[indexk], MP->BUDGET_NODES[indexk]);
			}
		
		
		}



		

		
		


		printf("----------------------------------------------------------------\n");


		tempo.tv_sec = 1;
		tempo.tv_nsec = 0;
		rimanente.tv_sec = 0;
		rimanente.tv_nsec = 0;
		nanosleep(&tempo, &rimanente);

		while(rimanente.tv_nsec > 0){
			switch (errno) {
					case EINTR:
						tempo.tv_sec = rimanente.tv_sec;
						tempo.tv_nsec = rimanente.tv_nsec;
						rimanente.tv_sec = 0;
						rimanente.tv_nsec = 0;
						nanosleep(&tempo,&rimanente);
						break;
					case EINVAL:

						break;
					default:
						perror("nanosleep");
			}
		}
	}
	 
	  
	  	    /*------------------------------------------------------------------------
	    RESOCONTO FINALE
	    -------------------------------------------------------------------------*/
	       
		if(SO_REGISTRY_SIZE<=LM->posizione && control==0 && utentimorti!=SO_USERS_NUM){
		 
			printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			printf("+         RESOCONTO FINALE:                                    +\n");
			printf("+         MOTIVO TERMINAZIONE= libro mastro pieno              +\n");
		}
		

		printf("+         NUMERO BLOCCHI NEL LIBRO MASTRO: %d                    \n",LM->posizione);
		printf("+         NUMERO UTENTI TERMINATI PREMATURAMENTE: %d             \n",utentimorti);

		for (indexk=0; indexk<SO_USERS_NUM; indexk++){
	  		kill(MP->PID_USERS[indexk],SIGTERM);
		}
		
		for (indexk=0; indexk<MP->numeronodi; indexk++){
			kill(MP->PID_NODES[indexk],SIGTERM);
		}
		semctl(s_id, 0, IPC_RMID);
		
		
		printf("+         BUDGET UTENTI:                                       +\n");
		for (indexk=0; indexk<SO_USERS_NUM; indexk++){
			printf("+  - Utente %d ha budget %d                                    \n",MP->PID_USERS[indexk],MP->BUDGET_USERS[indexk]);
	        }

		printf("+      BUDGET NODI E NUMERO DI TRANSAZIONI IN TRANSACTION POOL: +\n");;
		for (indexk=0; indexk<MP->numeronodi; indexk++){
			printf("+  - Nodo %d ha budget %d  e %d transazioni in tp               \n",MP->PID_NODES[indexk],MP->BUDGET_NODES[indexk],MP->TP_REMAINING[indexk]);
	        }
		/*for (int a=0;a<SO_REGISTRY_SIZE;a++){
			for (int b=0;b<SO_BLOCK_SIZE;b++){
			printf("Utente:transazione seguente timestamp %d sender %d receiver %d quantita %d reward %d\n",LM->t[a][b].timestamp,LM->t[a][b].sender,LM->t[a][b].receiver,LM->t[a][b].quantita,LM->t[a][b].reward);


			}
		}*/
		printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		exit(EXIT_SUCCESS);
		
	    
	}
