Corso di laurea in Informatica anno accademico 2016/2017
Sistemi Operativi - Progetto 2: Countdown Led
Spallitta Giuseppe 178211
Martinelli Luca 179013

Il progetto è stato diviso in due parti per rendere possibile la compilazione su due piattaforme diverse (Ubuntu e Raspbian).
La cartella src è strutturata nel seguente modo:
- Files "common.c" e "common.h", i quali definisco le funzioni comuni sia su piattaforma Ubuntu che su Raspbian.
- La cartella "Ubuntu" che contiene il file main per eseguire la shell interattiva
- La cartella "Raspbian" contenente i file per gestire le funzioni extra richieste da Raspberry e display, nonchè il main per eseguire la shell interattiva.

Per quanto riguarda la parte Raspberry sono allegati i due datasheet delle componenti utilizzate e una foto di come è stato montato.
Abbiamo deciso di utilizzare lo shiftregister SN74HC595 perchè ci ha reso possibile pilotare un display a 7 segmenti con un singolo input, in modo tale da dividere decine e unità.
I Led vanno da 1 a 7 numerati in senso orario, nelle funzioni per gestire RPI il Led numero 8 (DOT) non viene utilizzato perchè non necessario ai fini del progetto.
I Colori disponibili per i led sono i seguenti: purple, white, red, green, yellow e blue.


L'albero dei processi è cosi composto:

[1] PROCESSO MAIN: gestisce la shell interattiva
 |
 |--- [2] PROCESSO ELAPSED: gestisce la parte grafica per l'apparizione del tempo rimanente in formato simil led digitale.
 |
 |--- [3] PROCESSO RPI (Solo su Raspbian): gestisce Raspberry e i display collegati.
 |
 |--- [4] PROCESSO COUNTER: gestisce il countdown e il suo stop.
 	   |
 	   |--- [5] PROCESSO TENS: gestisce i led delle decine e le loro attività.
 	   |     |
 	   |	 |--- [6 - 12] PROCESSI TENS_LED: gestiscono rispettivamente uno dei led delle decine, le informazioni su colori e status di accensione.
 	   |
	   |--- [13] PROCESSO UNITS: gestisce i led delle unità e le loro attività.
	         |
 	    	 |--- [14 - 20] PROCESSI UNITS_LED: gestiscono rispettivamente uno dei led delle unità, le informazioni su colori e status di accensione.
	   
Alcune implementazioni extra sono:
- La funzione elapsed appare graficamente con una struttura simil led, si adatta anche ai cambi di colore tramite opportune funzioni.
- Quando viene presa da input la stringa per la shell interattiva, essa non è sensitive agli spazi tra le parole (quindi "start 43", "     start     43" vengono letti correttamente).
- Possibilità di compilazione con Raspberry e funzionamento di due display a 7 segmenti.
- Su Raspberry possibilità di attivare opzionalmente i monitor a 7 segmenti con l'opzione "-d"


