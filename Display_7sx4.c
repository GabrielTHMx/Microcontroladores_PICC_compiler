//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
//Registros
#byte trisa = 0x85
#byte porta = 0x05 
#byte trisb = 0x86
#byte portb = 0x06
#byte trisc = 0x87
#byte portc = 0x07
//Variables globales
byte const display[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67};    //Arreglo que contiene los segmentos que deben encender para formar los numeros
//Funciones
void mostrar_display(int contador, int decenas, int centenas, int millares);
//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisa = 0x00;     //Puerto A configurado como salida
   trisc = 0x00;     //Puerto C configurado como salida
   bit_set(trisb, 3);      //Puerto RB3 configurado como entrada   
   
   porta = 0;     //Inicializando puertos en 0
   portb = 0;
   portc = 0;
   
   port_b_pullups(1);      //Activo resistencias pull ups de Puerto B
   
   int contador, decenas, centenas, millares, bandera = 0;
   
   //Bucle infinito
   while(1){
      for(millares = 0; millares < 10; millares ++){
         for(centenas = 0; centenas < 10; centenas ++){
            for(decenas = 0; decenas < 10; decenas ++){
               for(contador = 0; contador < 10; contador ++){
                  mostrar_display(contador, decenas, centenas, millares);
                  
                  if(bit_test(portb, 3) == 0) bandera = 1;
                  
                  while(bandera == 1){
                     if(bit_test(portb, 3) == 1) bandera = 0;
                     mostrar_display(contador, decenas, centenas, millares);
                  }
               }
            }
         }
      }
   }
}

void mostrar_display(int contador, int decenas, int centenas, int millares){
   porta = 0x03;
   portc = display[contador];
   delay_ms(1);
   porta = 0x02;
   portc = display[decenas];
   delay_ms(1);
   porta = 0x01;
   portc = display[centenas];
   delay_ms(1);
   porta = 0x00;
   portc = display[millares];
   delay_ms(1);
}
