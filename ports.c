
/***************************************************************************//**
  @file     +ports.c+
  @brief    +Manipulacion de los puertos A,B y D+
  @author   +Grupo 6+
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "ports.h"

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static union{
    uint16_t D;
    uint8_t myPorts[2];
}portD;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void bitSet (char letra_puerto, char bits)
{
    if (letra_puerto == 'A'){
        char mask = 1 << bits;
        portD.myPorts[0] = portD.myPorts[0] | mask;
    }else if (letra_puerto == 'B'){
        char mask = 1 << bits;
        portD.myPorts[1] = portD.myPorts[1] | mask;
    }else if (letra_puerto == 'D'){
        uint16_t mask = 1 << bits;
        portD.D = portD.D | mask;
        }
}

void bitClr (char letra_puerto, char bits)
{
    if (letra_puerto == 'A'){
        char mask = ~(1 << bits);
        portD.myPorts[0] = portD.myPorts[0] & mask;
    }else if (letra_puerto == 'B'){
        char mask = ~(1 << bits);
        portD.myPorts[1] = portD.myPorts[1] & mask;
    }else if (letra_puerto == 'D'){
        uint16_t mask = ~(1 << bits);
        portD.D = portD.D & mask;
        }
}

char bitGet (char letra_puerto, char bits)
{
    char result;
    if (letra_puerto == 'A'){
        char mask = 1;
        result = mask & (portD.myPorts[0] >> bits);
    }else if (letra_puerto == 'B'){
        char mask = 1;
        result = mask & (portD.myPorts[1] >> bits);
    }else if (letra_puerto == 'D'){
        uint16_t mask = 1;
        uint16_t result_16b = mask & (portD.D >> bits);
        result = result_16b;
        }
 return result;
} 



// invierto el valor del bit seleccionado en el puerto elegido 
void bitToggle (char letra_puerto, char bits)
{
    if(bitGet(letra_puerto,bits)==0)
        bitSet(letra_puerto,bits);
    else if(bitGet(letra_puerto,bits)==1)
        bitClr(letra_puerto,bits);           
}

//Para prender un bit del puerto, colocar un 1 en el bit correspondiente de la mascara. 
//Y para que un bit se mantenga igual en el puerto, colocar un 0 en el bit correspondiente de la mascara
void maskOn(char letra_puerto, uint16_t mask)
{
    if (letra_puerto == 'A'){
        char mask_char = mask;
        portD.myPorts[0] = portD.myPorts[0] | mask_char;
    }else if (letra_puerto == 'B'){
        char mask_char = mask;
        portD.myPorts[1] = portD.myPorts[1] | mask_char;
    }else if (letra_puerto == 'D'){
        portD.D = portD.D | mask;
        }
}


// Para apagar un bit del puerto, colocar un 1 en el bit correspondiente de la mascara. 
//Y para que un bit se mantenga igual en el puerto, colocar un 0 en el bit correspondiente de la mascara
void maskOff(char letra_puerto, uint16_t mask)
{
    if (letra_puerto == 'A'){
        char mask_char = mask;
        portD.myPorts[0] = ~(~(portD.myPorts[0]) | mask_char);
    }else if (letra_puerto == 'B'){
        char mask_char = mask;
        portD.myPorts[1] = ~(~(portD.myPorts[1]) | mask_char);
    }else if (letra_puerto == 'D'){
    portD.D = ~(~(portD.D) | mask);
    }
}

//pone en el bit opuesto a aquellos bits que tienen 1 en la mascara
void maskToggle(char letra_puerto, uint16_t mask)
{
    if (letra_puerto == 'A'){
        char mask_char = mask;
        portD.myPorts[0] = portD.myPorts[0]^mask_char;
    }else if (letra_puerto == 'B'){
        char mask_char = mask;
        portD.myPorts[1] = portD.myPorts[1]^mask_char;
    }else if (letra_puerto == 'D'){
        portD.D = (portD.D)^mask;
    }
}

//funciones para que el usuario lea lo que se escribio en el puerto

uint16_t lectura_puerto(char puerto)
{
    uint16_t res=0;
    if(puerto == 'A'){
        res = portD.myPorts[0];
    }else if (puerto == 'B'){
        res = portD.myPorts[1];
    }else if (puerto == 'D'){
        res = portD.D;
    }
    return res;
}

void imprimir_puerto(char puerto)
{
    printf("Estado del puerto %c \n", puerto);
    int i;
    if(puerto == 'A' || puerto == 'B' )
    {
        for(i=7;i>=0;i--)
        {
            printf("%d ",bitGet(puerto,i));
        }
    }else if (puerto == 'D')
    {
        for(i=15;i>=0;i--)
        {
            printf("%d ",bitGet(puerto,i));
        }
        
    }
    printf("\n");
}

