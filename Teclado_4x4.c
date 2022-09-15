//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
#include <lcd.c>
//Registros
#byte trisb = 0x86
#byte portb = 0x06
//Funciones
//Variables globales
//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisb = 0x0f;    //RB0:RB3 como entradas, RB4:RB7 como salidas
   portb = 0xff
   ;
   lcd_init();
   printf(lcd_putc,"\fTECLADO\nMATRICIAL :P");
   delay_ms(200);
   port_b_pullups(1);
   
   int columna, fila;
   //Bucle infinito
   while(1){
      for(columna = 0; columna < 4; columna ++){
         bit_clear(portb, columna + 4);       //RB4:RB7 recorro los puertos con un 0
         
         for(fila = 0; fila < 4; fila ++){
            if(bit_test(portb, fila) == 0){
               printf(lcd_putc,"\f%d%d", columna, fila);
               delay_ms(200);
            }
         }
         bit_set(portb, columna+4);
      }
   }
}

