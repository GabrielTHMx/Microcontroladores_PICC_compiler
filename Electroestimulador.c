//Directivas
#include <16f877a.h>
#fuses xt, nowdt                        //Uso de crystal externo
#use delay(clock = 4M)                  //Frecuencia de reloj de 4MHz
#include <lcd.c>

//Registros
#byte trisb = 0x86                       //Uso de trisb
#byte portb = 0x06                       //Uso de portb
#byte trisc = 0x87                       //Uso de trisc
#byte portc = 0x07                       //Uso de portc
#byte trisd = 0x88                       //Uso de trisd
#byte portd = 0x08                       //Uso de portd

//Variables globales
int bandera_frecuencia = 0;
int contador = 0;
int frecuencia = 6;
short sleep_mode;

//Estructuras
typedef struct             //STRUCT LCD
{
   int fantasma[8];
   int carita[8];
   char bienvenido[10];
   char saliendo[8];
   
}LCD_t;

typedef struct             //STRUCT EFECTOS
{
   int x;
   int contador;
   int contador2;
}Efecto_t;

typedef struct             //STRUCT MENU
{
   int opcion;
}Menu_t;

//Funciones
void Efectos(int opcion);
void Menus(int opcion);
void Menus_LCD(int opcion);

//Interrupciones
#int_timer2
void timer2_isr()
{
   bandera_frecuencia ++;
   contador ++;
   
   if(bandera_frecuencia == frecuencia) 
   {
      output_toggle(pin_c1);
      bandera_frecuencia = 0;
   }
   
   if(contador == 100) 
   {
      frecuencia += 10;
      contador = 0;
   }
   
   if(frecuencia >= 255) frecuencia = 6;
   
}

#int_rb
void rb_isr()
{
   Menu_t menu;
   delay_ms(20);
   
   if(bit_test(portb, 4) == 0)         //CAMBIO DE OPCION
   {
      menu.opcion ++; 
      if(menu.opcion > 2) menu.opcion = 0;
      Menus_LCD(menu.opcion);
   }
   
   if(bit_test(portb, 5) == 0)         //ACEPTAR OPCION
   {
      Menus(menu.opcion);
   }
   
}

#int_ext
void ext_isr()
{
   static short button_pressed=FALSE;

   if(!button_pressed)        // if button action and was not pressed
   {
      button_pressed=TRUE;    // the button is now down
      sleep_mode=TRUE;        // activate sleep
      
      ext_int_edge(L_TO_H);   // change so interrupts on release
   }
   else                       // if button action and was pressed
   {
      button_pressed=FALSE;   // the button is now up
      sleep_mode=FALSE;       // reset sleep flag
      ext_int_edge(H_TO_L);   // change so interrupts on press
   }
   if(!input(PIN_B0))         // keep button action sychronized wth button flag
      button_pressed=TRUE;
   delay_ms(100);             // debounce button
}

//Programa principal
void main()
{  
   //Configuracion de Perifericos - GPIOs
   trisb = 0xf1;               //RB0 : RB3 configurado como salidas y RB4 : RB7 configurado como entrada
   portb = 0x00;               //Limpio puerto B
   trisc = 0x00;               //PORTC como salidas
   portc = 0x00;               //Limpio puerto C
   
   port_b_pullups(1);          //Activo resistencias pullups internas de pic en port B
   
   //Configuracion de LCD
   lcd_init();

   Efectos(0);       //EFECTO DE INICIO EN LCD

   //Configuracion de interrupciones
   enable_interrupts(global);
   enable_interrupts(int_rb);          //Interrupcion RB4 : RB7
   enable_interrupts(int_timer2);      //Interrupcion Timer2
   enable_interrupts(int_ext);         //Interrupcion Externa RB0
   ext_int_edge(H_TO_L);
   
   Menus_LCD(0);         //MUESTRA OPCION 1 EN LCD
   sleep_mode=FALSE;          // init sleep flag
   
   //Bucle infinito
   while(1)
   {
      output_toggle(pin_c0);
      delay_ms(500);
      
      if(sleep_mode)  sleep();        // if sleep flag set
                     // make processor sleep
   }
}

void Efectos(int opcion)
{
   Efecto_t efecto ;
   
   LCD_t _lcd = {{
            0b00011111,    //1
            0b00011111,    //2
            0b00010101,    //3
            0b00011111,    //4
            0b00011111,    //5
            0b00010101,    //6
            0b00010001,    //7
            0b00000000     //8
            },
            {
            0b00011011,    //1
            0b00011011,    //2
            0b00011011,    //3
            0b00011011,    //4
            0b00000000,    //5
            0b00011011,    //6
            0b00001110,    //7
            0b00000000     //8
            }, {'B', 'I', 'E', 'N', 'V', 'E', 'N', 'I', 'D', 'O'}, {'S', 'A', 'L', 'I', 'E', 'N', 'D', 'O'}};
   
   switch(opcion)       //MENU DE EFECTOS
   {
      case 0:        //EFECTO DE INICIO  
            
         lcd_set_cgram_char(0, _lcd.fantasma);
         lcd_set_cgram_char(1, _lcd.carita);

         //INICIA EFECTO DE FANTASMA
         for(efecto.x = 1; efecto.x <= 20; efecto.x ++)
         {
            lcd_putc("\f");
            lcd_gotoxy(efecto.x, 1);
            lcd_putc(0);
            delay_ms(20);
         }
         lcd_putc("\n");
         for(efecto.x = 20; efecto.x >= 1; efecto.x --)
         {
            lcd_putc("\f");
            lcd_gotoxy(efecto.x, 2);
            lcd_putc(0);
            delay_ms(20);
         }
         lcd_putc("\f");
         lcd_gotoxy(1, 1);
         lcd_putc(0);
         delay_ms(50);
         ///TERMINA EFECTO DE FANTASMA
         
         for(efecto.contador = 0; efecto.contador <= 3; efecto.contador ++)
         {
            lcd_putc("\f");
            delay_ms(200);
            lcd_putc(" ELECTROESTIMULADOR ");
            delay_ms(200);
         }
         lcd_putc("\f    ");
         for(efecto.contador = 0; efecto.contador < 10; efecto.contador ++)
         {
            lcd_gotoxy(efecto.contador + 3, 1);
            printf(lcd_putc,"%c", _lcd.bienvenido[efecto.contador] );
            lcd_gotoxy(efecto.contador + 3, 2);
            lcd_putc("_");
            delay_ms(200);
         }
         lcd_gotoxy(14, 1);
         lcd_putc(1);
         delay_ms(500);
      break;
      
      case 1:

         for(efecto.contador2 = 0; efecto.contador2 < 3; efecto.contador2 ++)
         {
            lcd_putc("\f");
            lcd_gotoxy(6, 1);
            
            for(efecto.contador = 0; efecto.contador < 8; efecto.contador ++)
            {
               printf(lcd_putc, "%c", _lcd.saliendo[efecto.contador]);
               delay_ms(100);
            }
            for(efecto.contador = 0; efecto.contador < 3; efecto.contador ++)
            {
               lcd_putc(".");
               delay_ms(100);
            }
         }
            
      break;
      
   }
}

void Menus_LCD(int opcion)
{
   switch(opcion)
   {
      case 0:
         lcd_putc("\f ELECTROESTIMULACION \n");
         lcd_putc("  FREQ. VARIABLE ");
      break;
      
      case 1:
         lcd_putc("\f ELECTROESTIMULACION \n");
         lcd_putc("   MODO BURST");
      break;
      
      case 2:
         lcd_putc("\f    SALIR ");
      break;
   }
}

void Menus(int opcion)
{
   switch(opcion)
   {
      case 0:     //OPCION 1 DE ELECTROESTIMULACION, MODO FREQ. VARIABLE                           
         //Configuracion de Timer2
         setup_timer_2(T2_DIV_BY_4, 124, 1);    //Medio ciclo de señal de 1KHz = 1 ms
      break;
      
      case 1:
         
      break;
      
      case 2:
         sleep();
         //Efectos(1);                //Efecto de SALIENDO
         
      break;
   }
}
