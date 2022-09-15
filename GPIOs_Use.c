//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)

//#USE FAST_IO(PUERTO)
/*#USE FAST_IO(B)
#USE FAST_IO(C)*/
//#USE STANDARD_IO(PUERTO)
/*#use standard_io(b)
#use standard_io(c)
//#USE FIXED_IO(PUERTO_OUTPUTS=PIN_A0,...)*/
#use fixed_io(b_outputs = )
#use fixed_io(c_outputs = pin_c0, pin_c1, pin_c2, pin_c3, pin_c4, pin_c5, pin_c6, pin_c7) 

//programa principal
void main(){
   //Configuración de periféricos
   /*set_tris_b(0xff);        //Solo para FAST_IO; Puerto b como entradas
   set_tris_c(0x00);        //Solo para FAST_IO; Puerto c como salidas */
   
   port_b_pullups(1);   //Activas resistencias pull ups ddel puerto B
   //Bucle infinito
   while(1){
      if(input(pin_b3) == 0){
         output_c(0xff);
      }
      else{
         output_c(0x00);
      }
   }
}
