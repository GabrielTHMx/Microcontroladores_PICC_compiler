//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
//Registros
#byte trisc = 0x87
#byte portc = 0x07
//Variables globales
int bandera = 0;          //Bandera de transicion
int16 contador = 0;       //contador que llegue hasta 1000 para generar 1000 ms = 1 s
//Interrupciones
#int_timer0
void timer0_isr(){
   //Cada 1000 milisegundos
   contador ++;
   //Cada 1000 ms = 1 s
   if(contador == 940){          //Se debe tomar en cuenta que la config. del TIMER0 no es precisa y además las instrucciones de lenguaje C consumen tiempo de ejecución
      //Cada 1 milisegundo
      if(bandera == 0){
         bit_set(portc, 0);
         bandera = 1;
      }
      else{
         bit_clear(portc, 0);
         bandera = 0;
      }
      contador = 0;
   }
   set_timer0(0);
}

//Funciones
//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisc = 0x00;
   portc = 0x00;
   
   //Configuracion del TIMER 0
   setup_timer_0(RTCC_INTERNAL | RTCC_DIV_4);      //TIMER 0 config. como temporizador interno y preescaler de 4 =1032 us = aprox. 1 ms
   set_timer0(0);                                  //Carga valor inicial de TIMER0, es un valor de 8 bits, puede ir desde 0 hasta 255
   
   //Configuracion de interrupciones
   enable_interrupts(global);                      //Habilitacion global de interrupciones, se activan GIE = 1 y PIE = 1
   enable_interrupts(int_timer0);                  //Habilitacion especifica de TIMER0, bit TMR0IE = 1
   
   //Bucle infinito
   while(1){
   
   }
}

