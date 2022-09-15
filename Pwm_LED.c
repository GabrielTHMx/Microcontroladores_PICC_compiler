//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
//Registros
#byte trisc = 0x87
#byte portc = 0x07

//Variables globlaes
int16 duty_cycle = 0;
int subir = 1;
int contador = 0;

//Interrupciones
#int_timer2
void timer2_isr(){
/*La interrupcon TIMER 2 se ejecuta cada 4 ms, debido al desbordamiento y se reinicia el TIMER 2
   Esos 4 ms se ejecutan 12 veces generando asi un tiempo de 48 ms, entrando al if de contador == 12
   El duty_cycle se incrementa en 50 y así sucesivamentes hasta hacerlo 20 veces y pasar a decrementar
   el duty_cycle en 50 hasta 0 y repetir el circlo, entonces, finalmente el Tiempo de encendido y apagado 
   es 48 ms * 20 = 960 ms -> aprox 1s de encendido y 1s de apagado*/
   contador ++;
   
   if(contador == 12){
      //Transiscion de subida
      if(subir == 1){
         duty_cycle += 50;
         set_pwm1_duty(duty_cycle);
         
         if(duty_cycle == 1000) subir = 0; //Tardará 20 ciclos para llegar a 1000
      }
      
      //Transicion de bajada
      if(subir == 0){
         duty_cycle -= 50;
         set_pwm1_duty(duty_cycle);
         
         if(duty_cycle == 0) subir = 1;
      }
      
      contador == 0;
   }
}

//Programa principal
void main(){
   //Configuracion de pwm
   set_pwm1_duty(duty_cycle);
   
   trisc = 0x00;
   
   setup_timer_2(T2_DIV_BY_16, 255, 10);   //TIMER 2 produce un desbordamiento a los 4000us = 4 ms
   
   setup_ccp1(CCP_PWM);                   //Puerto CCP1 configurado como PWM
   
   //Habilitacion de interrupciones
   enable_interrupts(global);
   enable_interrupts(int_timer2);
   
   //Bucle infinito
   while(1){
   
   }
}
