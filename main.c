/***************************************************************************
  FileName:        main.c
  Processor:       ATSAML21G18B
  IDE :            SEGGER ver 4.12
  Created by:      http://strefapic.blogspot.com
 ***************************************************************************/
/*------------------------------------------------------------------*/
/* SPI configuration and sending a character A                      */
/*------------------------------------------------------------------*/
//      
//    Reqired connections :
//     - SCK   - PA05 
//     - MISO  - PA06 
//     - MOSI  - PA04
//     - CS    - not used
//     - LED   - PB11
//
//***************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <saml21.h>
#include "delay.h"

void main(void) {
 /* Initialize the SAM system */
SystemInit();
/*Set pin PB11 direction to Output*/
PORT->Group[1].DIRSET.reg |= PORT_PB11;
/*Start SPI configuration*/
/*GENCTRLn1 --> SRC --> 0x6  / ustaw źródło zegara Generatora nr 1 na OSC16M*/
GCLK->GENCTRL[1].bit.SRC = GCLK_GENCTRL_SRC_OSC16M_Val;
/*GENCTRLn1 --> GENEN --> 1  / włącz Generator nr 1*/
GCLK->GENCTRL[1].bit.GENEN = 1;

/*PCHCTRL17 --> GEN --> 0x1   / podepnij SERCOM_Slow do Generatora nr 1*/
GCLK->PCHCTRL[17].bit.GEN = GCLK_PCHCTRL_GEN_GCLK1_Val;
/*PCHCTRL17 --> CHEN --> 1  / włącz kanał dystrybucji zegara SERCOM Slow*/
GCLK->PCHCTRL[17].bit.CHEN = 1;
/*PCHCTRL18 --> CHEN --> 1  / włącz kanał dystrybucji zegara SERCOM0 Core*/
GCLK->PCHCTRL[18].bit.CHEN = 1;

/*PORTA --> PINCFG4 --> PMUXEN --> 1 / podłącz pin PA04 do modułu PMUX*/
PORT->Group[0].PINCFG[4].bit.PMUXEN = 1;
/*PORTA --> PINCFG5 --> PMUXEN --> 1 / podłącz pin PA05 do modułu PMUX*/
PORT->Group[0].PINCFG[5].bit.PMUXEN = 1;
/*PORTA --> PINCFG6 --> PMUXEN --> 1 / podłącz pin PA06 do modułu PMUX*/
PORT->Group[0].PINCFG[6].bit.PMUXEN = 1;

/*PORTA --> PMUX2 --> PMUXO --> 0x3 / podłącz pin PA05 (nieparzysty pin) do modułu SERCOM0 (literka D)*/
PORT->Group[0].PMUX[2].bit.PMUXO = 0x3;
/*PORTA --> PMUX2 --> PMUXE --> 0x3 / podłącz pin PA04 (parzysty pin) do modułu SERCOM0 (literka D)*/
PORT->Group[0].PMUX[2].bit.PMUXE = 0x3;
/*PORTA --> PMUX3 --> PMUXE --> 0x3 / podłącz pin PA06 (parzysty pin) do modułu SERCOM0 (literka D)*/
PORT->Group[0].PMUX[3].bit.PMUXE = 0x3;

/*CTRLA --> ENABLE --> 0 / wyłącz SPI na czas konfiguracji*/
SERCOM0->SPI.CTRLA.bit.ENABLE = 0;
/*wait for synchro*/
while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
/*CTRLA --> MODE --> 0x3  / włącz tryb SPI MASTER dla SERCOM*/
SERCOM0->SPI.CTRLA.bit.MODE = 0x3;
/*CTRLB --> RXENABLE --> 1 / włącz RX*/
SERCOM0->SPI.CTRLB.bit.RXEN = 1;
/*wait for synchro*/
while(SERCOM0->SPI.SYNCBUSY.bit.CTRLB);
/*CTRLA --> DIPO -->  0x2 / w trybie MASTER przyporządkuj do PAD[2] MISO */
SERCOM0->SPI.CTRLA.bit.DIPO = 0x2;
/*CTRLA --> DOPO -->  0x0 / w trybie MASTER przyporządkuj do PAD[0] MOSI a do PAD[1] CLK*/
SERCOM0->SPI.CTRLA.bit.DOPO = 0;
/*BAUD --> BAUD --> 0 / ustaw na 8 bitach wartość Baud Generator*/
SERCOM0->SPI.BAUD.reg = 0; //for Baud = 0 --> f max = F Core / 2
/*CTRLA --> ENABLE --> 0 / włącz SPI*/
SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
/*wait for synchro*/
while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
/*End  SPI configuration*/

/* Set SysTick */
SysTick_Config(4000000 * 0.1); //0.1s, 4MHz core clock  

   while (1)
   {
               
   }

}

/* Routime Interrupt */
 __attribute__((interrupt)) void SysTick_Handler(void){
     SERCOM0->SPI.DATA.reg = 65; //send character A
     /*wait for transfer complete*/
     while(!SERCOM0->SPI.INTFLAG.bit.TXC);
     /*LED toggle pin PB11*/
     PORT->Group[1].OUTTGL.reg |= PORT_PB11;
}
