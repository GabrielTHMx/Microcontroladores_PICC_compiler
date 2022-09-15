//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
#define lcd_data_port getenv("SFR:PORTB")    //Se incluye cuando el LCD no se va a trabajar en el puerto D y se asigna el puerto a usar
#include <lcd.c>                             //#include <lcd.c> por defecto trabaja en el puerto D, si se usa este no se debe agregar la linea de codigo anterior
//Registros
#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
//Funciones
void menu(int contador);
void efecto(int contador);
//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   set_tris_a(0x03);    //RA0 y RA1 como entrada
   set_tris_c(0x00);    //Puerto C como salidas
   
   int pacman[8] =
   {
      0b00001111,    //1
      0b00011111,    //2
      0b00011110,    //3
      0b00011100,    //4
      0b00011110,    //5
      0b00011111,    //6
      0b00001111,    //7
      0b00000000     //8   
   };
   int fantasma[8] =
   {
      0b00011111,    //1
      0b00011111,    //2
      0b00010101,    //3
      0b00011111,    //4
      0b00011111,    //5
      0b00010101,    //6
      0b00010001,    //7
      0b00000000     //8   
   };
   
   int bandera, bandera_1 = 0;
   int contador = 1;
   
   output_c(0x00);
   lcd_init();
   
   lcd_set_cgram_char(0, pacman);
   lcd_set_cgram_char(1, fantasma);
   
   lcd_putc("BIENVENIDO :)");
   delay_ms(500);
   
   lcd_putc('\f');
   menu(contador);
   //Bucle infinito
   while(1){
      if(input(pin_a0) == 1) bandera = 1;
      if(input(pin_a0) == 0 && bandera == 1){
         bandera = 0;
         contador ++;
         if(contador > 5) contador = 1;
         menu(contador); 
         delay_ms(10);
      }
      
      if(input(pin_a1) == 1) bandera_1 = 1;
      if(input(pin_a1) == 0 && bandera_1 == 1){
         bandera_1 = 0;
         efecto(contador); 
      }
       
   }
}
void menu(int contador){
   switch(contador){
      case 0:
         break;
      
      case 1:
         lcd_putc('\f');
         lcd_putc("EFECTO 1");
         delay_ms(10);
         break;
      
      case 2:
         lcd_putc('\f');
         lcd_putc("EFECTO 2");
         delay_ms(10);
         break;
      
      case 3:
         lcd_putc('\f');
         lcd_putc("EFECTO 3");
         delay_ms(10);
         break;
      
      case 4:
         lcd_putc('\f');
         lcd_putc("EFECTO 4");
         delay_ms(10);
         break;
      
      case 5:
         printf(lcd_putc, "\fCARACTER\n ESPECIAL");
         delay_ms(10);
         break;
         
      default:
         break;
   }
}
void efecto(int contador){
   switch(contador){
      case 0:
         break;
      
      case 1:
         output_c(0x01);
         delay_ms(500);
         output_c(0x00);
         break;
      
      case 2:
         output_c(0x02);
         delay_ms(500);
         output_c(0x00);
         break;
      
      case 3:
         output_c(0x04);
         delay_ms(500);
         output_c(0x00);
      break;
      
      case 4:
         output_c(0x08);
         delay_ms(500);
         output_c(0x00);
         break;
         
      case 5:
         lcd_putc('\f');
         lcd_gotoxy(6,1);
         lcd_putc(0);
         lcd_putc(1);
         delay_ms(2000);
         printf(lcd_putc, "\fCARACTER\n ESPECIAL");
         delay_ms(10);
         break;
      
      default:
         break;
   }
}


