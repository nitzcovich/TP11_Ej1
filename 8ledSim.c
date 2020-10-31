#include "ports.h"
#define puertoA 'A' // por consigna los 8 LEDs se conectan al puerto A
#define PASAJE_DECIMAL(n) ((n)-'0') // pasar char ingresado a su decimal correspondiente
#include <allegro5/allegro.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_image.h"


static void draw_mouse_button_led(int but, bool down);
static void button_msk(int but,bool estado);
static void manejo_leds(int led, bool estado);

#define LEDs_EJE_Y 250
#define B0_EJE_X 800
#define B1_EJE_X 700
#define B2_EJE_X 600
#define B3_EJE_X 500
#define B4_EJE_X 400
#define B5_EJE_X 300
#define B6_EJE_X 200
#define B7_EJE_X 100

#define BOTON_ANCHO 60
#define BOTON_ALTO 40
#define BOTON_OFFSET_LED 70
#define BOTON_TOGGLE 1
#define BOTON_ON 2
#define BOTON_OFF 3
#define BOTON_OFFSET_EJE_X 150
#define BOTON_TOGGLE_EJE_Y 420
#define BOTON_ON_EJE_Y 480
#define BOTON_OFF_EJE_Y 540
#define BOTON_MSK_ANCHO 100
#define BOTON_MSK_ALTO 40

#define LEDs_RATIO 30

#define MASK 0x00FF

int main(void) {
    
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;
    bool close_display = false; //explicar bool tipo de dato
    int i = 0;

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
   
    event_queue = al_create_event_queue(); //Allegro usa cola eventos, como las colas del super pero sin comida :( (por orden de llegada)
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }     
    
    display = al_create_display(900,640); //(ancho,alto)
    al_set_window_title(display,"Simulador 8 LEDs en el puerto A");
    if (!display) {
        al_destroy_event_queue(event_queue);
        return -1;
    }
    al_clear_to_color(al_map_rgb(255, 255, 255));
 
    al_install_mouse();
    if (!al_is_mouse_installed()){
        close_display=al_show_native_message_box(display,"Error","No se pudo inicializar correctamente el mouse","El programa se cerrara automaticamente ","OK",ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    
    //Registra el display a la cola de eventos, los eventos del display se iran guardando en la cola 
    // a medida que vayan sucediendo 
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    //al_register_event_source(event_queue,al_get_keyboard_event_source());

    
    //inicializacion de los leds en el display, en forma de circulo
    al_init_primitives_addon();
    al_draw_circle(B0_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    al_draw_circle(B1_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    al_draw_circle(B2_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    al_draw_circle(B3_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    al_draw_circle(B4_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    al_draw_circle(B5_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    al_draw_circle(B6_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    al_draw_circle(B7_EJE_X,LEDs_EJE_Y,LEDs_RATIO+2,al_map_rgb(0,0,0),2);
    
    //inicializacion de botones
    int n;
    for(n=1;n<=8;n++){
        draw_mouse_button_led(n,true);
    }
    
    for(n=1;n<=3;n++){
        button_msk(n,true);
    }
    
    
    al_flip_display();
    while (!close_display) {
        ALLEGRO_EVENT events;
        
        if (al_get_next_event(event_queue, &events)) //Toma un evento de la cola, VER RETURN EN DOCUMENT.
        {
            
            if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                close_display = true;
            }
            
            if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
                
                al_flip_display();
                if(events.mouse.x <= B0_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B0_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,0)== 0){
                        bitSet(puertoA,0);
                        al_draw_filled_circle(B0_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(1,false);
                    }else if (bitGet(puertoA,0)== 1){
                        bitClr(puertoA,0);
                        al_draw_filled_circle(B0_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(1,true);
                    }   
                }
                if(events.mouse.x <= B1_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B1_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,1)== 0){
                        bitSet(puertoA,1);
                        al_draw_filled_circle(B1_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(2,false);
                    }else if (bitGet(puertoA,1)== 1){
                        bitClr(puertoA,1);
                        al_draw_filled_circle(B1_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(2,true);
                    }   
                }
                if(events.mouse.x <= B2_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B2_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,2)== 0){
                        bitSet(puertoA,2);
                        al_draw_filled_circle(B2_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(3,false);
                    }else if (bitGet(puertoA,2)== 1){
                        bitClr(puertoA,2);
                        al_draw_filled_circle(B2_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(3,true);
                    }   
                }
                if(events.mouse.x <= B3_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B3_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,3)== 0){
                        bitSet(puertoA,3);
                        al_draw_filled_circle(B3_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(4,false);
                    }else if (bitGet(puertoA,3)== 1){
                        bitClr(puertoA,3);
                        al_draw_filled_circle(B3_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(4,true);
                    }   
                }                
                if(events.mouse.x <= B4_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B4_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,4)== 0){
                        bitSet(puertoA,4);
                        al_draw_filled_circle(B4_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(5,false);
                    }else if (bitGet(puertoA,4)== 1){
                        bitClr(puertoA,4);
                        al_draw_filled_circle(B4_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(5,true);
                    }   
                }
                if(events.mouse.x <= B5_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B5_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,5)== 0){
                        bitSet(puertoA,5);
                        al_draw_filled_circle(B5_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(6,false);
                    }else if (bitGet(puertoA,5)== 1){
                        bitClr(puertoA,5);
                        al_draw_filled_circle(B5_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(6,true);
                    }
                }
                
                if(events.mouse.x <= B6_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B6_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,6)== 0){
                        bitSet(puertoA,6);
                        al_draw_filled_circle(B6_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(7,false);
                    }else if (bitGet(puertoA,6)== 1){
                        bitClr(puertoA,6);
                        al_draw_filled_circle(B6_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(7,true);
                    }
                    
                }
                if(events.mouse.x <= B7_EJE_X+(BOTON_ANCHO/2) && events.mouse.x >= B7_EJE_X-(BOTON_ANCHO/2) && events.mouse.y <= LEDs_EJE_Y+BOTON_OFFSET_LED+(BOTON_ALTO/2) && events.mouse.y >= LEDs_EJE_Y+BOTON_OFFSET_LED-(BOTON_ALTO/2)){
                    if (bitGet(puertoA,7)== 0){
                        bitSet(puertoA,7);
                        al_draw_filled_circle(B7_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
                        draw_mouse_button_led(8,false);
                    }else if (bitGet(puertoA,7)== 1){
                        bitClr(puertoA,7);
                        al_draw_filled_circle(B7_EJE_X,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
                        draw_mouse_button_led(8,true);
                    }   
                }
                
                if(events.mouse.x <= BOTON_OFFSET_EJE_X+(BOTON_MSK_ANCHO/2) && events.mouse.x >= BOTON_OFFSET_EJE_X-(BOTON_MSK_ANCHO/2) && events.mouse.y <= BOTON_TOGGLE_EJE_Y+(BOTON_MSK_ALTO/2) && events.mouse.y >= BOTON_TOGGLE_EJE_Y-(BOTON_MSK_ALTO/2)){
                    maskToggle( puertoA, MASK);
                    //actualizo los botones
                    int n;
                    for(n=0;n<=7;n++){
                        if(bitGet(puertoA,n)== 1){
                            manejo_leds(n,1);
                            draw_mouse_button_led(n+1,false);                            
                        }else if (bitGet(puertoA,n)== 0){
                            manejo_leds(n,0);
                            draw_mouse_button_led(n+1,true);   
                        }                    
                    }
   
                }
                if(events.mouse.x <= BOTON_OFFSET_EJE_X+(BOTON_MSK_ANCHO/2) && events.mouse.x >= BOTON_OFFSET_EJE_X-(BOTON_MSK_ANCHO/2) && events.mouse.y <= BOTON_ON_EJE_Y+(BOTON_MSK_ALTO/2) && events.mouse.y >= BOTON_ON_EJE_Y-(BOTON_MSK_ALTO/2)){
                    maskOn( puertoA, MASK);
                    //actualizo los botones
                    int n;
                    for(n=0;n<=7;n++){
                        if(bitGet(puertoA,n)== 1){
                            manejo_leds(n,1);
                            draw_mouse_button_led(n+1,false);                            
                        }else if (bitGet(puertoA,n)== 0){
                            manejo_leds(n,0);
                            draw_mouse_button_led(n+1,true);   
                        }                    
                    }
   
                }
                if(events.mouse.x <= BOTON_OFFSET_EJE_X+(BOTON_MSK_ANCHO/2) && events.mouse.x >= BOTON_OFFSET_EJE_X-(BOTON_MSK_ANCHO/2) && events.mouse.y <= BOTON_OFF_EJE_Y+(BOTON_MSK_ALTO/2) && events.mouse.y >= BOTON_OFF_EJE_Y-(BOTON_MSK_ALTO/2)){
                    maskOff(puertoA, MASK);
                    //actualizo los botones
                    int n;
                    for(n=0;n<=7;n++){
                        if(bitGet(puertoA,n)== 1){
                            manejo_leds(n,1);
                            draw_mouse_button_led(n+1,false);                            
                        }else if (bitGet(puertoA,n)== 0){
                            manejo_leds(n,0);
                            draw_mouse_button_led(n+1,true);   
                        }                    
                    }
   
                }
                    
                }               
                
                al_flip_display();

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
       
    */
 
    return (EXIT_SUCCESS);
}

static void draw_mouse_button_led(int but, bool down)
 {
    int offset[8] = {B0_EJE_X, B1_EJE_X, B2_EJE_X, B3_EJE_X,B4_EJE_X, B5_EJE_X, B6_EJE_X, B7_EJE_X};
    ALLEGRO_COLOR grey;
    ALLEGRO_COLOR black;
    int x = offset[but-1];
    int y = LEDs_EJE_Y+BOTON_OFFSET_LED;
 
    grey = al_map_rgb(155, 155, 155);
    black = al_map_rgb(0, 0, 0);
 
    al_draw_filled_rectangle(x-(BOTON_ANCHO/2), y-(BOTON_ALTO/2), x + (BOTON_ANCHO/2), y + (BOTON_ALTO/2), grey);
    al_draw_rectangle(x-(BOTON_ANCHO/2)+1, y-(BOTON_ALTO/2)+1, x + (BOTON_ANCHO/2)+1, y + (BOTON_ALTO/2)+1, black, 1);
    if (!down) {
       al_draw_filled_rectangle(x-(BOTON_ANCHO/2), y-(BOTON_ALTO/2), x + (BOTON_ANCHO/2), y + (BOTON_ALTO/2), black);
    }
 }

static void button_msk(int but, bool estado){
    int offset[3]={BOTON_TOGGLE_EJE_Y,BOTON_ON_EJE_Y,BOTON_OFF_EJE_Y};
    int x = BOTON_OFFSET_EJE_X;
    int y = offset[but-1];
    
    ALLEGRO_COLOR grey;
    ALLEGRO_COLOR black;    
    
    grey = al_map_rgb(155, 155, 155);
    black = al_map_rgb(0, 0, 0);
    
    al_draw_filled_rectangle(x-BOTON_MSK_ANCHO/2, y-BOTON_MSK_ALTO/2, x + BOTON_MSK_ANCHO/2, y + BOTON_MSK_ALTO/2, grey);
    al_draw_rectangle(x-BOTON_MSK_ANCHO/2, y-BOTON_MSK_ALTO/2, x + BOTON_MSK_ANCHO/2, y + BOTON_MSK_ALTO/2, black, 1);
    if (!estado) {
       al_draw_filled_rectangle(x-BOTON_MSK_ANCHO/2, y-BOTON_MSK_ALTO/2, x + BOTON_MSK_ANCHO/2, y + BOTON_MSK_ALTO/2, black);
    }
    
}

static void manejo_leds(int led, bool estado){
    int offset[8] = {B0_EJE_X, B1_EJE_X, B2_EJE_X, B3_EJE_X,B4_EJE_X, B5_EJE_X, B6_EJE_X, B7_EJE_X};
    ALLEGRO_COLOR grey = al_map_rgb(155, 155, 155);;
    ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
    int x = offset[led];
   
    if (estado == 1){
        al_draw_filled_circle(x,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(57,255,20));
    }else{
        al_draw_filled_circle(x,LEDs_EJE_Y,LEDs_RATIO,al_map_rgb(255,255,255));
    }
   
}
