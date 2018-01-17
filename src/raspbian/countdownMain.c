#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<signal.h>

#include <wiringPi.h>

#include "../common.h"
#include "rpi.h"

/*Funzione che fa partire il programma permettendo all'utente di interagire tramite una shell interattiva*/
int main(int argc, char* argv[]){
	int mainToTens[2], mainToUnits[2],mainToElapsed[2], mainToCounter[2];
  	pid_t pid;
	char scelta[20];
	int i,res;
	int time;
	int charOption;
	FILE *fp;
	boolean startArgument=F;
	boolean badStart=F;
	boolean usingRPI=F;
	
	/*Inizializzazione: creazione delle pipe, attivazione dei segnali e raggruppament dei processi in un unico gruppo*/
	creaPipe(mainToTens,mainToUnits, mainToCounter, mainToElapsed);
	signal(SIGUSR1,sigHandler);
	signal(SIGUSR2,sigHandler2);
	setpgid(0,getpid());
	
	/*Apertura dei file txt di default ai led, qualora elapsed si attivasse prima che i singoli led inizializzino i loro file*/
	for(i=1;i<=7;i++){
		sprintf(scelta,"tens_led_%d.txt",i);
		fp=fopen(scelta,"w");
		if(fp==NULL){
			perror("Errore apertura file");
			kill(-(getpgid(getpid())),SIGKILL);
		}
		res=fputs("OFF\nred",fp);
		if( res == EOF){
			printf("Errore scrittura file: il programma smetterà di funzionare.\n");
			kill(-(getpgid(getpid())),SIGKILL);
		}
		fclose(fp);
	}
	for(i=1;i<=7;i++){
		sprintf(scelta,"units_led_%d.txt",i);
		fp=fopen(scelta,"w");
		if(fp==NULL){
			perror("Errore apertura file");
			kill(-(getpgid(getpid())),SIGKILL);
		}
		res=fputs("OFF\nred",fp);
		if( res == EOF){
			printf("Errore scrittura file: il programma smetterà di funzionare.\n");
			kill(-(getpgid(getpid())),SIGKILL);
		}
		fclose(fp);
	}
	
	/*Opzione display: usingRpi sara' TRUE solo se l'opzione "-d" è usata nell'argmento di esecuzione*/
	if((charOption = getopt(argc,argv,"d")) != -1){
		if(charOption == 'd'){
			usingRPI=T;
		}
		else{
			printf("Use ./countdown <num>, ./countdown -d or ./countdown -d <num>, where 0 < num < 60, in order to use this program.\n");
			exit(1);
		}
	}
	
	/*Controlli per verificare se viene utilizzara la funzione test o se viene usata una sintassi errata di esecuzione*/
	if(!usingRPI && argc==2){
		if(isNumber(argv[1])){
			startArgument=T;
			time=atoi(argv[1]);
		}
		else badStart=T;
	}
	else if(usingRPI && argc==3){
		if(isNumber(argv[1])){
			startArgument=T;
			time=atoi(argv[1]);
		}
		else if(isNumber(argv[2])){
			startArgument=T;
			time=atoi(argv[2]);
		}
		else badStart=T;
	}
	
	/*Se viene passato più di un argomento o l'argomento passato non è un numero, viene data eccezione.*/
	if(argc > 3 || badStart){
		printf("Use ./countdown, ./countdown <num> where 0 < num < 60, in order to use this program.\n");
		exit(1);	
	}

	/*Creazione di un processo adibito alla corretta esecuzine della funzione elapsed*/
	if((pid=fork())==0){
		gestisciElapsed(mainToElapsed);
	}
	
	/*Se l'opzione -d è attiva, allora viene generato un nuovo processo per la gestione di raspberry*/ 
	if(usingRPI){
		pid=fork();
		if(pid==0){
			gestisciRPI();
		}
	}
	
	/*Ultima fork nel main, crea il processo counter per la gestione del timer; il main serve semplicmente come shell interattiva*/
	if((pid=fork())==0){
		int n;
		close(mainToCounter[1]);
		creaCounter(&pid, mainToTens, mainToUnits, mainToCounter);
	}
	else{
		
		/*Inizializzazione variabili processo main*/
		int action;
		int option;
		char color[3];
		int pointerStart;
		int numberWords;
		int elapser=4;
		int nBytes=256;
		int hashedLed;
		int running;
		stop=F;
		char listArgument[4][15];
		int prova;
		int j;
		boolean foundLetter;
		int end;
		
		close(mainToElapsed[0]);
		close(mainToCounter[0]);

		/*Modalità di compilazione test con numero di secondi in argomento*/
		if(startArgument){
				printf("Start con argomento %d\n",time);
				option=START;
				write(mainToCounter[1],&option,sizeof(option));
				write(mainToCounter[1],&time,sizeof(time));
				print=T;
				while(!stop){};
				stop=F;
		}
		
		do{
			/*Primo passaggio: leggere la stringa da input*/
			init(listArgument);
			char *s = NULL;
			size_t nBytes=0;
			int bytesRead = getline(&s, &nBytes,stdin);
			if(bytesRead == -1){
				perror("Errore lettura da stdin");
				exit(1);
			}
			foundLetter=0;
			numberWords=0;
			if(strlen(s) > 1)
				s[strlen(s)-1]='\0';

			/*Faccio una verifica carattere per carattere*/
			for (i = 0;i<strlen(s);i++){
				
				/*Caso 1: trovo uno spazio dopo le lettere: è finita una parola*/
				if (s[i] == ' ' && foundLetter==1){
					if(i-pointerStart > 10){
						numberWords=5;
						break;
					}
					foundLetter = 0;
					strncpy(listArgument[numberWords-1],s+pointerStart,i-pointerStart);
					listArgument[numberWords-1][i-pointerStart]='\0';
				}
				/*Caso 2: se non h uno spazio, allora veririfico se è la prima lettera che trovo da un pò, 
				poichè ciò indica l'inizio di una nuova parola/numero*/
				else if(s[i] != ' '){
					if (foundLetter == 0){
						numberWords++;
						pointerStart=i;
						/*Interrompo subito il ciclo se ho pù di 4 parole, visto che non ho comandi con così tanti termini*/
						if(numberWords > 4) break;
					}
				foundLetter = 1;
				}
			}
			/*Salvataggio dell'ultima parola in stringa opportuna e chiamata a funzione per verificare il tipo di operazione da seguire*/
			if(numberWords <= 4){
				if(i-pointerStart > 10){
						action=-1;
				}
				else{
					strncpy(listArgument[numberWords-1],s+pointerStart,i-pointerStart);
					listArgument[numberWords-1][i-pointerStart]='\0';
					action=hashingOption(listArgument,numberWords);
				}
			}
			/*Più di 4 argomenti -> manda in ouput l'help*/
			else{
				action=-1;
			}
			/*SHELL INTERATTIVA: vengono gestite le varie opzioni dell'utente.*/
			switch(action){
				case START:
					if(print!=T && isNumber(listArgument[1])){
						option=START;
						time=atoi(listArgument[1]);
						write(mainToCounter[1],&option,sizeof(option));
						write(mainToCounter[1],&time,sizeof(time));
						print=T;
						while(!stop){};
						stop=F;
					}
					else if(print==T){
						printf("Il countdown e' gia attivo!\n");
					}
					else{
						printf("Scegli un numero tra 1 e 60.\n");
					}	
				break;
				case QUIT:
					kill(-(getpid()),SIGKILL);
				break;
				case STOP:
					if(print==T){
						kill(pid,SIGUSR1);
						print=F;
					}
				break;
				case ELAPSED:
					write(mainToElapsed[1],&elapser,sizeof(elapser));
				break;
				case TENS:
					option=-1;
					write(mainToTens[1],&option,sizeof(option));
				break;
				case UNITS:
					option=-1;
					write(mainToUnits[1],&option,sizeof(option));
				break;
				case TENSLED_INFO:
					hashedLed=atoi(listArgument[2])-10;
					write(mainToTens[1],&hashedLed,sizeof(hashedLed));
				break;
				case TENSLED_COLOR:
					hashedLed=atoi(listArgument[2])-20;
					hashingColor(listArgument[3],color);
					write(mainToTens[1],&hashedLed,sizeof(hashedLed));
					write(mainToTens[1],color,3);
				break;
				case UNITSLED_INFO:
					hashedLed=atoi(listArgument[2])-10;
					write(mainToUnits[1],&hashedLed,sizeof(hashedLed));
				break;
				case UNITSLED_COLOR:
					hashingColor(listArgument[3],color);
					hashedLed=atoi(listArgument[2])-20;
					write(mainToUnits[1],&hashedLed,sizeof(hashedLed));
					write(mainToUnits[1],color,3);
				break;
					default:
						printf("\nOpzioni disponibili:\n");
						printf("\x1b[35mstart <sec>:\x1b[0m \x1b[33mavvia un countdown per i secondi indicati (massimo 59). \x1b[0m\n");
						printf("\x1b[35melapsed:\x1b[0m \x1b[33mmostra i secondi rimasti.\x1b[0m\n");
						printf("\x1b[35mstop:\x1b[0m \x1b[33mferma il timer corrente se attivo.\x1b[0m\n");
						printf("\x1b[35mtens:\x1b[0m \x1b[33mmostra lo stato dei led delle decine.\x1b[0m\n");
						printf("\x1b[35munits:\x1b[0m \x1b[33mmostra lo stato dei led delle unita'\x1b[0m\n");
						printf("\x1b[35mtensled info <n>:\x1b[0m \x1b[33mmostra le info del led n-esimo delle decine.\x1b[0m\n");
						printf("\x1b[35mtensled color <n> <color>:\x1b[0m \x1b[33mimposta il colore del led n-esimo delle decine.\x1b[0m\n");
						printf("\x1b[35munitsled info <n>:\x1b[0m \x1b[33mmostra le info del led n-esimo delle unita'.\x1b[0m\n");
						printf("\x1b[35munitsled color <n> <color>:\x1b[0m \x1b[33mimposta il colore del led n-esimo delle unita'.\x1b[0m\n");
						printf("\x1b[35mquit:\x1b[0m \x1b[33mesci dal programma.\x1b[0m\n");

				}
			}while(action != 0);
		}	

	return 0;
}

