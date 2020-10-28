#include "ports.h"
#define puertoA 'A' // por consigna los 8 LEDs se conectan al puerto A
#define PASAJE_DECIMAL(n) ((n)-'0') // pasar char ingresado a su decimal correspondiente

int main(void) {

    printf("Simulador de 8 LEDs conectados al puerto A\n");
    int flag = 1; //el programa se ejecuta mientras que el flag sea 1.
    
    while(flag){
        printf("Opciones para manipular los LEDs:\n");
        printf("-Un numero de 0 a 7 correspondiente al LED que se quiere prender.\n");
        printf("-Ingresar la letra 't' para que todos los LEDs cambien al estado puesto.\n");
        printf("-Ingresar la letra 'c' para apagar todos los LEDs.\n");
        printf("-Ingresar la letra 's' para encender todos los LEDs.\n");
        printf("-Ingresar la letra 'q' para salir del p.\n");
        
        char letra = 0;
        uint16_t mask_t = 0x00FF;
        while((letra=getchar())!= '\n'){ // se imlementan las funciones ya creadas
            if (letra == 'q'){
                flag=0; //termine el programa, sale del primer while.
            }else if (letra =='t'){
                maskToggle( puertoA, mask_t);
                imprimir_puerto(puertoA);
            }else if (letra == 'c'){
                maskOff( puertoA, mask_t);
                imprimir_puerto(puertoA);
            }else if (letra == 's'){
                maskOn( puertoA, mask_t);
                imprimir_puerto(puertoA);               
            }else if(letra>='0' && letra <='7'){
                char valor = PASAJE_DECIMAL(letra);
                bitSet(puertoA,valor);
                imprimir_puerto(puertoA);
            }else{
                printf("Ingresaste una opcion INVALIDA\n");
            }
        }
    }
   
    return (EXIT_SUCCESS);
}

