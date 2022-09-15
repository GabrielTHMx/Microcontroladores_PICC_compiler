#include <16f877a.h>
#device ADC = 10
#fuses xt, nowdt, noprotect, noput, nolvp, nobrownout
#use delay (clock = 4M)
#use rs232(BAUD = 9600, Bits = 8, Parity = N, XMIT=PIN_C6, RCV=PIN_C7)
#byte trisb=0x86 
#byte trisc=0x87
#byte trisd=0x88
#byte PORTD=0X08
#byte PORTC=0X07
#byte PORTB=0x06
#include <stdlib.h>

int elemento = 0;
char numero_leido[5];
int16 numero_leido_int;

#int_rda
void rda_isr()
{
   while(numero_leido[elemento] != '\0')
   {
      numero_leido[elemento] = getchar();
      elemento++;
   }

   numero_leido_int = atol(numero_leido);
   
   if(numero_leido_int <= 50)
   {
      portd = 0x01;
   }
   else
   {
      portd = 0x00;
   }
}

void main()
{

   trisd = 0x00;
   portd = 0x00;
   
   setup_adc_ports(AN0_AN1_AN3);
   setup_adc(ADC_CLOCK_DIV_8);   
   
   enable_interrupts(global);
   disable_interrupts(int_rda);
   
   while(true)
   {
      set_adc_channel(0);
      delay_us(25);
      int16 v_adc1 = read_adc();
      
      putc('I');
      printf("%Lu", v_adc1);
      putc('N');
      printf("\r\n");
      delay_ms(50);
      
   }
}
