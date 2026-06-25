#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<allegro5/allegro.h> //allegro
#include<allegro5/allegro_image.h> //imagenes
#include<allegro5/allegro_font.h> //fuentes
#include<allegro5/allegro_ttf.h> //fuentes ttf
#include<allegro5/allegro_primitives.h> //Dibujo 
#include<allegro5/allegro_audio.h> //audio
#include<allegro5/allegro_acodec.h> //audio codecs
#define TAM_TILE 40
#define fila 15
#define columna 20

struct arma_
{
    int daño;
    int alcance;
    int energia;
    bool en_uso;
};
typedef struct arma_ arma;

struct personaje_
{
    float x;
    float y;
    float velocidad;
    int vida;
    int energia;
    bool ataque;
    arma armas;
};
typedef struct personaje_ personaje;

struct enemigo_
{
    float x;
    float y;
    int vida;
    int energia;
    int daño;
    int alcance;
    float velocidad;
    bool ataque;
};
typedef struct enemigo_ enemigo;

struct cofre_
{
    float x;
    float y;
    bool abierto;
    int contenido;
};
typedef struct cofre_ cofre;

struct pocion_
{
    float x;
    float y;
    bool usada;
    int tipo; 
};
typedef struct pocion_ pocion;

int mapa[fila][columna];
personaje heroe;
personaje amigo;
enemigo guardia;
enemigo jefe;
enemigo basico;
cofre cofrecito;

void limites_pantalla(float *x, float *y);
bool colision(float x,float y);
bool cargar_mapa(const char *nombre_archivo);

int main()
{
    if(!cargar_mapa("nivel1.txt"))
    {
        return -1;
    }

    if(!al_init()) 
    {
        printf("Error al abrir Allegro");
        return -1;
    }

    al_init_primitives_addon();
    al_install_keyboard(); 
    al_init_image_addon();

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_BITMAP *fondo_nivel1=al_load_bitmap("Fondo1.jpg");
    if(!fondo_nivel1)
    {
        printf("Error fatal: No se encontro 'Fondo1.jpg'\n");
        return -1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool corriendo=true;
    bool dibujar=true;
    bool izquierda=false;
    bool derecha=false;
    int i,j;
    float x_bloque,y_bloque,velocidad_caida=0.0;

    al_start_timer(timer);

    while (corriendo)
    {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue, &evento);

        if(evento.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            corriendo=false;
        }
        else if(evento.type==ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_LEFT:
                izquierda=true;
                break;
            
            case ALLEGRO_KEY_RIGHT:
                derecha=true;
                break;

            case ALLEGRO_KEY_UP:
                if(colision(heroe.x,heroe.y+1.0))
                {
                    velocidad_caida=-9.0;
                }
                break;
            case ALLEGRO_KEY_ESCAPE:
                corriendo=false;
                break;
            }
        }
        else if(evento.type==ALLEGRO_EVENT_KEY_UP)
        {
            switch (evento.keyboard.keycode)
            {
            case ALLEGRO_KEY_LEFT:
                izquierda=false;
                break;
            
            case ALLEGRO_KEY_RIGHT:
                derecha=false;
                break;
            }
        }
        else if(evento.type==ALLEGRO_EVENT_TIMER)
        {
            if(izquierda)
            {
                heroe.x-=4.0;
                if(colision(heroe.x,heroe.y))
                {
                    heroe.x+=4.0;
                }
            }
            if(derecha)
            {
                heroe.x+=4.0;
                if(colision(heroe.x,heroe.y))
                {
                    heroe.x-=4.0;
                }
            }
            velocidad_caida+=0.5;
            heroe.y+=velocidad_caida;
            if(colision(heroe.x,heroe.y))
            {
                if(velocidad_caida>0)
                {
                    heroe.y = ((int)(heroe.y + 30) / TAM_TILE) * TAM_TILE - 30.1;
                }
                else if (velocidad_caida < 0) 
                {
                    heroe.y = ((int)heroe.y / TAM_TILE) * TAM_TILE + TAM_TILE + 0.1;
                }
                velocidad_caida = 0;
            }
            dibujar=true;
        }

        if(dibujar&&al_is_event_queue_empty(queue))
        {
            dibujar=false;
            al_clear_to_color(al_map_rgb(20,30,50));

            al_draw_scaled_bitmap(
                fondo_nivel1,
                0,0,al_get_bitmap_width(fondo_nivel1),al_get_bitmap_height(fondo_nivel1),
                0,0,columna*TAM_TILE,fila*TAM_TILE,
                0
            );

            for(i=0;i<fila;i++)
            {
                for(j=0;j<columna;j++)
                {
                    if(mapa[i][j]==1)
                    {
                        x_bloque=j*TAM_TILE;
                        y_bloque=i*TAM_TILE; 
                        al_draw_filled_rectangle(x_bloque,y_bloque,x_bloque+TAM_TILE,y_bloque+TAM_TILE,al_map_rgb(100,100,100));
                        al_draw_rectangle(x_bloque,y_bloque,x_bloque+TAM_TILE,y_bloque+TAM_TILE,al_map_rgb(50,50,60),1);
                    }
                }
            }
            al_draw_filled_rectangle(heroe.x,heroe.y,heroe.x+30,heroe.y+30,al_map_rgb(255,50,50));

            al_flip_display();
        }
    }
    
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    al_destroy_bitmap(fondo_nivel1);
    
    return 0;
}

void limites_pantalla(float *x, float *y)
{
    if(*x < 0)
    {
        *x = 0;
    }
    if(*x > 800)
    {
        *x = 800;
    }
    if(*y < 0)
    {
        *y = 0;
    }
    if(*y > 600)
    {
        *y = 600;
    }
}

bool colision(float x,float y)
{
    int izq=x/TAM_TILE;
    int der=(x+30)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+30)/TAM_TILE;
    if(izq<0||der>=20||arr<0||aba>=15)
    {
        return true;
    }
    if (mapa[arr][izq] == 1) 
    {
        return true;
    }
    if (mapa[arr][der] == 1) 
    {
        return true;
    }
    if (mapa[aba][izq] == 1) 
    {
        return true;
    }
    if (mapa[aba][der] == 1) 
    {
        return true;
    }
    return false;
}

bool cargar_mapa(const char *nombre_archivo)
{
    int i,j;
    FILE *archivo=fopen(nombre_archivo, "r");
    if(archivo==NULL)
    {
        printf("Error al abrir el archivo");
        return false;
    }
    for (i=0;i<fila;i++)
    {
        for(j=0;j<columna;j++)
        {
            fscanf(archivo,"%d",&mapa[i][j]);
            if(mapa[i][j]==9)
            {
                heroe.x=TAM_TILE*j;
                heroe.y=TAM_TILE*i;  
                mapa[i][j]=0;
            }
            else if(mapa[i][j]==8)
            {
                guardia.x=TAM_TILE*j;
                guardia.y=TAM_TILE*i;
                mapa[i][j]=0;
            }
            else if(mapa[i][j]==7)
            {
                jefe.x=TAM_TILE*j;
                jefe.y=TAM_TILE*i;
                mapa[i][j]=0;
            }
            else if(mapa[i][j]==6)
            {
                amigo.x=TAM_TILE*j;
                amigo.y=TAM_TILE*i;
                mapa[i][j]=0;
            }
            else if(mapa[i][j]==5)
            {
                basico.x=TAM_TILE*j;
                basico.y=TAM_TILE*i;
                mapa[i][j]=0;
            }
            else if(mapa[i][j]==4)
            {
                cofrecito.x=TAM_TILE*j;
                cofrecito.y=TAM_TILE*i;
                mapa[i][j]=0;
            }
            
        }
    }
    fclose(archivo);
    return true;
}