
#ifndef rpi_h
#define rpi_h

#include <wiringPi.h>
#include <stdlib.h>
#include <stdio.h>

#define   SDI_U   0   //serial data input (UNITS)
#define   RCLK_U  1   //memory clock input(STCP)
#define   SRCLK_U 2   //shift register clock input(SHCP)
#define   SDI_T   3   //serial data input (TENS)
#define   RCLK_T  4   //memory clock input(STCP)
#define   SRCLK_T 5   //shift register clock input(SHCP)

void gestisciRPI();

/*Verifica con una funzione specifica se è possibili inizializzare il display*/
void initializeCheck(void);

/*Imposta i vari GPIO per il funzionamente dello shiftregister*/
void initDisplay(void);

/*Legge i Led Units nel file txt*/
unsigned int updateUnits(FILE *fpUnits[]);

/*Legge i Led Tens nel file txt*/
unsigned int updateTens(FILE *fpTens[]);

/*Dato un valore di input int permette di settare i led delle unità*/
void shiftUnits(unsigned char dat);

/*Dato un valore di input int permette di settare i led delle decine*/
void shiftTens(unsigned char dat);

#endif
