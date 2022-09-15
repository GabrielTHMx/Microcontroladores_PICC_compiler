#include <16f877a.h>
#fuses xt, nowdt
#use delay(clock = 4M)
#use rs232(BAUD = 9600, XMIT = pin_c6, RCV = pin_c7, Bits = 8, Parity = N)
#include <stdlib.h>

#define IGUAL 1
#define MENOR 2
#define MAYOR 3

int const numero_secreto = 77;
char numero_leido [elemento];

int16 numero_leido_int = 50;

int validar_numero(int16);
int comparar_numero(int16);
int jugar = 0;

#int_rda
void rda_isr()
{
   
   gets(numero_leido);
   printf("\n\rNumero introducido en letra %s", numero_leido);
   numero_leido_int = atol(numero_leido);
   printf("\n\rNumero introducido en numero %ld", numero_leido_int);
   delay_ms(1000);
   
   if(validar_numero(numero_leido_int) == 1) jugar = 1;
   else
   {
      printf("\fEl numero o caracter %s introducido no es valido", numero_leido);
      printf("\n\rPor favor, introduzca un numero entre 0 y 150\n\r");
   }
}

void main()
{
   enable_interrupts(int_rda);
   enable_interrupts(global);
   
   puts("Introduzca el numero");
   printf("\n\rNumero leido : %ld", numero_leido_int);
   
   int solucion = 0;
   while(1)
   {
      if(jugar == 1)
      {
         jugar = 0;
         
         solucion = comparar_numero(numero_leido_int);
         switch(solucion)
         {
            case IGUAL:
               printf("\n\rEl numero %ld es el correcto!! \n\r FIN DEL JUEGO", numero_leido_int);
               setup_uart(false);
            break;
            
            case MENOR:
               printf("\n\rEl numero %ld es menor", numero_leido_int);
               printf("\n\rIntroduzca el numero\n\r");
            break;
            
            case MAYOR:
               printf("\n\rEl numero %ld es mayor", numero_leido_int);
               printf("\n\rIntroduzca el numero\n\r");
            break;
         }
      }
   }
}

int validar_numero(int16 numero)
{
   if(numero >= 1 && numero <= 150) return TRUE;
   
   else return FALSE;
}

int comparar_numero(int16 numero)
{
   if(numero == numero_secreto) return IGUAL;
   else
   {
      if(numero < numero_secreto) return MENOR;
      else return MAYOR;
   }
}
