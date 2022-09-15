//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)

//Registros
#byte trisb = 0x86
#byte portb = 0x06
#byte trisc = 0x87
#byte portc = 0x07

//Variables globales
int banderaRB0, banderaON, contador = 0;

//Interrupciones
#int_ext
void ext_isr(){            //LED INDICADOR DE PAUSA
   banderaRB0 = 1;
   
   while(banderaRB0 == 1){

      if(input(pin_b0) == 0 && banderaON == 0 && banderaRB0 == 1){
         delay_ms(20);
         bit_set(portb, 1);
         banderaON = 1;
         banderaRB0 = 0;
         
         setup_timer_2(T2_DISABLED, 195, 16);       //TIMER 2 desabilitado, guarda el valor donde se quedó y cuando se habilite de nuevo, comenzará desde ahí
      }
      if(input(pin_b0) == 0 && banderaON == 1 && banderaRB0 == 1){
         delay_ms(20);
         bit_clear(portb, 1);
         banderaON = 0;
         banderaRB0 = 0;
         setup_timer_2(T2_DIV_BY_16, 195, 16);
      }
   }
}
#int_timer2
void timer2_isr(){
   contador ++;
   if (contador == 20){                //20 ciclos de 50 ms, osea 1 s
      output_toggle(PIN_C1); 
      contador = 0;
   }
}
//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisb = 0x01;
   portb = 0x00; 
   trisc = 0x00;
   portc = 0x00;
   
   //Configuracion de TIMER2
   //setup_timer_2(modo, periodo, postscaler);
   //modo = 1, 4, 16 : periodo = 0 a 255 : postscaler = 1 a 16
   //T = (4/Fosc) * [Prescaler(Valor_recarga + 1) * postscaler)]
   //Valor_recarga = [(T * Fosc) / (prescaler * postscaler * 4)] - 1
   //Valor_recarga = periodo = [(50 ms * 4MHz) / (16 * 16 * 4)] - 1 = 194.31 = aprox 194 o 195
   setup_timer_2(T2_DIV_BY_16, 195,  16);       //TIME = 16 * 195 * 16 = 49,920 us -> aprox. 50 ms
   
   //Configuracion de Interrupciones
   ext_int_edge(L_TO_H);      //Config. de interrupcion RB0 por flanco de subida
   
   //Inicializacion de interrupciones
   enable_interrupts(global);
   enable_interrupts(int_ext);
   enable_interrupts(int_timer2);
   
   //Variables locales
   
   //Bucle infinito
   while(1){
   
   }
}
