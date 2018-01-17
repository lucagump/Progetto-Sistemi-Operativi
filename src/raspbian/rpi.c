#include"rpi.h"

void gestisciRPI(){
    FILE *fpTens[8];
    FILE *fpUnits[8];
	unsigned int a=0,b=0;
	int i;
	char title[20];

    initializeCheck();
	initDisplay();

	for(i=1;i<=7;i++){
		sprintf(title,"tens_led_%d.txt",i);
		fpTens[i]=fopen(title,"r");
		if(fpTens[i] == NULL){
			printf("Errore apertura file '%s': il display smetterà di funzionare.\n",title);
			exit(2);
		}
	}
	for(i=1;i<=7;i++){
		sprintf(title,"units_led_%d.txt",i);
		fpUnits[i]=fopen(title,"r");
		if(fpUnits[i] == NULL){
			printf("Errore apertura file '%s': il display smetterà di funzionare.\n",title);
			exit(2);
		}
	}
	
	while(1){
		a=updateTens(fpTens);
		b=updateUnits(fpUnits);
		shiftTens(a);
		shiftUnits(b);
		usleep(200);
	}
}

void initDisplay(void){
	pinMode(SDI_U, OUTPUT); //make Pin output unit
	pinMode(RCLK_U, OUTPUT); //make Pin output unit
	pinMode(SRCLK_U, OUTPUT); //make Pin output unit

	pinMode(SDI_T, OUTPUT); //make Pin output tens
	pinMode(RCLK_T, OUTPUT); //make Pin output tens
	pinMode(SRCLK_T, OUTPUT); //make Pin output tens

	digitalWrite(SDI_U, 0);
	digitalWrite(RCLK_U, 0);
	digitalWrite(SRCLK_U, 0);

	digitalWrite(SDI_T, 0);
	digitalWrite(RCLK_T, 0);
	digitalWrite(SRCLK_T, 0);
}

unsigned int updateUnits(FILE *fpUnits[]){
	int i,res;
	int val=0;
	char title[20];
	int convert=1;
	char status[10];
	char color[10];

	for(i=1;i<=7;i++){
		do{
			res = fscanf(fpUnits[i],"%s %s",status, color);
		}while(res == -1);
		res=fseek(fpUnits[i],0,SEEK_SET);
		if(res != 0){
			printf("Errore movimento puntatore nel file 'units_led_%d': il display smetterà di funzionare.\n",i);
			exit(2);
		}
		if(status[1]!='F')
			val+=convert;
		convert*=2;	
	}

	return val;
}

unsigned int updateTens(FILE *fpTens[]){
	int i,res;
	int val=0;	
	char title[20];
	int convert=1;
	char status[10];
	char color[10];
	
	for(i=1;i<=7;i++){
		do{
			res = fscanf(fpTens[i],"%s %s",status, color);
		}while(res == -1);
		res =fseek(fpTens[i],0,SEEK_SET);
		if(res != 0){
			printf("Errore movimento puntatore nel file 'tens_led_%d': il display smetterà di funzionare.\n",i);
			exit(2);
		}
		if(status[1]!='F')
			val+=convert;
		convert*=2;
	}
	return val;
}

void shiftUnits(unsigned char dat){
	int i;
	for(i=0;i<8;i++){
		digitalWrite(SDI_U, 0x80 & (dat << i));
		digitalWrite(SRCLK_U, 1);
		delay(1);
		digitalWrite(SRCLK_U, 0);
	}
	digitalWrite(RCLK_U, 1);
	delay(1);
	digitalWrite(RCLK_U, 0);
}

void shiftTens(unsigned char dat){
	int i;

	for(i=0;i<8;i++){
		digitalWrite(SDI_T, 0x80 & (dat << i));
		digitalWrite(SRCLK_T, 1);
		delay(1);
		digitalWrite(SRCLK_T, 0);
	}

	digitalWrite(RCLK_T, 1);
	delay(1);
	digitalWrite(RCLK_T, 0);
}

void initializeCheck(){
	if(wiringPiSetup() == -1){ 
		printf("\nSetup wiringPi fallito!\n");
		exit(2); 
	}
}


