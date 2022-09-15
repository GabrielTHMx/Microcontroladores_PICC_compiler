#include <16f877a.h>
#device adc = 10
#fuses xt, nowdt
#use delay (clock = 4M)

#use rs232(BAUD = 9600, XMIT = pin_c6, RCV = pin_c7, Bits = 8, Parity = N)

#byte trisa = 0x85

int numero_leido = 50;
void main()
{

   trisa = 0xff;        //Todos los puertos A confg. como entradas
   
   setup_adc_ports(AN0);            //Entrada RA0 como analogica
   setup_adc(ADC_CLOCK_DIV_8);      //Seleccion del reloj para la conversion. Reloj mas cercano o superior a 4M
   set_adc_channel(0);              //Seleccion de canal 0
   delay_us(20);                    //Esperamos tiempo de adquisicion tras config. ADC
   
   int16 valor_digital = 0;         //Para leer registros ADRESH y ADRESL
   float valor_tension = 0;
   
   enable_interrupts(int_rda);
   enable_interrupts(global);
   
   puts("Introduzca el numero");
   printf("\n\rNumero leido : %d", numero_leido * 2);
   
   while(1)
   {
      delay_us(20);                    //Esperamos tiempo de adquisicion antes de realizar alguna conversion
      
      valor_digital = read_adc();      //10 bits; valor entre 0 y 1023
      
      valor_tension = 5.0 * (valor_digital / 1023.0);    //Valor de tension analogico de entrada
      
      //Mostrando en LCD
     printf("ADC = %ld bits\n\r", valor_digital);
      
      printf("Voltaje = %1.2f V\n\r", valor_tension);
      
      delay_ms(500);                   //Periodo para las conversiones de ADC
   }
}
