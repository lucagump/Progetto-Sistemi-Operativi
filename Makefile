#
#Corso di laurea in Informatica anno accademico 2016/2017
#Sistemi Operativi - Progetto 2: Countdown Led
#Spallitta Giuseppe 178211
#Martinelli Luca 179013
#

#definizione variabili
COMPILE=@gcc 
SOURCE?=src/
RPI?=src/raspbian/
UBUNTU?=src/ubuntu/
BUILD?=build/
REMOVE=@rm
MAKE=@make
MAKEDIR=@mkdir
CLEAR=@clear
#variaibili librerie RPI
INCLUDE	= -I/usr/local/include
LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm
#colori
txtred=\033[0;31m
txtgrn=\033[0;32m
txtrst=\033[0m


help:
	$(CLEAR)
	@echo " _________________________________________________________________________________________________ "
	@echo "|${txtred}  _____  ______   _   _   _   _   _____   ___     _____   _     _    _   _ ${txtrst}                      |"
	@echo "|${txtred} |     | |     | | | | | | | | | |     | |   \   |     | | |   | |  | | | |${txtrst}                      |"
	@echo "|${txtred} |   __| |  _  | | | | | | | | | |_   _| |    \  |  _  | | |   | |  | | | |${txtrst}                      |"
	@echo "|${txtred} |  |    | | | | | | | | | \ | |   | |   |  _  \ | | | | | | _ | |  | \ | |${txtrst}                      |"
	@echo "|${txtred} |  |    | | | | | | | | |     |   | |   | |_| | | | | | | |/ \  |  |     |${txtrst}                      |"
	@echo "|${txtred} |  |__  | |_| | | |_| | | |\  |   | |   |     / | |_| | |   _   |  | |\  |${txtrst}                      |"
	@echo "|${txtred} |     | |     | |     | | | | |   | |   |    /  |     | |  / \  |  | | | |${txtrst}                      |"
	@echo "|${txtred} |_____| |_____| |_____| |_| |_|   |_|   |___/   |_____| |_/   \ |  |_| |_|${txtrst}                      |"
	@echo "|_________________________________________________________________________________________________|"
	@echo "|                                                                                                 |"
	@echo "|Corso di laurea in Informatica anno accademico 2016/2017                                         |"
	@echo "|Sistemi Operativi - Progetto 2: Countdown Led 						    	  |"
	@echo "|Spallitta Giuseppe 178211									  |"
	@echo "|Martinelli Luca 179013                                                                           |"
	@echo "|                                                                                                 |"
	@echo "|Leggere il file README per ulteriori informazioni                                                |"
	@echo "|_________________________________________________________________________________________________|"
	@echo "${txtrst}|                                                                                                 |"
	@echo "|${txtgrn}Lista comandi Shell Interattiva:${txtrst}                      	                                          |"                                                                                                              
	@echo "|                                                                                                 |"
	@echo "|${txtred}-make help:${txtrst}${txtgrn} mostra informazioni e comandi del make file${txtrst}                                          |"                                                                                                           
	@echo "|${txtred}-make clean:${txtrst}${txtgrn} cancella cartella build${txtrst}                                                             |"                                                     
	@echo "|${txtred}-make build:${txtrst}${txtgrn} richiama clean e crea cartella \"build\" con file eseguibile all'interno${txtrst}              |"                                                     
	@echo "|${txtred}-make build-rpi:${txtrst}${txtgrn} \"build\" per raspberry pi con incluse le librerie necessarie (wiringPi)${txtrst}          |"                                                     
	@echo "|_________________________________________________________________________________________________|"

#make clean - cancella tutto quello che è all'interno della cartella build, cioè i file di supporto e l'esegubile 
clean:
	$(REMOVE) -f -R $(BUILD)

# make compile - esegue clean, compila il sorgente e mette l'eseguibile per RPI nella cartella build
build-rpi: clean $(SOURCE)common.c $(SOURCE)common.h $(RPI)rpi.c $(RPI)rpi.h $(RPI)countdownMain.c 
	$(MAKEDIR) -p $(BUILD)
	$(COMPILE)$(SOURCE)common.c $(SOURCE)common.h $(RPI)rpi.c $(RPI)rpi.h $(RPI)countdownMain.c  -o $(BUILD)/countdown $(INCLUDE) $(LDFLAGS) $(LDLIBS)

# make compile - esegue clean, compila il sorgente e mette l'eseguibile per Ubuntu nella cartella build
build: clean $(SOURCE)common.c $(SOURCE)common.h $(UBUNTU)countdownMain.c 
	$(MAKEDIR) -p $(BUILD)
	$(COMPILE)$(SOURCE)common.c $(SOURCE)common.h $(UBUNTU)countdownMain.c -o $(BUILD)/countdown 
	