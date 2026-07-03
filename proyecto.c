#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<allegro5/allegro.h> 
#include<allegro5/allegro_image.h> 
#include<allegro5/allegro_font.h> //fuentes
#include<allegro5/allegro_ttf.h> //fuentes ttf
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h> 
#include<allegro5/allegro_acodec.h> //audio codecs
#define TAM_TILE 40
#define fila 15
#define columna 20
#define MAX_ENEMIGOS 4
#define MAX_LASERS 10
#define ESTADO_IDLE 0
#define ESTADO_CAMINAR 1
#define ESTADO_SALTO 2
#define ESTADO_DISPARO 3
#define ESTADO_ATAQUE 4
#define ESTADO_DANO 5
#define ESTADO_CORRER 6
#define ESTADO_RECARGA 7

struct arma_
{
    int prejuicio;
    int alcance;
    int energia;
    bool en_uso;
};
typedef struct arma_ arma;

//INCLUIR A CADA STRUCT QUE LO NECEISITE UN ALTO Y ANCHO
struct personaje_
{
    float x;
    float y;
    float velocidad;
    int vida;
    int energia;
    int estado;
    bool ataque;
    arma armas;
};
typedef struct personaje_ personaje;

struct enemigo_
{
    float x;
    float y;
    int tipo;
    int vida;
    int energia;
    int herida;
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

struct lasers_
{
    float x;
    float y;
    float velocidad;
    bool activo;
};
typedef struct lasers_ lasers;

char mapa[fila][columna];
int total_enemigos=0;
personaje heroe;
personaje amigo;
enemigo enemigos[MAX_ENEMIGOS];
lasers laser_protagonista[MAX_LASERS];
lasers laser_enemigos[MAX_LASERS];
cofre cofrecito;

//AGREGAR MÁS FUNCIONES PARA ORDENAR EL JUEGO(SEPARAR DIBUJO DE LOGICA)
void limites_pantalla(float *x, float *y);
void inicializar_personaje(personaje *entidad,int vida_base,float velocidad_base);
void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial);
void mover_personaje(personaje *p,bool izq, bool der,float *vel_caida);
void mover_enemigos(enemigo ejercito[],int total);
void mover_lasers(lasers mis_lasers[],int maximo);
void mover_lasers_enemigos(lasers lasers_malos[],int maximo);
void dibujar_juego(ALLEGRO_BITMAP *fondo,ALLEGRO_BITMAP *img_heroe,float r_x,float ancho,float alto,float tam_dib,float a_x,float a_y);
bool colision(float x,float y);
bool cargar_mapa(const char *nombre_archivo);

int main()
{
    //BORRAR BOOLEANOS Y USAR ALLEGRO_KEYBOARD_STATE PARA DETECTAR TECLAS
    bool corriendo=true;
    bool dibujar=true;
    bool izquierda=false;
    bool derecha=false;
    int i,j;
    float x_bloque,y_bloque;
    float velocidad_caida=0.0;
    float ancho_casilla=128.0;
    float alto_casilla=128.0;
    float r_x=0.0;
    float tamano_dibujo=128.0;
    float ajuste_x=(tamano_dibujo-40.0)/2.0;
    float ajuste_y=tamano_dibujo-40.0;

    inicializar_personaje(&heroe,100,4.0);

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
    ALLEGRO_BITMAP *img_ataque=al_load_bitmap("prota_ataque.png");
    if(!img_ataque)
    {
        printf("Error fatal: No se encontro 'prota_ataque.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_dano=al_load_bitmap("prota_auch.png");
    if(!img_dano)
    {
        printf("Error fatal: No se encontro 'prota_auch.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_caminar=al_load_bitmap("prota_caminar.png");
    if(!img_caminar)
    {
        printf("Error fatal: No se encontro 'prota_caminar.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_correr=al_load_bitmap("prota_corre.png");
    if(!img_correr)
    {
        printf("Error fatal: No se encontro 'prota_corre.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_disparo=al_load_bitmap("prota_disparo.png");
    if(!img_disparo)
    {
        printf("Error fatal: No se encontro 'prota_disparo.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_muerte=al_load_bitmap("prota_muerte.png");
    if(!img_muerte)
    {
        printf("Error fatal: No se encontro 'prota_muerte.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_recarga=al_load_bitmap("prota_recarga.png");
    if(!img_recarga)
    {
        printf("Error fatal: No se encontro 'prota_recarga.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_salto=al_load_bitmap("prota_salto.png");
    if(!img_salto)
    {
        printf("Error fatal: No se encontro 'prota_salto.png\n");
        return -1;
    }
    ALLEGRO_BITMAP *img_idle=al_load_bitmap("prota_Idle.png");
    if(!img_idle)
    {
        printf("Error fatal: No se encontro 'prota_Idle.png\n");
        return -1;
    }


    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
   
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
            mover_personaje(&heroe,izquierda,derecha,&velocidad_caida);

            heroe.estado=ESTADO_IDLE;

            if(izquierda||derecha)
            {
                heroe.estado=ESTADO_CAMINAR;
            }
            if(velocidad_caida!=0)
            {
                heroe.estado=ESTADO_SALTO;
            }
            if(heroe.ataque)
            {
                heroe.estado=ESTADO_DISPARO;
            }

            mover_enemigos(enemigos,total_enemigos);
            mover_lasers(laser_protagonista,MAX_LASERS);
            mover_lasers_enemigos(laser_enemigos,MAX_LASERS);
            dibujar=true;
        }

        if(dibujar&&al_is_event_queue_empty(queue))
        {
            dibujar=false;

            ALLEGRO_BITMAP *img_actual=NULL;

            if(heroe.estado==ESTADO_IDLE) 
            {
                img_actual=img_idle;
            }

            else if(heroe.estado==ESTADO_CAMINAR) 
            {
                img_actual=img_caminar;
            }

            else if(heroe.estado==ESTADO_SALTO) 
            {
                img_actual=img_salto;
            }

            else if(heroe.estado==ESTADO_DISPARO) 
            {
                img_actual=img_disparo;
            }

            else if(heroe.estado==ESTADO_ATAQUE) 
            {
                img_actual=img_ataque;
            }

            else if(heroe.estado==ESTADO_DANO) 
            {
                img_actual=img_dano;
            }

            else if(heroe.estado==ESTADO_CORRER) 
            {
                img_actual=img_correr;
            }

            else if(heroe.estado==ESTADO_RECARGA) 
            {
                img_actual=img_recarga;
            }
            
            dibujar_juego(fondo_nivel1,img_actual,r_x,ancho_casilla,alto_casilla,tamano_dibujo,ajuste_x,ajuste_y);
        }

    }

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}
    

void limites_pantalla(float *x, float *y){
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
    int der=(x+39)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+39)/TAM_TILE;
    
    if(izq<0||der>=20||arr<0||aba>=15)
    {
        return true;
    }

    if (mapa[arr][izq]=='1') 
    {
        return true;
    }

    if (mapa[arr][der]=='1') 
    {
        return true;
    }

    if (mapa[aba][izq]=='1') 
    {
        return true;
    }

    if (mapa[aba][der]=='1') 
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
            fscanf(archivo," %c",&mapa[i][j]);

            if(mapa[i][j]=='*')
            {
                heroe.x=TAM_TILE*j;
                heroe.y=TAM_TILE*i;  
                mapa[i][j]=0;
            }

            else if(mapa[i][j]=='2'||mapa[i][j]=='5'||mapa[i][j]=='9')
            {
                if(total_enemigos<MAX_ENEMIGOS)
                {
                    inicializar_enemigo(&enemigos[total_enemigos],mapa[i][j],TAM_TILE*j,TAM_TILE*i);
                    total_enemigos++;
                }
                mapa[i][j]=0;
            }

            else if(mapa[i][j]=='#')
            {
                amigo.x=TAM_TILE*j;
                amigo.y=TAM_TILE*i;
                mapa[i][j]=0;
            }

            else if(mapa[i][j]=='c')
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

void inicializar_personaje(personaje *entidad,int vida_base,float velocidad_base)
{
    (*entidad).velocidad=velocidad_base;
    (*entidad).vida=vida_base;
    (*entidad).energia=100;
    (*entidad).ataque=false;
}

void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial)
{
    (*villano).tipo=tipo_char;
    (*villano).x=x_inicial;
    (*villano).y=y_inicial;
    (*villano).ataque=false;

    if(tipo_char=='2')
    {
        (*villano).vida=50;
        (*villano).velocidad=0.5;
        (*villano).herida=5;
    }

    if(tipo_char=='5')
    {
        (*villano).vida=100;
        (*villano).velocidad=1.5;
        (*villano).herida=15;
    }

    if(tipo_char=='9')
    {
        (*villano).vida=250;
        (*villano).velocidad=3.0;
        (*villano).herida=35;
    }
}

void mover_personaje(personaje *p,bool izq, bool der,float *vel_caida)
{
    if(izq)
    {
        (*p).x-=4.0;
        if(colision((*p).x,(*p).y))
        {
            (*p).x+=4.0;
        }
    }

    if(der)
    {
        (*p).x+=4.0;

        if(colision((*p).x,(*p).y))
        {
            (*p).x-=4.0;
        }
    }

    *vel_caida+=0.5;
    (*p).y+=*vel_caida;

    if(colision((*p).x,(*p).y))
    {
        if(*vel_caida>0)
        {
            (*p).y=((int)((*p).y+39)/TAM_TILE)*TAM_TILE-39.1;
        }

        else if (*vel_caida<0) 
        {
            (*p).y=((int)(*p).y/TAM_TILE)*TAM_TILE+TAM_TILE+0.1;
        }

        *vel_caida=0;
    }
}

void mover_enemigos(enemigo ejercito[],int total)
{
    for(int i=0;i<total;i++)
    {
        if(ejercito[i].velocidad!=0)
        {
            ejercito[i].x+=ejercito[i].velocidad;

            if(colision(ejercito[i].x, ejercito[i].y))
            {
                ejercito[i].velocidad=-ejercito[i].velocidad;
                ejercito[i].x+=ejercito[i].velocidad;
            }

            ejercito[i].y+=4.0;

            if(colision(ejercito[i].x,ejercito[i].y))
            {
                ejercito[i].y=((int)(ejercito[i].y+39)/TAM_TILE)*TAM_TILE-39.1;
            }
        }
    }
}

void mover_lasers(lasers mis_lasers[],int maximo)
{
    int i;

    for(i=0;i<maximo;i++)
    {
        if(mis_lasers[i].activo) 
        {
            mis_lasers[i].x+=mis_lasers[i].velocidad;

            if(colision(mis_lasers[i].x, mis_lasers[i].y)||mis_lasers[i].x>800||mis_lasers[i].x<0)
            {
                mis_lasers[i].activo=false;
            }
        }
    }
}

void mover_lasers_enemigos(lasers lasers_malos[],int maximo)
{
    int i;

    for(i=0;i<maximo;i++)
    {
        if(lasers_malos[i].activo)
        {
            lasers_malos[i].x+=lasers_malos[i].velocidad;
            
            if(colision(lasers_malos[i].x,lasers_malos[i].y)||lasers_malos[i].x>800||lasers_malos[i].x<0)
            {
                lasers_malos[i].activo=false;
            }
        }
    }
}

void dibujar_juego(ALLEGRO_BITMAP *fondo,ALLEGRO_BITMAP *img_heroe,float r_x,float ancho,float alto,float tam_dib,float a_x,float a_y)
{
    int i,j;
    float x_bloque,y_bloque;

    al_clear_to_color(al_map_rgb(20, 30, 50));
    al_draw_scaled_bitmap(fondo,0,0,al_get_bitmap_width(fondo),al_get_bitmap_height(fondo),0,0,columna*TAM_TILE,fila*TAM_TILE,0);

    for(i=0;i<fila;i++)
    {
        for(j=0;j<columna;j++)
        {
            if(mapa[i][j]=='1')
            {
                x_bloque=j*TAM_TILE;
                y_bloque=i*TAM_TILE; 
                al_draw_filled_rectangle(x_bloque, y_bloque, x_bloque + TAM_TILE, y_bloque + TAM_TILE, al_map_rgb(100, 100, 100));
                al_draw_rectangle(x_bloque, y_bloque, x_bloque + TAM_TILE, y_bloque + TAM_TILE, al_map_rgb(50, 50, 60), 1);
            }
        }
    }

    if(heroe.vida>0&&img_heroe!=NULL)
    {
        al_draw_scaled_bitmap(img_heroe,r_x,0,ancho,alto,heroe.x-a_x,heroe.y-a_y,tam_dib,tam_dib,0);
    }

    for(i=0;i<total_enemigos;i++)
    {
        if(enemigos[i].tipo=='2') 
        {
            al_draw_filled_rectangle(enemigos[i].x,enemigos[i].y,enemigos[i].x+40,enemigos[i].y+40, al_map_rgb(50, 200, 50));
        }

        if(enemigos[i].tipo=='5') 
        {
            al_draw_filled_rectangle(enemigos[i].x,enemigos[i].y,enemigos[i].x+40,enemigos[i].y+40,al_map_rgb(200, 50, 200));
        }

        if(enemigos[i].tipo=='9') 
        {
            al_draw_filled_rectangle(enemigos[i].x,enemigos[i].y,enemigos[i].x+40,enemigos[i].y+40,al_map_rgb(50, 50, 200));
        }
    }

    al_flip_display();
}