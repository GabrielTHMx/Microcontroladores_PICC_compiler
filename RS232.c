//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
#include <lcd.c>

//Perifericos
#byte trisb = 0x86
#byte portb = 0x06

//Configuracion USART
//9600 Baudios, usart interno, 8 bits, sin bit de paridad
#use rs232(BAUD = 9600, XMIT = pin_c6, RCV = pin_c7, bits = 8, parity = N)

//Variables globales
char caracter_leido = '0';       //Para guardar el caracter enviado desde el PC
int numero_ascii = 0;            //Para transformar el caracter en numero entero

//Funciones
void modificar_LED(int);

//Interrupciones
#int_rda
void rda_isr()
{
   //Leer caracter
   caracter_leido = getch();     //Leemos el caracter
   
   //Transformar en entero
   numero_ascii = caracter_leido - 48; //Transformamos el caracter leido de ascii a entero
   
   if(numero_ascii >= 1 && numero_ascii <= 8) modificar_LED(numero_ascii);
}


//Programa principal
void main()
{
   //Configuracion de GPIOs
   trisb = 0x00;                 //Puerto B como salidas
   portb = 0x00;                 //Inicializacion de puerto B a 0
   
   //Inicializacion de LCD
   lcd_init();
   
   //Configuracion de interrupciones
   enable_interrupts(global);
   enable_interrupts(int_rda);
   
   //Bucle infinito
   while(1)
   {
      printf(lcd_putc,"\fCaracter recibido\n%c", caracter_leido);
      delay_ms(300);
   }
}

void modificar_LED(int numero)
{
   switch(numero)
   {
      case 1: output_toggle(pin_b0);      //Cambia valor de pin B0
      break;
      case 2: output_toggle(pin_b1);      //Cambia valor de pin B1
      break;
      case 3: output_toggle(pin_b2);      //Cambia valor de pin B2
      break;
      case 4: output_toggle(pin_b3);      //Cambia valor de pin B3
      break;
      case 5: output_toggle(pin_b4);      //Cambia valor de pin B4
      break;
      case 6: output_toggle(pin_b5);      //Cambia valor de pin B5
      break;
      case 7: output_toggle(pin_b6);      //Cambia valor de pin B6
      break;
      case 8: output_toggle(pin_b7);      //Cambia valor de pin B7
      break;
   }
}
