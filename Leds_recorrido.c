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
   //Configuración de periféricos
   //Configuración de GPIOs
   trisc = 0x00;
   bit_set(trisb,3);    //RB3 configurado como entrada
   
   port_b_pullups(1);   //Activo resistencias pull ups de Puerto B
   
   int16 rotacion = 1;
   //Bucle infinito
   while(1){
      //Si rc4 es 1 no he pulsado, rotar izquierda
      //Si rc4 es 0 he pulsdado, rotar derecha
      if(bit_test(portb,3) == 1){
         //Rotar izquierda
         portc = rotacion;
         rotacion *= 2;
         if(rotacion == 256) rotacion = 1;
      }
      else{
         //Rotar derecha
         portc = rotacion;
         rotacion /= 2;
         if(rotacion == 0) rotacion = 128;
      }
      delay_ms(200);
   }
}
