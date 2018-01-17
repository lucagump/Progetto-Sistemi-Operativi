#include "common.h"


int hashingOption(char listArgument[4][15], int argc){
	if(argc==2 && strcmp("start", listArgument[0])==0 && isNumber(listArgument[1]))
		return 1;
	if(argc==1 && strcmp(listArgument[0], "quit")==0)
		return 0;
	if(argc==1 && strcmp(listArgument[0], "stop")==0)
		return 2;
	if(argc==1 && strcmp(listArgument[0], "elapsed")==0)
		return 3;
	if(argc==1 && strcmp(listArgument[0], "tens")==0)
		return 4;
	if(argc==1 && strcmp(listArgument[0], "units")==0)
		return 5;
	if(strcmp(listArgument[0], "tensled")==0 && strcmp(listArgument[1], "info")==0 && argc==3 && isLed(listArgument[2]))
		return 6;
	if(strcmp(listArgument[0], "tensled")==0 && strcmp(listArgument[1], "color")==0 && argc==4  && isLed(listArgument[2])  && isColor(listArgument[3]))
		return 7;
	if(strcmp(listArgument[0], "unitsled")==0 && strcmp(listArgument[1], "info")==0 && argc==3 && isLed(listArgument[2]))
		return 8;
	if(strcmp(listArgument[0], "unitsled")==0 && strcmp(listArgument[1], "color")==0 && argc==4 && isLed(listArgument[2])  && isColor(listArgument[3]))
		return 9;
	if(strcmp(listArgument[0], "help")==0)
		return -1;	
	else
		return -1;
}

void useColor(char colorTens[8][10],char colorUnits[8][10],char actualTens[8][10],char actualUnits[8][10]){
	int i;
	for(i=1;i<=7;i++){
		
		/*Tens*/
		if(strcmp(colorTens[i],"wht")==0)
			strcpy(actualTens[i],"\x1b[37m");
		if(strcmp(colorTens[i],"red")==0)
			strcpy(actualTens[i],"\x1b[31m");
		if(strcmp(colorTens[i],"blu")==0)
			strcpy(actualTens[i],"\x1b[34m");
		if(strcmp(colorTens[i],"ylw")==0)
			strcpy(actualTens[i],"\x1b[33m");
		if(strcmp(colorTens[i],"grn")==0)
			strcpy(actualTens[i],"\x1b[32m");
		if(strcmp(colorTens[i],"pur")==0)
			strcpy(actualTens[i],"\x1b[35m");
			
		/*Units*/
		if(strcmp(colorUnits[i],"wht")==0)
			strcpy(actualUnits[i],"\x1b[37m");
		if(strcmp(colorUnits[i],"red")==0)
			strcpy(actualUnits[i],"\x1b[31m");
		if(strcmp(colorUnits[i],"blu")==0)
			strcpy(actualUnits[i],"\x1b[34m");
		if(strcmp(colorUnits[i],"ylw")==0)
			strcpy(actualUnits[i],"\x1b[33m");
		if(strcmp(colorUnits[i],"grn")==0)
			strcpy(actualUnits[i],"\x1b[32m");
		if(strcmp(colorUnits[i],"pur")==0)
			strcpy(actualUnits[i],"\x1b[35m");
	}
}

void setLed(char statusTens[8][10],char statusUnits[8][10],char starsTens[8][10],char starsUnits[8][10]){
	int i;
	for(i=1;i<=7;i++){
		if(i==1 || i==4 || i==7){
			if(strcmp(statusTens[i],"ON")==0)
				strcpy(starsTens[i]," *** ");
			else
				strcpy(starsTens[i],"     ");
			
			if(strcmp(statusUnits[i],"ON")==0)
				strcpy(starsUnits[i]," *** ");
			else
				strcpy(starsUnits[i],"     ");
		}
		if(i==2|| i==3 || i==5 || i==6){
			if(strcmp(statusTens[i],"ON")==0)
				strcpy(starsTens[i],"*");
			else
				strcpy(starsTens[i]," ");
			
			if(strcmp(statusUnits[i],"ON")==0)
				strcpy(starsUnits[i],"*");
			else
				strcpy(starsUnits[i]," ");
		}
			
	}
}

void gestisciElapsed(int mainToElapsed[]){
	FILE *fpTens[8];
    FILE *fpUnits[8];
	char title[20];
	int i;
	int e;//Variabile per garantire la corretta lettura
	int res;//Error checking
	char statusTens[8][10];
	char colorTens[8][10];
	char statusUnits[8][10];
	char colorUnits[8][10];
	char actualUnits[8][10];
	char actualTens[8][10];
	char starsTens[8][10];
	char starsUnits[8][10];	
	close(mainToElapsed[1]);
	for(i=1;i<=7;i++){
		sprintf(title,"tens_led_%d.txt",i);
		fpTens[i]=fopen(title,"r");
		if(fpTens[i]==NULL){
			perror("Errore apertura file");
			kill(-(getpgid(getpid())),SIGKILL);//Kill Group
		}
	}
	for(i=1;i<=7;i++){
		sprintf(title,"units_led_%d.txt",i);
		fpUnits[i]=fopen(title,"r");
		if(fpUnits[i]==NULL){
			perror("Errore apertura file");
			kill(-(getpgid(getpid())),SIGKILL);
		}
	}
	while(1){
		if(read(mainToElapsed[0],&e,sizeof(e))){
			if(e==4){
				for(i=1;i<=7;i++){
					res = fscanf(fpTens[i],"%s %s",statusTens[i],colorTens[i]);
					if(res != 2){
						perror("Errore lettura file");
						kill(-(getpgid(getpid())),SIGKILL);
					}
					res =fseek(fpTens[i],0,SEEK_SET);
					if(res != 0){
						printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
						kill(-(getpgid(getpid())),SIGKILL);
					}
					res = fscanf(fpUnits[i],"%s %s",statusUnits[i],colorUnits[i]);
					if(res != 2){
						perror("Errore lettura file");
						kill(-(getpgid(getpid())),SIGKILL);
					}
					res = fseek(fpUnits[i],0,SEEK_SET);
					if(res != 0){
						printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
						kill(-(getpgid(getpid())),SIGKILL);
					}
				}
				useColor(colorTens,colorUnits,actualTens,actualUnits);
				setLed(statusTens,statusUnits,starsTens,starsUnits);
				printf("%s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[1],starsTens[1],actualUnits[1],starsUnits[1]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[6],starsTens[6],actualTens[2],starsTens[2],actualUnits[6],starsUnits[6],actualUnits[2],starsUnits[2]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[6],starsTens[6],actualTens[2],starsTens[2],actualUnits[6],starsUnits[6],actualUnits[2],starsUnits[2]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[6],starsTens[6],actualTens[2],starsTens[2],actualUnits[6],starsUnits[6],actualUnits[2],starsUnits[2]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[7],starsTens[7],actualUnits[7],starsUnits[7]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[5],starsTens[5],actualTens[3],starsTens[3],actualUnits[5],starsUnits[5],actualUnits[3],starsUnits[3]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[5],starsTens[5],actualTens[3],starsTens[3],actualUnits[5],starsUnits[5],actualUnits[3],starsUnits[3]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m   %s%s\x1b[0m\n",actualTens[5],starsTens[5],actualTens[3],starsTens[3],actualUnits[5],starsUnits[5],actualUnits[3],starsUnits[3]);
				printf("%s%s\x1b[0m   %s%s\x1b[0m\n\n",actualTens[4],starsTens[4],actualUnits[4],starsUnits[4]);
			}
		}
	}
}

void sigHandler(int sig){
	stop=T;
}

void sigHandler2(int sig){
	print=F;
}

void init(char listArgument[4][15]){
	int a,b;
	for(a=0;a<4;a++){
		for(b=0;b<15;b++){
			listArgument[a][b]=' ';
		}
	}
}

boolean isNumber(char* number){
	int i=0;
	while(number[i] != '\0'){
		if(number[i] < '0' || number[i] > '9'){
			return F;
		}
		i++;
	}
	i=atoi(number);
	if(i >= 60 || i <= 0)
		return F;
	return T;
}

boolean isLed(char* number){
	if(strlen(number)==1){
		if(number[0]>='1' && number[0]<='7')
			return T;
	}
	return F;
}

boolean isColor(char* color){
	if(strcmp(color,"white")==0)
		return T;
	if(strcmp(color,"red")==0)
		return T;
	if(strcmp(color,"blue")==0)
		return T;
	if(strcmp(color,"yellow")==0)
		return T;
	if(strcmp(color,"green")==0)
		return T;
	if(strcmp(color,"purple")==0)
		return T;
	return F;
}

void hashingColor(char *color, char *res){
	if(strcmp(color,"white")==0)
		strncpy(res,"wht",3);
	if(strcmp(color,"red")==0)
		strncpy(res,"red",3);
	if(strcmp(color,"blue")==0)
		strncpy(res,"blu",3);
	if(strcmp(color,"yellow")==0)
		strncpy(res,"ylw",3);
	if(strcmp(color,"green")==0)
		strncpy(res,"grn",3);
	if(strcmp(color,"purple")==0)
		strncpy(res,"pur",3);
}

void setPattern(int s[], int valore){
	switch(valore){
		case 0:
			s[1]=s[2]=s[3]=s[4]=s[5]=s[6]=1;
			s[7]=0;
		break;
		case 1:
			s[1]=s[7]=s[4]=s[5]=s[6]=0;
			s[2]=s[3]=1;
		break;
		case 2:
			s[1]=s[2]=s[4]=s[5]=s[7]=1;
			s[6]=s[3]=0;
		break;
		case 3:
			s[1]=s[2]=s[4]=s[3]=s[7]=1;
			s[6]=s[5]=0;
		break;
		case 4:
			s[7]=s[2]=s[6]=s[3]=1;
			s[1]=s[5]=s[4]=0;
		break;
		case 5:
			s[1]=s[6]=s[4]=s[3]=s[7]=1;
			s[2]=s[5]=0;
		break;
		case 6:
			s[1]=s[7]=s[3]=s[4]=s[5]=s[6]=1;
			s[2]=0;
		break;
		case 7:
			s[7]=s[4]=s[6]=s[5]=0;
			s[1]=s[2]=s[3]=1;
		break;
		case 8:
			s[1]=s[2]=s[3]=s[4]=s[5]=s[6]=s[7]=1;
		break;
		case 9:
			s[1]=s[7]=s[3]=s[4]=s[2]=s[6]=1;
			s[5]=0;
		break;
	}
}

/*Funzione che crea file.txt per operare sui led tens, nonchè ne gestisce le informazioni principali*/
void gestisciTensLed(int i, int tensLedFd[8][2]){
	typeLed led;
	led.posix=i;
	int res;
	led.status=OFF;
	strcpy(led.color,"red");
	sprintf(led.nome,"tens_led_%d.txt",led.posix);
	int n;
	boolean changedColor;
	char testoFile[15];

	led.fp=fopen(led.nome,"w");
	if(led.fp==NULL){
			perror("Errore apertura file");
			kill(-(getpgid(getpid())),SIGKILL);
	}
	res=fputs("OFF\nred",led.fp);
	if(res == EOF){
		printf("Errore scruttura file: il programma smettera' di funzionare.\n");
		kill(-(getpgid(getpid())),SIGKILL);
	}
	res = fseek(led.fp,0,SEEK_SET);
	if(res != 0){
		printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
		kill(-(getpgid(getpid())),SIGKILL);
	}

	while(1){
		if (n=read(tensLedFd[led.posix][0],&(led.var),sizeof(led.var))) {
			if(led.var == -1){
				if(led.status == OFF)
					printf("STATUS LED %d: OFF\n",led.posix);
				else
					printf("STATUS LED %d: %s\n",led.posix,led.color);
			}
			else if(led.var == -2){
				printf("Informazioni sul led delle decine n. %d\n",led.posix);
				if(led.status == OFF)
					printf("STATUS LED: OFF\n");
				else
					printf("STATUS LED: ON\n");
				printf("ACTUAL COLOR: %s\n\n", led.color);
			}
			
			else{
				if(led.var == -3){
					read(tensLedFd[led.posix][0],led.color,3);
					led.var=led.status;
					changedColor=T;
				}
				if(led.var != led.status || changedColor){
					if(led.var == OFF){
						led.status=OFF;
						sprintf(testoFile,"OFF \n%s",led.color);
						res=fputs(testoFile,led.fp);
						if( res == EOF){
							printf("Errore scrittura file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}
						res=fseek(led.fp,0,SEEK_SET);
						if(res != 0){
							printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}

					}	
					else{
						led.status=ON;
						sprintf(testoFile,"ON  \n%s",led.color);
						res=fputs(testoFile,led.fp);
						if( res == EOF){
							printf("Errore scrittura file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}
						res=fseek(led.fp,0,SEEK_SET);
						if(res != 0){
							printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}
					}
				changedColor=F;
				}
			}
		}	
	}
}

void gestisciUnitsLed(int i, int unitsLedFd[8][2]){
	typeLed led;
	led.posix=i;
	int res;
	led.status=OFF;
	strcpy(led.color,"red");
	sprintf(led.nome,"units_led_%d.txt",led.posix);
	int n;
	char testoFile[15];
	boolean changedColor;

	led.fp=fopen(led.nome,"w");
	if(led.fp==NULL){
			perror("Errore apertura file");
			kill(-(getpgid(getpid())),SIGKILL);
	}
	fputs("OFF\nred",led.fp);
	if(res == EOF){
		printf("Errore scruttura file: il programma smettera' di funzionare.\n");
		kill(-(getpgid(getpid())),SIGKILL);
	}	
	res = fseek(led.fp,0,SEEK_SET);
	if(res != 0){
		printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
		kill(-(getpgid(getpid())),SIGKILL);
	} 

	while(1){
		if (n=read(unitsLedFd[led.posix][0],&(led.var),sizeof(led.var))){
			if( led.var == -1){
				if(led.status == OFF)
					printf("STATUS LED %d: OFF\n",led.posix);
				else
					printf("STATUS LED %d: %s\n",led.posix,led.color);
			}
			else if(led.var == -2){
				printf("Informazioni sul led delle unita' n. %d\n",led.posix);
				if(led.status == OFF)
					printf("STATUS LED: OFF\n");
				else
					printf("STATUS LED: ON\n");
				printf("ACTUAL COLOR: %s\n\n", led.color);
			}
			else{
				if(led.var == -3){
					read(unitsLedFd[led.posix][0],led.color,3);
					led.var=led.status;
					changedColor=T;
				}
				if(led.var != led.status || changedColor){
					if(led.var == OFF){
						led.status=OFF;
						sprintf(testoFile,"OFF \n%s",led.color);
						res=fputs(testoFile,led.fp);
						if(res == EOF){
							printf("Errore scrittura file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}
						res=fseek(led.fp,0,SEEK_SET);
						if(res != 0){
							printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}
					}	
					else{
						led.status=ON;
						sprintf(testoFile,"ON  \n%s",led.color);
						res=fputs(testoFile,led.fp);
						if(res == EOF){
							printf("Errore scrittura file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}
						res=fseek(led.fp,0,SEEK_SET);
						if(res != 0){
							printf("Errore movimento puntatore nel file: il programma smettera' di funzionare.\n");
							kill(-(getpgid(getpid())),SIGKILL);
						}
					}
				changedColor=F;
				}
			}
		}
	}
}

void gestioneTens(int mainToTens[]){
	pid_t pid=getpid();
	int n;
	int i;
	int var;
	int tensLedFd[8][2];
	char color[15];
	int t[8];
	for(i=1;i<=7;i++){
		pipe(tensLedFd[i]);
		pid=fork();
		if(pid == 0){
			close(tensLedFd[i][1]);
			gestisciTensLed(i,tensLedFd);
		}
		else{
			close(tensLedFd[i][0]);
		}
	}
	close(mainToTens[1]);
	while(1){
		if (n=read(mainToTens[0],&var,sizeof(var))){
			//Action: tens
			if(var == -1){
				printf("\nLista led delle decine:\n\n");
				for(i=1;i<=7;i++)
					write(tensLedFd[i][1],&var,sizeof(var));
			}
			//Action: tensled info con led passato in versione hashed
			else if(var < -1 && var >= -11){
				var+=10;
				n=-2;
				write(tensLedFd[var][1],&n,sizeof(n));
			}
			//Action: tensled color con led e colore passati in versione hashed
			else if(var <= -12 && var >= -22){
				read(mainToTens[0],color,3);
				var+=20;
				n=-3;
				write(tensLedFd[var][1],&n,sizeof(n));
				write(tensLedFd[var][1],color,3);	
			}
			//Action: aggiornamento dei led delle decine
			else{
				setPattern(t,var/10);
				for(i=1;i<=7;i++)
					write(tensLedFd[i][1],&t[i],sizeof(t[i]));
			}
		}
	}
}

void gestioneUnits(int mainToUnits[]){
	pid_t pid=getpid();
	int n;
	int i;
	int var;
	int unitsLedFd[8][2];
	int u[8];
	char color[15];
	for(i=1;i<=7;i++){
		pipe(unitsLedFd[i]);
		pid=fork();
		if(pid == 0){
			close(unitsLedFd[i][1]);
			gestisciUnitsLed(i, unitsLedFd);
		}
		else{
			close(unitsLedFd[i][0]);
		}
	}
	close(mainToUnits[1]);
	while(1){
		if (n=read(mainToUnits[0],&var,sizeof(var))){
			//Action: units
		 	if(var == -1){
				printf("\nLista led delle unita':\n\n");
				for(i=1;i<=7;i++)
					write(unitsLedFd[i][1],&var,sizeof(var));
			}
			//Action: unitsled info con led passato in versione hashed
			else if(var < -1 && var >= -11){
				var+=10;
				n=-2;
				write(unitsLedFd[var][1],&n,sizeof(n));
			}
			//Action: unitsled color con led e colore passati in versione hashed
			else if(var <= -12 && var >= -22){
				read(mainToUnits[0],color,3);
				var+=20;
				n=-3;
				write(unitsLedFd[var][1],&n,sizeof(n));
				write(unitsLedFd[var][1],color,3);	
			}
			//Action: aggiornamento dei led delle unità
			else{
				setPattern(u,var%10);
				for(i=1;i<=7;i++)
				write(unitsLedFd[i][1],&u[i],sizeof(u[i]));
			}	
		}
	}
}

void creaPipe(int mainToTens[], int mainToUnits[], int mainToCounter[], int mainToElapsed[]){
	if(pipe(mainToTens) < 0){
		perror("Errore creazione Pipe mainToTens");
		exit(2);
	}
	if(pipe(mainToUnits) < 0){
		perror("Errore creazione Pipe mainToUnits");
		exit(2);
	}
	if(pipe(mainToCounter) < 0){
		perror("Errore creazione Pipe mainToCounter");
		exit(2);
	}
	if(pipe(mainToElapsed) < 0){
		perror("Errore creazione pipe mainToElapsed");
		exit(2);
	}
}

void creaCounter(pid_t *pid, int mainToTens[], int mainToUnits[], int mainToCounter[]){
	int option;
	int time;
	char testoFile[4];
	*pid=fork();
	if(*pid != 0){
		*pid=fork();
		if(*pid != 0){
			close(mainToTens[0]);
			close(mainToUnits[0]);
		}
		else{
			gestioneTens(mainToTens);
		}
	}
	else{
		gestioneUnits(mainToUnits);
	}
	while(1){
		if(read(mainToCounter[0],&option,sizeof(option))){
			if(option == START){
				read(mainToCounter[0],&time, sizeof(time));
				kill(getppid(),SIGUSR1);//comunica che può iniziare a fare start, evita blocchi improvvisi
				
					//time > -1 permette di avere il valore zero con elapsed 
					while(time > -1){
						if(stop==T){
							printf("\nCountdown interrotto a tempo %d\n", time+1);
							stop=F;
							break;
						}
						else{
							write(mainToTens[1],&time,sizeof(time));
							write(mainToUnits[1],&time,sizeof(time));
							sleep(1);
							time=time-1;	
						}
					}
					kill(getppid(),SIGUSR2);
					
				printf("Lavoro terminato!\n\n");
			}
		}
	}
}

