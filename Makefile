# MPLAB IDE generated this makefile for use with GNU make.
# Project: 30F4011_EMAG_rev4.mcp
# Date: Mon Sep 08 21:27:33 2014

AS = pic30-as.exe
CC = pic30-gcc.exe
LD = pic30-ld.exe
AR = pic30-ar.exe
HX = pic30-bin2hex.exe
RM = rm

./hex/30F4011_EMAG_rev4.hex : ./hex/30F4011_EMAG_rev4.cof
	$(HX) ".\hex\30F4011_EMAG_rev4.cof"

./hex/30F4011_EMAG_rev4.cof : spwm.o full_bridge_inv_spwm_rev4.o DataEEPROM.o
	$(CC) -mcpu=30F4011 "spwm.o" "full_bridge_inv_spwm_rev4.o" "DataEEPROM.o" "C:\Program Files (x86)\Microchip\MPLAB C30\lib\libp30F4011-coff.a" "C:\Program Files (x86)\Microchip\MPLAB C30\lib\libp30F4011-elf.a" "C:\Program Files (x86)\Microchip\MPLAB C30\lib\libdsp-coff.a" -o"./hex\30F4011_EMAG_rev4.cof" -Wl,-L"C:\Program Files (x86)\Microchip\MPLAB C30\lib",--script="C:\Program Files (x86)\Microchip\MPLAB C30\support\gld\p30f4011.gld",--defsym=__MPLAB_BUILD=1,-Map="./hex\30F4011_EMAG_rev4.map",--report-mem

spwm.o : c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/p30F4011.h spwm.h spwm.c
	$(CC) -mcpu=30F4011 -x c -c "spwm.c" -o"spwm.o" -g -Wall

full_bridge_inv_spwm_rev4.o : DataEEPROM.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/p30f4011.h jslcd.h c:/program\ files\ (x86)/microchip/mplab\ c30/include/stdio.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/p30F4011.h spwm.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/p30f4011.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/p30fxxxx.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/peripheral_30F_24H_33F/adc10.h c:/program\ files\ (x86)/microchip/mplab\ c30/include/math.h c:/program\ files\ (x86)/microchip/mplab\ c30/include/yvals.h c:/program\ files\ (x86)/microchip/mplab\ c30/include/stdlib.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/dsp.h c:/program\ files\ (x86)/microchip/mplab\ c30/support/h/p30F4011.h full_bridge_inv_spwm_rev4.c
	$(CC) -mcpu=30F4011 -x c -c "full_bridge_inv_spwm_rev4.c" -o"full_bridge_inv_spwm_rev4.o" -g -Wall

DataEEPROM.o : c:/program\ files\ (x86)/microchip/mplab\ c30/support/inc/p30f4011.inc c:/program\ files\ (x86)/microchip/mplab\ c30/support/inc/p30fxxxx.inc DataEEPROM.s
	$(CC) -mcpu=30F4011 -c "DataEEPROM.s" -o"DataEEPROM.o" -Wa,-g

clean : 
	$(RM) "spwm.o" "full_bridge_inv_spwm_rev4.o" "DataEEPROM.o" ".\hex\30F4011_EMAG_rev4.cof" ".\hex\30F4011_EMAG_rev4.hex"

