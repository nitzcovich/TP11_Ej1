/***************************************************************************//**
  @file     +ports.h+
  @brief    +Manipulacion de los puertos A,B y D+
  @author   +Grupo 6+
 ******************************************************************************/
#ifndef _PORTS_H_
#define _PORTS_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Dado un puerto y un número de bit, debe cambiar su estado a 1.
 * @param param1 Letra en mayuscula del puerto que se desea modificar
 * @param param2 Numero del bit que se desea modificar (del 0 al 7 para el 
 * puerto A y B, y del 0 al 15 para el D)
*/
void bitSet (char  , char);

/**
 * @brief Dado un puerto y un número de bit, debe cambiar su estado a 0
 * @param param1 Letra en mayuscula del puerto que se desea modificar
 * @param param2 Numero del bit que se desea modificar (del 0 al 7 para el
 *  puerto A y B, y del 0 al 15 para el D)
*/
void bitClr (char  , char );

/**
 * @brief Dado un puerto y un número de bit, debe devolver su valor.
 * @param param1 Letra en mayuscula del puerto el cual se quiere analizar
 * @param param2 Numero del bit que del cual se desea obtener su valor (del 0 
 * al 7 para el puerto A y B, y del 0 al 15 para el D)
 * @return Valor del bit
 */
char bitGet (char  , char);

/**
 * @brief Dado  un  puerto  y  un  número  de  bit,  debe  cambiar  al  estado 
 * opuesto en el que está (si está en 0 pasar a 1, y si está en 1 pasar a 0)
 * @param param1 Letra en mayuscula del puerto que se desea modificar
 * @param param2 Numero del bit que se desea modificar (del 0 al 7 para el 
 * puerto A y B, y del 0 al 15 para el D)
*/
void bitToggle (char , char );

/**
 * @brief Dado un puerto y una máscara, debe prender todos aquellos bits que 
 * estén  prendidos  en  la  máscara,  sin  cambiar  el  estado  de  los  restantes
 * @param param1 Letra en mayuscula del puerto que se desea modificar
 * @param param2 Mascara que se desa aplicar al puerto.Para prender un bit del 
 * puerto, colocar un 1 en el bit correspondiente de la mascara.Y para que un bit 
 * se mantenga igual en el puerto, colocar un 0 en el bit correspondiente de la mascara.
 * Aclaracion: si el puerto que se desea modificar es de 1 byte, dejar 0s en el 
 * primer byte de la mascara
*/
void maskOn(char, uint16_t);

/**
 * @brief Dado un puerto y una máscara, debe apagar todos aquellos bits que 
 * estén  prendidos  en  la  máscara,  sin  cambiar  el  estado  de  los  restantes.
 * @param param1 Letra en mayuscula del puerto que se desea modificar
 * @param param2 Mascara que se desa aplicar al puerto.Para apagar un bit del 
 * puerto, colocar un 1 en el bit correspondiente de la mascara.Y para que un bit 
 * se mantenga igual en el puerto, colocar un 0 en el bit correspondiente de la 
 * mascara.Aclaracion: si el puerto que se desea modificar es de 1 byte, dejar 0s 
 * en el primer byte de la mascara
*/
void maskOff(char , uint16_t mask);

/**
 * @brief Dado un puerto y una máscara, debe cambiar el estado de todos aquellos
 * bits  que  estén  prendidos  en  la  máscara  al  opuesto,  sin  cambiar  el 
 * estado de los restantes.
 * @param param1 Letra en mayuscula del puerto que se desea modificar
 * @param param2 Mascara que se desa aplicar al puerto.Para invertir un bit del 
 * puerto, colocar un 1 en el bit correspondiente de la mascara.Y para que un 
 * bit se mantenga igual en el puerto, colocar un 0 en el bit correspondiente de
 * la mascara.Aclaracion: si el puerto que se desea modificar es de 1 byte, dejar 
 * 0s en el primer byte de la mascara
*/
void maskToggle(char , uint16_t mask);

/**
 * @brief Funcion para que el usuario lea lo que modifico en el puerto
 * @param param1 Letra en mayuscula del puerto que se desea leer
 * @return Contenido del puerto
*/
uint16_t lectura_puerto(char);

/**
 * @brief Imprimir todos los bits del puerto
 * @param param1 Letra en mayuscula del puerto que se desea leer
*/

void imprimir_puerto(char puerto);


/*******************************************************************************
 ******************************************************************************/

#endif // _PORTS_H_