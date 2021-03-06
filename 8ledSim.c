#include "ports.h"
#define puertoA 'A' // por consigna los 8 LEDs se conectan al puerto A
#define PASAJE_DECIMAL(n) ((n)-'0') // pasar char ingresado a su decimal correspondiente
#include <allegro5/allegro.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_font.h> //manejo de fonts
#include <allegro5/allegro_ttf.h> //Manejo de ttfs



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

#define FPS    1.0 //los led van a parpadear cada un segundo cada 1 segundo


int main(void) {
    
    ALLEGRO_DISPLAY * display = NULL;
    ALLEGRO_EVENT_QUEUE * event_queue = NULL;    
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT * font = NULL;
    ALLEGRO_BITMAP *logo;
    
    bool close_display = false; 
    bool redraw = false; //se prende cada vez que se produce un evento por el timer
    

    if (!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }
    
    //INICIALIZACION DE LA COLA DE EVENTOS
   
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "failed to create event_queue!\n");
        return -1;
    }
    
    //INICIALIZACION DEL TECLADO
    
    if (!al_install_keyboard()) {
        fprintf(stderr, "failed to initialize the keyboard!\n");
        return -1;
    }
    
    //INICIALIZACION DEL TIMER

    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }
    
    //INICIALIZACION DEL DISPLAY + DIMENSIONES + COLOR FONDO
   
    display = al_create_display(900,640); //(ancho,alto)
    al_set_window_title(display,"Simulador 8 LEDs en el puerto A");
    if (!display) {
        al_destroy_event_queue(event_queue);
        return -1;
    }
    al_clear_to_color(al_map_rgb(255, 255, 255));
    
    //INICIALIZACION + DIBUJOS DE TEXTOS
    
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    font = al_load_ttf_font("disney.ttf", 13, 0);
    if (!font) {
        fprintf(stderr, "Could not load 'disney.ttf'.\n");
        return -1;
    }
    al_draw_text(font, al_map_rgb(0, 0, 0), (260) ,(410), ALLEGRO_ALIGN_CENTER, "Toogle todo");
    al_draw_text(font, al_map_rgb(0, 0, 0), (260) ,(470), ALLEGRO_ALIGN_CENTER, "Prender todo");
    al_draw_text(font, al_map_rgb(0, 0, 0), (260) ,(530), ALLEGRO_ALIGN_CENTER, "Apagar todo");
    al_draw_text(font, al_map_rgb(0, 0, 0), (600) ,(440), ALLEGRO_ALIGN_CENTER, "Simulador de 8 LEDs conectados al puerto A.");
    al_draw_text(font, al_map_rgb(0, 0, 0), (600) ,(470), ALLEGRO_ALIGN_CENTER,"Instrucciones:");
    al_draw_text(font, al_map_rgb(0, 0, 0), (600) ,(500), ALLEGRO_ALIGN_CENTER," - Tocando la tecla b todos los leds encendidos comenzaran");
    al_draw_text(font, al_map_rgb(0, 0, 0), (600) ,(520), ALLEGRO_ALIGN_CENTER,"a parpadear hasta que se presione");
    al_draw_text(font, al_map_rgb(0, 0, 0), (600) ,(540), ALLEGRO_ALIGN_CENTER,"la misma tecla.");
    al_draw_text(font, al_map_rgb(0, 0, 0), (600) ,(570), ALLEGRO_ALIGN_CENTER," - Debajo de cada LED hay un boton de prendido/apagado.");
    
    //INICIALIZACION + DIBUJO DE LOGO
    
    if (!al_init_image_addon()) {
        fprintf(stderr, "Unable to start image addon \n"); //Igual que printf pero imprime al error std 
        al_uninstall_system();
        return -1;
    }
    if (!(logo = al_load_bitmap("itba_logo-2000x664.png"))) {
        fprintf(stderr, "Unable to load logo\n");
        al_uninstall_system();
        al_shutdown_image_addon();
        al_destroy_display(display);
        return -1;
    }
    al_draw_scaled_bitmap(logo,
            0, 0, al_get_bitmap_width(logo), al_get_bitmap_height(logo), //imagen
            200, 0,  //Posicion donde lo dibuja
            500, 200, // TE LO DIBUJA DE 500 X200
            0);
    
    //INICIALIZACION MOUSE
    
    al_install_mouse();
    if (!al_is_mouse_installed()){
        close_display=al_show_native_message_box(display,"Error","No se pudo inicializar correctamente el mouse","El programa se cerrara automaticamente ","OK",ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    
    //Registra el display a la cola de eventos, los eventos del display se iran guardando en la cola 
    // a medida que vayan sucediendo 
    al_register_event_source(event_queue, al_get_display_event_source(display)); //SE REGINTRAN LOS EVENTOS  DEL DISPLAY
    al_register_event_source(event_queue, al_get_mouse_event_source()); //SE REGINTRAN LOS EVENTOS  DEL MOUSE
    al_register_event_source(event_queue, al_get_timer_event_source(timer)); //SE REGINTRAN LOS EVENTOS  DEL TIMER
    al_register_event_source(event_queue, al_get_keyboard_event_source()); //SE REGINTRAN LOS EVENTOS  DEL TECLADO
 

    //inicializacion del timer
    al_start_timer(timer);
    
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
    
    char flag_b =0; //Flag -> existencia evento de la tecla b  
    char blink = 0; //manejar el parpadeo de los lEDs
    
    while (!close_display) { // mientras que no se cierre el display con el evento de cierre
        ALLEGRO_EVENT events;
        
        if (al_get_next_event(event_queue, &events)) //Toma un evento de la cola.
        {
            
            if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
                close_display = true;
            }

            if (events.type == ALLEGRO_EVENT_KEY_DOWN){
                if(events.keyboard.keycode == ALLEGRO_KEY_B){
                    flag_b = !flag_b; 
                }
                
            }
            
            if (events.type == ALLEGRO_EVENT_TIMER){
                redraw = true;
            }
            
            if (flag_b == 1 && redraw == true){ // presiona tecla b + evento timer = LEDs encendidos deben prenderse y apagarse
                blink = !blink;
                if (bitGet(puertoA,0)==1){
                    manejo_leds(0,blink);
                }
                if (bitGet(puertoA,1)==1){
                    manejo_leds(1,blink);
                }
                if (bitGet(puertoA,2)==1){
                    manejo_leds(2,blink);
                }
                if (bitGet(puertoA,3)==1){
                    manejo_leds(3,blink);
                }
                if (bitGet(puertoA,4)==1){
                    manejo_leds(4,blink);
                }
                if (bitGet(puertoA,5)==1){
                    manejo_leds(5,blink);
                }
                if (bitGet(puertoA,6)==1){
                    manejo_leds(6,blink);
                }
                if (bitGet(puertoA,7)==1){
                    manejo_leds(7,blink);
                }
                            
            }else if(flag_b==0 && blink==0){ //cuando se presiona nuevamente la b. Los lEDs queden encendidos
                
                if (bitGet(puertoA,0)==1){
                    manejo_leds(0,!blink);
                }
                if (bitGet(puertoA,1)==1){
                    manejo_leds(1,!blink);
                }
                if (bitGet(puertoA,2)==1){
                    manejo_leds(2,!blink);
                }
                if (bitGet(puertoA,3)==1){
                    manejo_leds(3,!blink);
                }
                if (bitGet(puertoA,4)==1){
                    manejo_leds(4,!blink);
                }
                if (bitGet(puertoA,5)==1){
                    manejo_leds(5,!blink);
                }
                if (bitGet(puertoA,6)==1){
                    manejo_leds(6,!blink);
                }
                if (bitGet(puertoA,7)==1){
                    manejo_leds(7,!blink);
                }
                
            }
            
            if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){ //evento click mouse. Se analizan las coordenadas del mouse simulando un boton. Se cambia el estado del LED y el color del boton.
                
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
    al_destroy_timer(timer);
    //Recordar al init es "destruido" automaticamente 

    return (EXIT_SUCCESS);
}
// funcion para cambiar el color de los botones que manjean el encendido y apagado de los LEDS
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

//funcion para cambiar el color de los botones actuan sobre todos los LEDs
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

//funcion para cambiar el color de los LEDs
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