//Directivas
#include <16f877a.h>
#device adc = 10
#fuses xt, nowdt
#use delay(clock = 4M)
#include <lcd.c>

//Registros
#byte trisa = 0x85
#byte porta = 0x05

//Variables globales

//Interrupciones

//Programa principal
void main(){
   //Configuracion de perifericos
   //Configuracion de GPIOs
   trisa = 0xff;        //Todos los puertos A confg. como entradas
   
   //Configuracion de lcd
   lcd_init();
   
   //Configuracion del modulo ADC
   setup_adc_ports(AN0);            //Entrada RA0 como analogica
   setup_adc(ADC_CLOCK_DIV_8);      //Seleccion del reloj para la conversion. Reloj mas cercano o superior a 4M
   set_adc_channel(0);              //Seleccion de canal 0
   delay_us(20);                    //Esperamos tiempo de adquisicion tras config. ADC
   
   //Variables locales
   int16 valor_digital = 0;         //Para leer registros ADRESH y ADRESL
   float valor_tension = 0;
   
   //Bucle infinito
   while(1){
      delay_us(20);                    //Esperamos tiempo de adquisicion antes de realizar alguna conversion
      
      valor_digital = read_adc();      //10 bits; valor entre 0 y 1023
      
      valor_tension = 5.0 * (valor_digital / 1023.0);    //Valor de tension analogico de entrada
      
      //Mostrando en LCD
     printf(lcd_putc,"\fADC = %ld bits\n", valor_digital);
      
      printf(lcd_putc,"Voltaje = %1.2f V", valor_tension);
      
      delay_ms(500);                   //Periodo para las conversiones de ADC
      
   }
}  


