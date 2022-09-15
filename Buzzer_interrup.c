//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
#use fast_io(b)
#use fast_io(c)
//Variables globales
int16 tiempo = 0;
int bandera_int =  false;
//Interrupcion externa
#int_ext
void ext_isr()
{  
   tiempo += 1000;
   bandera_int = true;
   while(bandera_int == true){
      if(input(pin_b0) == 0){
         delay_ms(20);
         bandera_int = false;
      }
   } 
}

void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   set_tris_b(0x03); //RB0 y RB1 como entradas
   set_tris_c(0x00); //Puerto C como salida
   
   //Configuracion de la interrupcion externa RB0
   ext_int_edge(L_TO_H);   //Interrupcion por flanco de subida
   
   //Habilitacion de interrupciones
   enable_interrupts(global);  //Ponemos en 1 a los bits GIE y PEIE
   enable_interrupts(int_ext);   //Bit a 1 de interrupcion externa INTE = 1
   
   output_b(0x00);      //Limpio puerto B 
   output_c(0x00);      //Limpio puerto C
   int bandera = 0;
   tiempo = 0;
   //Bucle infinito
   while(1){
      if(input(pin_b1) == 1) bandera = 1;
      if(input(pin_b1) == 0 && bandera == 1)
      {
         output_c(0x01);      //enciende buzzer
         delay_ms(tiempo);
         output_c(0x00);
         bandera = 0;
      }
   }
}

