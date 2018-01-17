
#ifndef common_h
#define common_h

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<signal.h>

#define T 1
#define F 0
#define QUIT 0
#define START 1
#define STOP 2
#define ELAPSED 3
#define TENS 4
#define UNITS 5
#define OFF 0
#define ON 1
#define TENSLED_INFO 6
#define TENSLED_COLOR 7
#define UNITSLED_INFO 8
#define UNITSLED_COLOR 9

typedef int boolean;

boolean stop;//Variabile utilizzata per fermare il Counter
boolean print;//Variabile per gestire la corretta esecuzione dello Stop

/*Struct utilizzata per scrivere sui txt e mandare in output a terminale 
 * le opzioni dei singoli Led*/
typedef struct typeLed{
	boolean status;
	char color[15];
	int posix;
	int var;
	char nome[15];
	FILE *fp;
}typeLed;


/*Funzione che gestisce un segnale SIGUSR1 al fine di indicare ai processi 
 il momento della loro riesecuzione*/
void sigHandler(int sig);

/*Funzione che gestisce un segnale al fine di indicare ai processi 
 il momento della loro riesecuzione, in particolare chiudere i processi prima del quit*/
void sigHandler2(int sig);

/*Funzione che reinizializza la matrice che gestisce le stringhe di input*/
void init(char listArgument[4][15]);

/*Data una stringa di input ritorna l'azione da compiere*/
int hashingOption(char listArgument[4][15], int argc);

/*Funzione che setta il correto colore del Led(ANSI)*/
void useColor(char colorTens[8][10],char colorUnits[8][10],char actualTens[8][10],char actualUnits[8][10]);

/*Funzione che setta la posizione dei Led accessi*/
void setLed(char statusTens[8][10],char statusUnits[8][10],char starsTens[8][10],char starsUnits[8][10]);

/*Funzione che apre e legge i file txt e grazie a useColor e setLed 
permette di avere l'ouput in modalità 7-segmenti su terminale*/
void gestisciElapsed(int mainToElapsed[]);

/*Verifica se il valore inserito è un numero valido*/
boolean isNumber(char *number);

/*Verifica se il valore inserito è un led valido*/
boolean isLed(char *number);

/*Verifica se il valore inserito è un colore valido*/
boolean isColor(char *color);

/*Permette di gestire i colori come stringhe di tre caratteri*/
void hashingColor(char color[],char res[]); 

/*Funzione che dato un valore modifica un array per creare i led a 7 segmenti*/
void setPattern(int s[], int valore);

/*Funzione che crea file.txt per operare sui led tens, nonchè ne gestisce le informazioni principali*/
void gestisciTensLed(int i, int tensLedfd[8][2]);

/*Funzione che crea file.txt per operare sui led units, nonchè ne gestisce le informazioni principali*/
void gestisciUnitsLed(int i, int unitsLedFd[8][2]);

/*Funzione che gestisce le operazioni e i corretti passaggi di informazioni riguardanti i led delle tens*/
void gestioneTens(int mainToTens[]);

/*Funzione che gestisce le operazioni e i corretti passaggi di informazioni riguardanti i led delle units*/
void gestioneUnits(int mainToUnits[]);

/*Funzione che crea le 4 pipe usate nel programma e gestisce i casi di eccezione*/
void creaPipe(int mainToTens[], int mainToUnits[], int mainToCounter[], int mainToElapsed[]);

/*Funzione che crea il counter (start/stop) inviando informazioni tramite le pipe a tens e units */
void creaCounter(pid_t *pid, int mainToTens[], int mainToUnits[], int mainToCounter[]);

#endif
