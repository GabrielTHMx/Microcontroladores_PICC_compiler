//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
//Registros
#byte trisb = 0x86
#byte portb = 0x06
#byte trisc = 0x87
#byte portc = 0x07
//Programa principal
void main(){
   //Configuración de perifericos
   //Configuracion de GPIOs
   trisc = 0x00;           //Puerto C como salidas
   bit_set(trisb,3);       //RB3 como entrada
   
   port_b_pullups(1);
   
   byte const display[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};    //Arreglo que contiene los segmentos que deben encender para formar los numeros
   int contador = 0;
   
   //Bucle infinito
   while(1){
      portc = display[contador];    
      if(bit_test(portb, 3) == 0){     //Testea bit 3 de puerto b igual a 0 , osea, RB3 == 0 
         contador ++;
         if(contador > 9) contador = 0;
         delay_ms(300);
      }
   }
}
