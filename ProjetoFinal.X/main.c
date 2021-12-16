/* 
 * File:   main.c
 * Author: Jelmires
 *
 * Created on 13 de Dezembro de 2021, 20:04
 */

#include <pic18f4520.h>
#include "keypad.h"
#include "ssd.h"
#include "lcd.h"
#include "rgb.h"
#include "config.h"
#include "bits.h"
#include "timer.h"
#define VERMELHO 1
#define VERDE 2
#define AZUL 4
#define ON 0x0F
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0

void mesaReset(int *namesa)
{
    int i;
    for(i=0;i<52;i++){
        namesa[i]=0;
    }
}

int sacaCarta(int *valor,int *namesa,unsigned long int cont)
{
    while(namesa[cont%52]){
        timerReset(2*cont%52);
        timerWait();
    }
    if(valor[cont%52]==1){
        lcdChar(49);
        lcdChar(48);
        return 10;
    }
    if(valor[cont%52]>=10){
        lcdChar(49);
        lcdChar(48);
        return 10;
    }
    lcdChar(valor[cont%52]+48);
    return valor[cont%52];
}
/*
 * 
 */
int main(void) {
    rgbInit();
    kpInit();
    lcdInit();
    timerInit();
    int valor[52]={1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,11,11,11,11,12,12,12,12,13,13,13,13};
    int namesa[52]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int soma=0;
    int aposta;
    int max=100;
    int jogando;
    int terminou=0;
    unsigned long int tecla=16;
    unsigned long int cont=0;
    for(;;){
        kpDebounce();
        if(kpRead()!=tecla){
            tecla=kpRead();
            if(bitTst(tecla,0)){if(jogando){jogando=0;lcdCommand(L_CLR);terminou=1;}else{jogando=1;lcdCommand(L_CLR);terminou=0;}}
            if(bitTst(tecla,3)){if((!jogando)&&(aposta>10)){aposta=aposta-10;lcdCommand(L_CLR);lcdChar((aposta / 1000) % 10 + 48);lcdChar((aposta / 100) % 10 + 48);lcdChar((aposta / 10) % 10 + 48);lcdChar(aposta % 10 + 48);}}
            if(bitTst(tecla,1)){if((!jogando)&&(aposta+10<=max)){aposta=aposta+10;lcdCommand(L_CLR);lcdChar((aposta / 1000) % 10 + 48);lcdChar((aposta / 100) % 10 + 48);lcdChar((aposta / 10) % 10 + 48);lcdChar(aposta % 10 + 48);}if(jogando&&soma<21){soma=soma+sacaCarta(valor,namesa,cont);}}
            
        }
        if(jogando){
            rgbColor(AZUL);
            lcdCommand(L_L2);
            lcdChar((soma / 1000) % 10 + 48);
            lcdChar((soma / 100) % 10 + 48);
            lcdChar((soma / 10) % 10 + 48);
            lcdChar((soma / 1) % 10 + 48);
            lcdCommand(L_L1);
        }
        else{
            lcdCommand(L_L2);
            lcdChar((max / 1000) % 10 + 48);
            lcdChar((max / 100) % 10 + 48);
            lcdChar((max / 10) % 10 + 48);
            lcdChar((max / 1) % 10 + 48);
            lcdCommand(L_L1);
            mesaReset(namesa);
            if(terminou){
                if((soma>21)||(soma<18)){
                    rgbColor(VERMELHO);
                    max=max-aposta;
                }else{
                    max=max+2*aposta*((soma/21)+1);
                    rgbColor(VERDE);
                }
                aposta=0;
                soma=0;
                terminou=0;
            }
        }
        cont++;
    }
}

