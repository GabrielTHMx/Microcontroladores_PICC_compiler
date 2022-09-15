/*
 * File:   ELECTROESTIMULADOR.c
 * Author: Gabriel y David
 *
 * Created on September 8, 2021, 7:06 PM
 */

#include <xc.h>
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/mcc.h"
#define FCY 8000000UL
#include <libpic30.h>
#include <p24FJ256GA702.h>

/////////////////// BOTONES ///////////////////////////////////////////////////
unsigned int Boton1(unsigned int opcion, int limite);       //FUNCION ELEGIR OPCION
void Opcion(unsigned int opcion);                           //TXT OPCIONES
int Boton2();                                               //FUNCION ACEPTAR OPCION
///////////////////////////////////////////////////////////////////////////////

//------------------ MENU --------------------------------------------------//
int Menu(unsigned int opcion, int aceptar);                //MENU DE OPCIONES
//--------------------------------------------------------------------------//

//////////////////VALORES DE DELAY DE FRECUENCIA //////////////////////////////
int milisegundos[]={0,47,22,14,10,7,6,5,4,3,2,2,2,1,1,1};             //0 Hz a 150 Hz (SALTOS DE 10Hz)

int microsegundos[]={0,800,100,200,300,900,200,100,200,450,920,500,100,800,515,300};                    
///////////////////////////////////////////////////////////////////////////////

//----------------- VISUALIZAR VALORES DE FRECUENCIA EN NEXTION -------------//
void BCDNextion(unsigned int x);
volatile unsigned char Ones,Tens,Hund,Thou,Tenk;
void BintoBCDONE(unsigned int x);  //BCD 0 - 9 HZ
void BintoBCDTEN(unsigned int x);  //BCD 10 - 99 HZ
void BintoBCDHUND(unsigned int x); //BCD 100 - 999 HZ
void FinCadena (void);
//---------------------------------------------------------------------------//

////////////////// SESIONES PROGRAMADAS ///////////////////////////////////////
void SesionName(unsigned int opcion);
int Sesiones(unsigned int opcion,int aceptar);
void Finaliza(void);
///////////////////////////////////////////////////////////////////////////////

int main(void)
{
    SYSTEM_Initialize();                        //INICIALIZACIÓN DE DISPOSITIVO
    PORTA = 0;                                  //LIMPIAR PUERTO A
    PORTB = 0;                                  //LIMPIAR PUERTO B
    
    unsigned int opcion = 1;                    //OPCION DE MENU
    int aceptar = 0;                            //ACEPTAR OPCION
    
    printf("page 0");                           //PAGINA 0 - LOGOTIPO
    FinCadena();
    __delay_ms(2000);
    
    printf("page 1");                           //PAGINA 1 - PAGINA PRINCIPAL
    FinCadena();

    while (1)                                   //PROGRAMA PRINCIPAL
    {
        int limite = 2;
        //BCDNextion(opcion); 
        opcion = Boton1(opcion, limite);                //ELIGE OPCION
        Opcion(opcion);                         
        aceptar = Boton2();                     //ACEPTA OPCION
        opcion = Menu(opcion, aceptar);                  //FUNCIÓN MENU
      
    }
    return 1;
}
unsigned int Boton1(unsigned int opcion, int limite)        //BOTON 1 - ESCOGER OPCION
{
    if(PORTAbits.RA0 == 1 ){                    //BOTON 1 - RB15 
        opcion ++;                              //INCREMENTA OPCION
        __delay_ms(200);                        //TIEMPO DE ANTIREBOTE
        
        if(opcion > limite) opcion = 1;              //OPCION DE 1 A 2
        
    }
    else opcion = opcion;
    
    return opcion;                              //REGRESA VALOR DE OPCION
}
void Opcion(unsigned int opcion)
{
  if(opcion == 1){
        printf("t0.txt=%c1.- SESION%c%cPROGRAMADA%c",0x22,0x0D,0x0A,0x22);         //MENSAJE "SESION PREPROGRAMADA"
        FinCadena();
    }
    else if(opcion == 2){
        printf("t0.txt=%c2.- SESION LIBRE%c",0x22,0x22);                           //MENSAJE t0 VACIO
        FinCadena();
    }
    else{
        Nop();                                  //OPCION NO DISPONIBLE
    }         
}
int Boton2(void)                                //BOTON 2 - ACEPTAR OPCION
{
    int aceptar = 0;                            
    if(PORTAbits.RA1 == 1){                     //BOTON 2 - ACEPTAR
        __delay_ms(200);
        aceptar = 1;                            //ACEPTAR = 1
    }
    return aceptar;                             //REGREA ACEPTAR = 1
}
int Menu(unsigned int opcion, int aceptar)     //MENU DE OPCIONES
{
    int bandera = 0;
    int limite = 4;
    
    if(aceptar == 1){                           //BOTÓN ACPETAR SELECCIONADO
        
        switch(opcion){                         //MENÚ
            aceptar = 0;
            case 0:
                Nop();
                break;

            case 1:                             //OPCIÓN 1 DE MENU

                opcion = 1;
                aceptar = 0;
                printf("page 2");               //PAGINA 2 - SESION PROGRAMADA
                FinCadena();

                    while(bandera == 0){                        //MENU SESIONES
                        opcion = Boton1(opcion, limite);        //ELIGE OPCION
                        SesionName(opcion);                         
                        aceptar = Boton2();                     //ACEPTA OPCION

                        bandera = Sesiones(opcion, aceptar);    //FUNCIÓN MENU
                    }

                printf("page 1");
                FinCadena();
                break;                              //TERMINA OPCION 1
          
            case 2:
                printf("page 3");
                FinCadena();
                break;
        }
        opcion = 1;
    }
        else                                        //ACEPTAR NO HA SIDO SELECCIONADO
        {
            Nop();
            opcion = opcion;
        }
    return opcion;
}
void BCDNextion(unsigned int x)                     //CONVERSION NUMEROS A BCD (NEXTION) 
{
    if(x < 10) BintoBCDONE(x);
    if(x >= 10 && x < 100) BintoBCDTEN(x);
    if(x >= 100 && x < 1000) BintoBCDHUND(x);
}
void FinCadena (void)                               // FIN CADENA DE CARACTERES UART
{
    UART1_Write(0xff);
    UART1_Write(0xff);
    UART1_Write(0xff);
}
void BintoBCDONE(unsigned int x)                    //FUNCIÓN BINARIO A BCD 0 - 9
{
    Ones=x%10+0x30;
    printf("t0.txt=%c %c%c",0x22,Ones,0x22);
    FinCadena();
}
void BintoBCDTEN(unsigned int x)                    //FUNCIÓN BINARIO A BCD 10 - 99
{
    Ones=x%10+0x30;
    x/=10;
    Tens=x%10+0x30;
    printf("t0.txt=%c %c%c%c",0x22,Tens,Ones,0x22);
    FinCadena();
}
void BintoBCDHUND(unsigned int x)                   //FUNCIÓN BINARIO A BCD 100 - 999
{
    Ones=x%10+0x30;
    x/=10;
    Tens=x%10+0x30;
    x/=10;
    Hund=x%10+0x30;
    printf("t0.txt=%c %c%c%c%c",0x22,Hund,Tens,Ones,0x22);
    FinCadena();
}
void SesionName(unsigned int opcion)
{
    if(opcion == 1){
        printf("t0.txt=%cSESION 1%c",0x22,0x22);                    //MENSAJE "SESION 1"
        FinCadena();
    }
    else if(opcion == 2){
        printf("t0.txt=%cSESION 2%c",0x22,0x22);                    //MENSAJE "SESION 2"
        FinCadena();
    }
    else if(opcion == 3){
        printf("t0.txt=%cSESION 3%c",0x22,0x22);                    //MENSAJE "SESION 3"
        FinCadena();
    }
    else if(opcion == 4){
        printf("t0.txt=%cREGRESAR%c%cA MENU%c",0x22,0x0D,0x0A,0x22);    //MENSAJE "REGRESAR A MENU"
        FinCadena();
    }
    else{
        Nop();                                                          //OPCION NO DISPONIBLE
    }    
}
int Sesiones(unsigned int opcion, int aceptar)
{
    int i, j, k, bandera = 0;                                   //VARIABLES DE FOR
    unsigned int frecuencia;                               //FRECUENCIA DE INICIO
    __delay_ms(10);
    
    if(aceptar == 1){
        
        switch(opcion){
            case 0:
                Nop();
                break;
                
            case 1:                                             //SESION 1 - 15 MIN - FREQ +10 - 50 Hz A 150 Hz
                printf("page 3");                               //PAGINA 3 DE NEXTION
                FinCadena();
                frecuencia = 50;
                BCDNextion(frecuencia);                         //FRECUENCIA A NEXTION
                PORTAbits.RA4 = 1;                              //ENCIENDE LED_TENS

                    for(k = 0; k <= 69; k++){                    //CONTROL DEL TIEMPO DE SESIÓN (13S x 69 = 897 s = aprox 900 s = 15 min)
                         for(j = 0; j <= 8; j++){              //10 SEGUNDOS DE SEÑAL (TIEMPO = 13 S)
                              for (i = 0; i <= frecuencia; i++){      //SEÑAL 10 SEGUNDOS
                                  PORTB = 0x8000;               //Activo puerto RB15
                                  __delay_us(200);              //400 us
                                  PORTB = 0;                    //Desactivo PORTB
                                  __delay_ms(milisegundos[(frecuencia/10)]);
                                  PORTB = 0;                    //Desactivo PORTB
                                  __delay_us(microsegundos[(frecuencia/10)]);
                                 }
                            
                         }
                         frecuencia += 10;                            //SEÑAL + 10Hz
                         
                        if(frecuencia > 150) frecuencia = 50;        //LIMITE SEÑAL HASTA 150 Hz
                        BCDNextion(frecuencia); 
                    }
                
                    Finaliza();
                break;
                
            case 2:                                             //SESION 2 - 15 MIN - FREQ +5 - 50 Hz A 150 Hz
                printf("page 3");                               //PAGINA 3 DE NEXTION
                FinCadena();
                frecuencia = 20;
                BCDNextion(frecuencia);                         //FRECUENCIA A NEXTION
                PORTAbits.RA4 = 1;                              //ENCIENDE LED_TENS

                    for(k = 0; k <= 69; k++){                    //CONTROL DEL TIEMPO DE SESIÓN (13S x 69 = 897 s = aprox 900 s = 15 min)
                         for(j = 0; j <= 8; j++){              //10 SEGUNDOS DE SEÑAL (TIEMPO = 13 S)
                              for (i = 0; i <= frecuencia; i++){      //SEÑAL 10 SEGUNDOS
                                  PORTB = 0x8000;               //Activo puerto RB15
                                  __delay_us(200);              //400 us
                                  PORTB = 0;                    //Desactivo PORTB
                                  __delay_ms(milisegundos[frecuencia/10]);
                                  PORTB = 0;                    //Desactivo PORTB
                                  __delay_us(microsegundos[frecuencia/10]);
                                 }
                            
                         }
                         frecuencia += 20;                            //SEÑAL + 5Hz
                         
                        if(frecuencia > 150) frecuencia = 20;        //LIMITE SEÑAL HASTA 150 Hz
                        BCDNextion(frecuencia); 
                    }
                
                Finaliza();
                break;
                
            case 3:                                             //SESION 3 - 15 MIN - FREQ +10 - 10 Hz A 150 Hz
                printf("page 3");                               //PAGINA 3 DE NEXTION
                FinCadena();
                frecuencia = 10;
                BCDNextion(frecuencia);                         //FRECUENCIA A NEXTION
                PORTAbits.RA4 = 1;                              //ENCIENDE LED_TENS

                    for(k = 0; k <= 69; k++){                    //CONTROL DEL TIEMPO DE SESIÓN (13S x 69 = 897 s = aprox 900 s = 15 min)
                         for(j = 0; j <= 8; j++){              //10 SEGUNDOS DE SEÑAL (TIEMPO = 13 S)
                              for (i = 0; i <= frecuencia; i++){      //SEÑAL 10 SEGUNDOS
                                  PORTB = 0x8000;               //Activo puerto RB15
                                  __delay_us(200);              //400 us
                                  PORTB = 0;                    //Desactivo PORTB
                                  __delay_ms(milisegundos[(frecuencia/10)]);
                                  PORTB = 0;                    //Desactivo PORTB
                                  __delay_us(microsegundos[(frecuencia/10)]);
                                 }
                            
                         }
                         frecuencia += 10;                            //SEÑAL + 10Hz
                         
                        if(frecuencia > 150) frecuencia = 10;        //LIMITE SEÑAL HASTA 150 Hz
                        BCDNextion(frecuencia); 
                    }
                Finaliza();
                break;
                
            case 4:
                Nop();
                __delay_ms(200);
                break;
        }
        return bandera = 1;
    }
    else{
        Nop();
        return bandera = 0;
    }
}
void Finaliza(void)
{
    PORTB = 0;                          //APAGA PUERTO 0
    printf("page 4");                   //PAGINA 4 "FIN DE SESION"
    FinCadena();
    __delay_ms(3000);                   //ESPERA 5 SEGUNDOS
    PORTAbits.RA4 = 0;                  //APAGA LED_TENS
}