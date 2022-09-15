//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
//Registros
#byte trisc = 0x87
#byte portc = 0x07

byte const display[10] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};
//Variables globales
int contador = 0;
//Interrupciones
#int_timer1
void timer1_isr(){
   set_timer1(3036);    //Inicializa el timer1 en 3036
   contador++;
   if(contador == 10) contador = 0;
}
//Funciones

//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisc = 0x00;
   portc = 0x00;
   //Configuracion de TIMER 1
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_8 );
   /* Calculando del VALOR DE RECARGA; Primero se calcula el tiempo máximo T max
      Se calcula el Tmax segun el presscaler elegido
      T max = 2^16 * preescaler = 2^16 * 8 = 524,288 us = 524.288 ms -> aproximadamente 500 ms
      Por lo tanto, el preescaler = 8 puede generar los 500 ms
      Usando la Fomrula:
      T = (4/Fosc)*Preescaler(2^16-VALOR RECARGA) 
      T = (4/Fosc)*Preescaler(65536-VALOR RECARGA)
      Se despeja el VALOR DE RECARGA:
      VALOR RECARGA = 65536 - [(T*Fosc)/(Preescaler*4)]
      VALOR RECARGA = 65536 - [(500ms * 4MHz)/(8 * 4)]
      VALOR RECARGA = 65536 - 62500 = 3036 */
      set_timer1(3036);          //Colocamos le valor donde el TIMER1 comenzará, puede ir de 0 a 65536
   
   //Configuracion de interrupcion de TIMER 1
   enable_interrupts(global);
   enable_interrupts(int_timer1);
   //Vairables locales
     
   //Bucle infinito
   while(1){
      portc = display[contador];
   }
}
