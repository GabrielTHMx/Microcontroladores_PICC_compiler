//Directivas
#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)

#include <lcd.c>
//Registros
#byte trisb = 0x86
#byte portb = 0x06
#byte trisc = 0x87
#byte portc = 0x07

#bit rbif_flag = 0x0b.0   //Bit RBIF (flag) del registro INTCON para activacion de interrupcion por nivel <rb7:<rb4>

#define derechas 0         //A palabra "derechas" se le asigna valor de '0'
#define izquierdas 1       //A palabra "izquierdas" se le asigna valor de '1'

//Variables globales
int delaytime_ms = 15;    //Para la velocidad del motor, dado como recomendación por el fabricante entre cada step
int sentido = derechas;   //Para saber el snetido de giro; derechas = 0, izquierdas = 1
int flanco_bajada = 0;    //Flag para saber si interrupcion se ha producido por flanco de subia o bajada

int mando[4] = {0, 0, 0, 0};  //{Derecha, lento, Izquierda, rapido}
//Funciones
void actualizar_estado();
void disminuir_velocidad();
void aumentar_velocidad();
//Interrupciones
#int_rb
void rb_isr(){
   delay_ms(20);     //Eliminar rebotes de boton
   //Comprobamos que boton fue pulsado entre RB4 a RB7
   for(int i = 4; i <= 7; i++){
      if(bit_test(portb, i) == 1){
          mando[i - 4] = 1;          //Se guarda opcion elegida entre RB4 a RB7 y se pone a 1
          flanco_bajada = 1;         //Flag activada
      }
      else mando[i - 4] = 0;
   }
   if(flanco_bajada == 1){          //Si es flanco de bajada se realiza la orden
      actualizar_estado();
      flanco_bajada = 0;            //Reseteamos flag 
   }
}

//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisc = 0xf0;
   portc = 0b00001100;  //Situacion inicial de step motor
   //Configuracion de LCD
   lcd_init();
   //Configuracion de interrupcion RBIF
   rbif_flag = 0;    //Bit RBIF se inicializa en 0, ya que tiene un valor desconocido en su registro INTCON
   //Habilitacion de interrupciones
   enable_interrupts(global);
   enable_interrupts(int_rb);
   
   //Variables locales
   int const half_step_derechas[8] = {0b00001100, 0b00001000, 0b00000110, 0b00000100, 0b00000011, 0b00000001, 0b00001001, 0b00001000};
   //int const full_step_derechas[4] = {0b00001100, 0b00000110, 0b00000011, 0b00001001};
   //int const wave_drive_derechas[4] = {0b00001000, 0b00000100, 0b00000010, 0b00000001};
   int i = 0;
   
   //Puesta en marcha de LCD
   printf(lcd_putc,"\fSentido = RIGHT");
   printf(lcd_putc,"\nSpeed = %2d ms", delaytime_ms);
   
   //Bucle infinito
   while(1){
      portc = half_step_derechas[i];   //Se ejecuta un step
      delay_ms(delaytime_ms);
      //Giro derechas
      if(sentido == derechas){
         if(i == 7) i = 0;             //Reinicia el numero de pasos del step motor definidos en el arreglo
         else i ++;                    //Para el full_step y wave_drive se deberá cambiar el valor de 7 a 3
      }
      //Giro izquierdas
      else{
         if(i == 0) i = 7;             //Reinicia el numero de pasos del step motor definidos en el arreglo
         else i --;                    //Para el full_step y wave_drive se deberá cambiar el valor de 7 a 3
      }  
   }
}
//Funcion que realiza la orden pedida desde el Joystick
void actualizar_estado(){
   for(int i = 0; i < 4; i ++){
      if(mando[i] == 1){
         switch(i){
            //Girar derechas
            case 0:
               sentido =  derechas;
               lcd_gotoxy(1,1);
               printf(lcd_putc,"\fSentido = RIGHT");
            break;
            //Disminuir velocidad
            case 1:
               disminuir_velocidad();
            break;
            //Girar izquierdas
            case 2:
               sentido = izquierdas;
               lcd_gotoxy(1,1);
               printf(lcd_putc,"\fSentido = LEFT");
            break;
            //Aumentar velocidad
            case 3:
               aumentar_velocidad();
            break;
         }
         lcd_gotoxy(1,2);
         printf(lcd_putc,"\nSpeed = %2d ms", delaytime_ms);
      }
   }
}
//Funcion para disminuir velocidad de motor aumentando el delay entre cada step
void disminuir_velocidad(){
   if(delaytime_ms == 20) delaytime_ms = 20;
   else delaytime_ms = delaytime_ms + 1;
}
//Funcion para aumentar velocidad de motor disminuyendo el delay entre cada step
void aumentar_velocidad(){
   if(delaytime_ms == 1) delaytime_ms = 1;
   else delaytime_ms = delaytime_ms - 1;
}
