#include "ports.h"
#define puertoA 'A' // por consigna los 8 LEDs se conectan al puerto A
#define PASAJE_DECIMAL(n) ((n)-'0') // pasar char ingresado a su decimal correspondiente
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <stdbool.h>


int main(void) {
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    bool close_display = false; //explicar bool tipo de dato
    int i = 0;

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    //queue == cola/fila en ingles 
    event_queue = al_create_event_queue(); //Allegro usa cola eventos, como las colas del super pero sin comida :( (por orden de llegada)
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }

    display = al_create_display(900,640); //(ancho,alto)
    al_set_window_title(display,"Simulador 8 LEDs en el puerto A");
    if (!display) {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_event_queue(event_queue);
        return -1;
    }

    //Registra el display a la cola de eventos, los eventos del display se iran guardando en la cola 
    // a medida que vayan sucediendo 
    al_register_event_source(event_queue, al_get_display_event_source(display));

    //void al_clear_to_color(ALLEGRO_COLOR color) , ALLEGRO_COLOR al_map_rgb( unsigned char r, unsigned char g, unsigned char b)
    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_flip_display();

    while (!close_display) {
        ALLEGRO_EVENT ev;
        if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                close_display = true;
        }
    }

    al_destroy_display(display); //Destruir recursor empleados 
    al_destroy_event_queue(event_queue);
    //Recordar al init es "destruido" automaticamente 



    /*
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
    */
   
    return (EXIT_SUCCESS);
}

