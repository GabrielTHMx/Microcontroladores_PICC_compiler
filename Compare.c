/*Se crea una señal cuadrada de 200 us de periodo y un ciclo de trabajo del 30%
   Se usa el modulo CCP en config. COMPARE o comparacion, para poner en alto la señal cuando
   el valor del CCP1 es el 70% del periodo y este sea igual al valor del TIMER 1, a su vez
   el TIMER 1 se resetea o reinicia.
   La señal cambia a bajo cuando el valor del CCP1 es el 30% del periodo y este sea igual al 
   valor del TIMER 1 produciendo también su reseteo.
*/
//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)

//Registros
#byte trisc = 0x87
#byte portc = 0x07

//Variables globales
int estado = 0;

//Interrupciones
#int_ccp1
void ccp1_isr(){
   //Nivel alto de señal
      if(estado == 0){
         bit_set(portc, 2);   //Activo PIN de CCP1
         CCP_1 = 60;          //Valor de CCP1 cambia a 60. 60 es el 30% de 200 us
         estado = 1;
      }
   //Nivel bajo de señal
      else{
         bit_clear(portc, 2);  //Activo PIN de CCP1
         CCP_1 = 140;          //Valor de CCP1 cambia a 140. 140 es el 70% de 200 us
         estado = 0;
      }
}

//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisc = 0x00;
   portc = 0x00;
   //Configuracion del TIMER 1
   //TIMER 1 con preescaler de 1 y una cuenta maxima de 16 bits = 65535 us
   setup_timer_1(T1_INTERNAL);
   
   //Configuracion del CCP1 
   setup_ccp1(CCP_COMPARE_RESET_TIMER);   //CCP en comparacion y reseteo dle TIMER 1
   CCP_1 = 140;                           //CCP1 = 140 us, osea, el 70% de 200 us
   
   //Habilitacion de interrupcion
   enable_interrupts(global);
   enable_interrupts(int_ccp1);
   
   //Bucle infinito
   while(1){
   
   }
}
