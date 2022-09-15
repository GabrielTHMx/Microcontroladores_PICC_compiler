//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
//Registros
#byte trisc = 0x87
#byte portc = 0x07
//Variables globales
int16 duty_cycle = 512;
//Interrupciones

//Programa principal
void main(){
   //Configuracion de PWM
   set_pwm1_duty(duty_cycle);          //Prepara duty_cycle de PWM escribiendo en el CCPR1L y CCP1CON
   
   trisc = 0x00;                       //Hacer el pin del CCP1 como salida limpiando el bit TRISC 2
   
   setup_timer_2(T2_DIV_BY_1, 255, 1); //Prepara el valor del preescaler del TIMER 2 y habilitia el TIMER 2 escribiendo en T2CON
   
   setup_ccp1(CCP_PWM);                //Configuracon del modulo CCP1 para operacion PWM    
   
   //Bucle infinito
   while(1){
   
   }
}
