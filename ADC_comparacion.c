//Directivas
#include <16f877a.h>
#device adc = 10
#fuses xt, nowdt
#use delay(clock = 4M)
#include <lcd.c>

//Registros
#byte trisa = 0x85
#byte porta = 0x05
#bit trisc_rc0 = 0x87.0
#bit portc_rc0 = 0x07.0

//Programa principal
void main()
{
   //Configuracion de GPIOs
   //Configuracion de perifericos
   trisa = 0xff;                    //Puesrto A config. como entradas
   trisc_rc0 = 0;                   //RC0 como salida
   portc_rc0 = 0;                   //RC0 inicializado a 0
   
   //Configuracion de LCD
   lcd_init();                      //Inicializacion de LCD
   
   //Configuracion del ADC
   setup_adc_ports(AN0_AN1_AN3);    //Entradas RA0, RA1 y RA3 como analogicas
   setup_adc(ADC_CLOCK_DIV_8);       //Seleccion del reloj para la conversion
   
   //Variables locales
   int16 potenciometro1 = 0, potenciometro2 = 0;      //Para leer los registros ADDRESS Y ADRESL

   float potenciometro_f1 = 0, potenciometro_f2 = 0;
   
   //Bucle infinito
   while(1)
   {
      //POTENCIOMETRO 1
      //Seleccionar canal
      set_adc_channel(0);
      //Esperar tiempo de adquisicion
      delay_us(20);
      //Lanzar conversion
      potenciometro1 = read_adc();
      
      //POTENCIOMETRO 2
      //Seleccionar canal
      set_adc_channel(1);
      //Esperar tiempo de adquisicion
      delay_us(20);
      //Lanzar conversion
      potenciometro2 = read_adc();
      
      //Calculo de tension
      potenciometro_f1 = 5.0 * (potenciometro1 / 1023.0);   //Valor de tension analogica de entrada POT 1
   
      potenciometro_f2 = 5.0 * (potenciometro2 / 1023.0);   //Valor de tension analogica de entrada POT 2
      
      //Mostramos en LCD
      printf(lcd_putc,"\fVoltaje P1 = %.2f V", potenciometro_f1);
      printf(lcd_putc,"\nVoltaje P2 = %.2f V", potenciometro_f2);
      
      //Realizamos la comparacion
      if(potenciometro_f1 >= potenciometro_f2) portc_rc0 = 1;
      else portc_rc0 = 0;
      
      delay_ms(200);    //Periodo de la conversion ADC
   }
}
