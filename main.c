#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int main(int argc, char **argv) {
    if (!al_init()) return -1; // Inicialización de Allegro
    al_init_primitives_addon(); //enciende la geometria
    al_install_keyboard(); // Activamos el teclado 

    ALLEGRO_DISPLAY *display = al_create_display(800, 600); //creamos la ventana
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); //60 FPS
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue(); //lista de espera

    al_register_event_source(queue, al_get_keyboard_event_source()); //avisa cuando se presiona una tecla
    al_register_event_source(queue, al_get_display_event_source(display)); //avisa cuando se cierra la ventana
    al_register_event_source(queue, al_get_timer_event_source(timer)); //avisa cuando pasa un intervalo de tiempo

    //El punto (0,0) es la esquina superior izquierda de la pantalla
    float posX = 375.0; //distancia del lado izquierdo de la pantalla
    float posY = 275.0; //distancia de la parte de arriba de la pantalla
    float tamano = 50.0; //tamaño del cuadrado
    
    // el bool sirve como un interruptor para saber si se está presionando una tecla o no
    bool tecla_arriba = false;
    bool tecla_abajo = false;
    bool tecla_izq = false;
    bool tecla_der = false;
    
    //tiene corriendo el juego hasta cerrarlo o presionar escape
    bool corriendo = true;

    // El bool dibujar se usa para controlar cuándo se debe redibujar la pantalla
    bool dibujar = true;

    //iniciamos el timer para que empiece a generar eventos de temporizador
    al_start_timer(timer);

    //ciclo infinito para mantener el juego corriendo hasta que se cierre la ventana o se presione escape
    while (corriendo) {

        //Guarda detalles del juego
        ALLEGRO_EVENT evento;

        //espera a que ocurra un evento y lo guarda en la variable evento
        al_wait_for_event(queue, &evento);

        //Ve si se presiona una tecla
        if (evento.type == ALLEGRO_EVENT_TIMER) {
            //Aquí es donde se controla el movimiento
            if (tecla_arriba) posY -= 5.0;
            if (tecla_abajo)  posY += 5.0;
            if (tecla_izq)    posX -= 5.0;
            if (tecla_der)    posX += 5.0;
            
            dibujar = true; 
        }

        //revisa si se presiona o suelta una tecla y actualiza los bools correspondientes
        else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(evento.keyboard.keycode) {
                case ALLEGRO_KEY_UP:    tecla_arriba = true; break;
                case ALLEGRO_KEY_DOWN:  tecla_abajo = true; break;
                case ALLEGRO_KEY_LEFT:  tecla_izq = true; break;
                case ALLEGRO_KEY_RIGHT: tecla_der = true; break;
                case ALLEGRO_KEY_ESCAPE: corriendo = false; break;
            }
        }

        // Si se suelta una tecla, actualiza el bool correspondiente a false
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch(evento.keyboard.keycode) {
                case ALLEGRO_KEY_UP:    tecla_arriba = false; break;
                case ALLEGRO_KEY_DOWN:  tecla_abajo = false; break;
                case ALLEGRO_KEY_LEFT:  tecla_izq = false; break;
                case ALLEGRO_KEY_RIGHT: tecla_der = false; break;
            }
        }
        else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
            switch(evento.keyboard.keycode) {
                case ALLEGRO_KEY_UP:    tecla_arriba = false; break;
                case ALLEGRO_KEY_DOWN:  tecla_abajo = false; break;
                case ALLEGRO_KEY_LEFT:  tecla_izq = false; break;
                case ALLEGRO_KEY_RIGHT: tecla_der = false; break;
            }
        }

        // Si se cierra la ventana, termina el juego
        else if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            corriendo = false;
        }

        //dibuja el cuadrado
        if (dibujar && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(20, 30, 50)); 
            al_draw_filled_rectangle(posX, posY, posX + tamano, posY + tamano, al_map_rgb(0, 0, 0)); 
            al_flip_display();
            dibujar = false;
        }
    }

    //destruye la ventana grafica, el reloj y los eventos guardados
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}