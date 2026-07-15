#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<allegro5/allegro.h> 
#include<allegro5/allegro_image.h> 
#include<allegro5/allegro_font.h> 
#include<allegro5/allegro_ttf.h> //fuentes ttf
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_audio.h> 
#include<allegro5/allegro_acodec.h> //audio codecs
#define TAM_TILE 40
#define fila 27
#define columna 48
#define alto_pantalla 1080
#define ancho_pantalla 1920
#define MAX_ENEMIGOS 4
#define MAX_LASERS 2
#define ANCHO_SPRITE 128.0
#define ALTO_SPRITE 128.0
#define TAM_DIBUJO 80.0
#define AJUSTE_X ((TAM_DIBUJO - 40.0) / 2.0)
#define AJUSTE_Y (TAM_DIBUJO - 40.0)
#define MAX_ITEMS 60
#define ANCHO_TEXTURA_LUNAR 10
#define ALTO_TEXTURA_LUNAR 10
#define MAX_COSAS 5
#define MAX_TRAMPAS 15

enum Estado_Personaje
{
    ESTADO_IDLE = 0, 
    ESTADO_CAMINAR,
    ESTADO_SALTO,
    ESTADO_DISPARO,
    ESTADO_ATAQUE,
    ESTADO_DANO,
    ESTADO_CORRER,
    ESTADO_RECARGA,
    ESTADO_MUERTE
};

enum Tipo_Item
{
    ITEM_VIDA=1,
    ITEM_MUNICION,
    ITEM_ENERGIA,
    ITEM_LLAVE_FINAL,
    ITEM_LLAVE_COMUN,
    ITEM_VENENO,
    ITEM_PUNTOS
};

struct arma_
{
    int prejuicio;
    int alcance;
    int energia;
    bool en_uso;
};
typedef struct arma_ arma;

struct proyectil_
{
    float x;
    float y;
    float velocidad_x;
    float velocidad_y;
    bool activo;
};
typedef struct proyectil_ proyectil;

struct personaje_
{
    float x;
    float y;
    float velocidad;
    int vida;
    int energia;
    int estado;
    int tiempo_dano;
    int cuadro_actual;
    int contador_animacion;
    int municion;
    int inventario[MAX_COSAS];
    int puntos;
    bool ataque;
    arma armas;
    proyectil proyectiles[MAX_ITEMS];
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
    int estado;
    int cuadro_actual;
    int contador_animacion; 
    int municion;
    float velocidad;
    float direccion_mirada;
    bool ataque;
    //lasers laser_enemigos[MAX_LASERS];
};
typedef struct enemigo_ enemigo;

struct trampas_
{
    float x;
    float y;
    bool activado;
};
typedef struct trampas_ trampas;

struct item_
{
    float x;
    float y;
    float velocidad_y;
    enum Tipo_Item tipo;
    int valor; 
    bool activo;
    
};
typedef struct item_ item;

struct lasers_
{
    float x;
    float y;
    float velocidad;
    bool activo;
};
typedef struct lasers_ lasers;

struct recursos_
{
    ALLEGRO_BITMAP *fondo_nivel1;
    ALLEGRO_BITMAP *img_ataque;
    ALLEGRO_BITMAP *img_dano;
    ALLEGRO_BITMAP *img_caminar;
    ALLEGRO_BITMAP *img_correr;
    ALLEGRO_BITMAP *img_disparo;
    ALLEGRO_BITMAP *img_muerte;
    ALLEGRO_BITMAP *img_recarga;
    ALLEGRO_BITMAP *img_salto;
    ALLEGRO_BITMAP *img_idle;
    ALLEGRO_BITMAP *img_basico;
    ALLEGRO_BITMAP *textura_luna;
    ALLEGRO_BITMAP *textura_metal;
    ALLEGRO_BITMAP *pocion_vida;
    ALLEGRO_BITMAP *alien5_idle;
    ALLEGRO_BITMAP *alien5_caminar;
    ALLEGRO_BITMAP *alien5_ataque;
    ALLEGRO_BITMAP *alien5_dead;
    ALLEGRO_BITMAP *veneno;
    ALLEGRO_BITMAP *llave_final;
    ALLEGRO_BITMAP *llave_basica;
    ALLEGRO_BITMAP *portal;
    ALLEGRO_BITMAP *punto;
    ALLEGRO_BITMAP *puas;
};
typedef struct recursos_ recursos;

struct contexto_dibujo_
{
    int espejo;
    int cuadro_moneda;
    int cuadro_portal;
    float r_x;
    recursos *catalogo;
    ALLEGRO_BITMAP *img_heroe; 
    ALLEGRO_FONT *fuente;
};
typedef struct contexto_dibujo_ contexto_dibujo;

char mapa[fila][columna];
int total_enemigos=0;
float x_portal=0;
float y_portal=0;
bool portal_activo = false;
personaje heroe;
personaje amigo;
enemigo enemigos[MAX_ENEMIGOS];
lasers laser_protagonista[MAX_LASERS];
lasers laser_enemigos[MAX_LASERS];
item mis_items[MAX_ITEMS];
trampas mis_trampas[MAX_TRAMPAS];

void limites_pantalla(float *x, float *y);
void inicializar_personaje(personaje *entidad,int vida_base,float velocidad_base);
void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial);
void mover_personaje(personaje *p,bool izq, bool der,float *vel_caida);
void mover_enemigos(enemigo ejercito[],int total);
void mover_lasers(lasers mis_lasers[],int maximo);
void mover_lasers_enemigos(lasers lasers_malos[],int maximo);
void dibujar_juego(contexto_dibujo *graficos);
void recolectar_items(item lista_items[],int maximo);
void mover_proyectiles(proyectil mis_proyectiles[], int maximo);
bool colision_proyectil(float x,float y);
bool colision(float x,float y);
bool cargar_mapa(const char *nombre_archivo);
bool colision_lasers(float x,float y);
bool cargar_imagenes(recursos *imgs);

int main()
{
    bool corriendo=true;
    bool dibujar=true;
    int i,j,k,casilla_x,casilla_y;
    int voltear=0;
    int max_cuadros_heroe=5;
    int cuadro_animacion_moneda=0;
    int contador_animacion_moneda=0;
    int cuadro_animacion_portal=0;
    int contador_animacion_portal=0;
    float x_bloque,y_bloque;
    float velocidad_caida=0.0;
    float r_x=0.0;
    float direccion_mirada=1.0;

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

    ALLEGRO_FONT *fuente_texto=al_create_builtin_font();

    recursos mis_imagenes;
    if(!cargar_imagenes(&mis_imagenes))
    {
        return -1;
    }

    al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_MAXIMIZED | ALLEGRO_FULLSCREEN_WINDOW);
    ALLEGRO_DISPLAY *display = al_create_display(ancho_pantalla,alto_pantalla);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

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
            case ALLEGRO_KEY_UP:
                if(colision(heroe.x,heroe.y+1.0))
                {
                    velocidad_caida=-9.0;
                }
                break;

            case ALLEGRO_KEY_SPACE:
                heroe.ataque=true;
                for(i=0;i<MAX_LASERS;i++)
                {
                    if(laser_protagonista[i].activo==false)
                    {
                        laser_protagonista[i].activo=true;
                        laser_protagonista[i].x=heroe.x+20;
                        laser_protagonista[i].y=heroe.y+15;
                        laser_protagonista[i].velocidad=15.0*direccion_mirada;
                        break;
                    }
                }
                break;
            
            case ALLEGRO_KEY_V:
                int bolsillo, k;
                for(bolsillo=0; bolsillo<MAX_COSAS; bolsillo++) 
                {
                    if(heroe.inventario[bolsillo] == ITEM_VENENO)
                    {
                        for(k=0; k<MAX_ITEMS; k++) 
                        {
                            if(!heroe.proyectiles[k].activo) 
                            {
                                heroe.proyectiles[k].activo=true;
                                heroe.proyectiles[k].x=heroe.x+20;
                                heroe.proyectiles[k].y=heroe.y+10;
                                heroe.proyectiles[k].velocidad_x=12.0*direccion_mirada;
                                heroe.proyectiles[k].velocidad_y=-8.0;
                                heroe.inventario[bolsillo] = 0;
                                printf("Veneno lanzado! Se vació el bolsillo\n");
                                break; 
                            }
                        }
                        break; 
                    }
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
            case ALLEGRO_KEY_SPACE:
                heroe.ataque=false;
                break;
            }
        }

        else if(evento.type==ALLEGRO_EVENT_TIMER)
        {
            ALLEGRO_KEYBOARD_STATE estado_teclado;
            al_get_keyboard_state(&estado_teclado);

            bool tecla_izq=al_key_down(&estado_teclado, ALLEGRO_KEY_LEFT);
            bool tecla_der=al_key_down(&estado_teclado,ALLEGRO_KEY_RIGHT);

            if(tecla_der)
            {
                direccion_mirada=1.0;
            }
            if(tecla_izq)
            {
                direccion_mirada=-1.0;
            }

            if(heroe.vida>0)
            {
                mover_personaje(&heroe,tecla_izq,tecla_der,&velocidad_caida);
            }

            if(heroe.tiempo_dano>0)
            {
                heroe.tiempo_dano--; 
                heroe.estado=ESTADO_DANO; 
            }

            else if(heroe.vida<=0)
            {
                heroe.estado=ESTADO_MUERTE; 
            }

            else
            {
                heroe.estado=ESTADO_IDLE;

                if(tecla_izq||tecla_der)
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
            }

            heroe.contador_animacion++;
            if(heroe.contador_animacion>=5) 
            {
                heroe.contador_animacion=0;
                heroe.cuadro_actual++;               
                
                if(heroe.cuadro_actual>=max_cuadros_heroe)
                {
                    if(heroe.estado==ESTADO_MUERTE)
                    {
                        heroe.cuadro_actual=max_cuadros_heroe-1;
                    }
                    else
                    {
                        heroe.cuadro_actual=0; 
                    }
                }
            }

            r_x = heroe.cuadro_actual*ANCHO_SPRITE;

            mover_enemigos(enemigos,total_enemigos);
            mover_lasers(laser_protagonista,MAX_LASERS);
            mover_lasers_enemigos(laser_enemigos,MAX_LASERS);
            mover_proyectiles(heroe.proyectiles,MAX_ITEMS);
            recolectar_items(mis_items,MAX_ITEMS);

            for(i=0;i<20;i++)
            {
                if(mis_trampas[i].activado)
                {
                    if(heroe.x<mis_trampas[i].x+TAM_TILE&&heroe.x+40>mis_trampas[i].x&&heroe.y<mis_trampas[i].y+TAM_TILE&&heroe.y+40>mis_trampas[i].y)
                    {
                        if(heroe.tiempo_dano==0)
                        {
                            heroe.vida-=20;
                            heroe.tiempo_dano=30;
                            velocidad_caida=-6.0; 
                            printf("Caiste en las púas! Vida: %d\n",heroe.vida);
                        }
                    }
                }
            }

            if(portal_activo)
            {
                if(heroe.x<x_portal+TAM_TILE&&heroe.x+40>x_portal&&heroe.y<y_portal+TAM_TILE&&heroe.y+40>y_portal-TAM_TILE)
                {
                    printf("Portal alcanzado! Nivel completado.\n");
                    corriendo=false;
                }
            }

            dibujar=true;
        }

        if(dibujar&&al_is_event_queue_empty(queue))
        {
            dibujar=false;

            ALLEGRO_BITMAP *img_actual=NULL;

            if(heroe.estado==ESTADO_IDLE) 
            {
                img_actual=mis_imagenes.img_idle;
            }
            else if(heroe.estado==ESTADO_CAMINAR) 
            {
                img_actual=mis_imagenes.img_caminar;
            }
            else if(heroe.estado==ESTADO_SALTO) 
            {
                img_actual=mis_imagenes.img_salto;
            }
            else if(heroe.estado==ESTADO_DISPARO) 
            {
                img_actual=mis_imagenes.img_disparo;
            }
            else if(heroe.estado==ESTADO_ATAQUE) 
            {
                img_actual=mis_imagenes.img_ataque;
            }
            else if(heroe.estado==ESTADO_DANO) 
            {
                img_actual=mis_imagenes.img_dano;
            }
            else if(heroe.estado==ESTADO_CORRER) 
            {
                img_actual=mis_imagenes.img_correr;
            }
            else if(heroe.estado==ESTADO_RECARGA) 
            {
                img_actual=mis_imagenes.img_recarga;
            }
            else if(heroe.estado==ESTADO_MUERTE)
            {
                img_actual=mis_imagenes.img_muerte;
            }

            if(direccion_mirada==-1.0)
            {
                voltear=ALLEGRO_FLIP_HORIZONTAL;
            }
            else
            {
                voltear=0;
            }

            contexto_dibujo mis_graficos;
            mis_graficos.catalogo=&mis_imagenes;
            mis_graficos.img_heroe=img_actual;
            mis_graficos.r_x=r_x;
            mis_graficos.espejo=voltear;
            mis_graficos.cuadro_moneda=cuadro_animacion_moneda; 
            mis_graficos.cuadro_portal=cuadro_animacion_portal;
            mis_graficos.fuente=fuente_texto;
            
            dibujar_juego(&mis_graficos);
        }

    }

    al_destroy_bitmap(mis_imagenes.fondo_nivel1);
    al_destroy_bitmap(mis_imagenes.img_ataque);
    al_destroy_bitmap(mis_imagenes.img_dano);
    al_destroy_bitmap(mis_imagenes.img_caminar);
    al_destroy_bitmap(mis_imagenes.img_correr);
    al_destroy_bitmap(mis_imagenes.img_disparo);
    al_destroy_bitmap(mis_imagenes.img_muerte);
    al_destroy_bitmap(mis_imagenes.img_recarga);
    al_destroy_bitmap(mis_imagenes.img_salto);
    al_destroy_bitmap(mis_imagenes.img_idle);
    al_destroy_bitmap(mis_imagenes.img_basico);
    al_destroy_bitmap(mis_imagenes.pocion_vida);
    al_destroy_bitmap(mis_imagenes.textura_luna);
    al_destroy_bitmap(mis_imagenes.textura_metal);
    al_destroy_bitmap(mis_imagenes.alien5_idle);
    al_destroy_font(fuente_texto);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}
    
void limites_pantalla(float *x, float *y){
    if(*x<0)
    {
        *x=0;
    }
    if(*x>ancho_pantalla)
    {
        *x=ancho_pantalla;
    }
    if(*y<0)
    {
        *y=0;
    }
    if(*y>alto_pantalla)
    {
        *y=alto_pantalla;
    }
}

bool colision(float x,float y)
{
    int izq=x/TAM_TILE;
    int der=(x+39)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+39)/TAM_TILE;
    
    if(izq<0||der>=columna||arr<0||aba>=fila)
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
    int i,j,k;
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
            else if(mapa[i][j]=='v')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!mis_items[k].activo)
                    {
                        mis_items[k].activo=true;
                        mis_items[k].tipo=ITEM_VENENO;
                        mis_items[k].x=TAM_TILE*j+5;
                        mis_items[k].y=TAM_TILE*i+10;
                        mis_items[k].velocidad_y=0;
                        mis_items[k].valor=20;
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='k')
            {
                for(k=0;k<MAX_ITEMS;k++)
                {
                    if(!mis_items[k].activo)
                    {
                        mis_items[k].activo=true;
                        mis_items[k].tipo=ITEM_LLAVE_COMUN;
                        mis_items[k].x=TAM_TILE*j+5;
                        mis_items[k].y=TAM_TILE*i+10;
                        mis_items[k].velocidad_y=0;
                        mis_items[k].valor=1;
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='p')
            {
                for(k=0;k<MAX_TRAMPAS;k++)
                {
                    if(!mis_trampas[k].activado)
                    {
                        mis_trampas[k].activado=true;
                        mis_trampas[k].x=TAM_TILE*j;
                        mis_trampas[k].y=TAM_TILE*i;
                        break;
                    }
                }
                mapa[i][j]=0;
            }
            else if(mapa[i][j]=='O') 
            {
                portal_activo = true;
                x_portal = TAM_TILE * j;
                y_portal = TAM_TILE * i;
                mapa[i][j] = 0; 
            }
            else if(mapa[i][j]=='.') 
            {
                for(k=0; k<MAX_ITEMS; k++)
                {
                    if(!mis_items[k].activo)
                    {
                        mis_items[k].activo=true;
                        mis_items[k].tipo=ITEM_PUNTOS;
                        mis_items[k].x=TAM_TILE*j+5;
                        mis_items[k].y=TAM_TILE*i+10;
                        mis_items[k].velocidad_y=0;
                        mis_items[k].valor=10; 
                        break;
                    }
                }
                mapa[i][j]=0;
            }
        }
    }

    fclose(archivo);
    return true;
}

void inicializar_personaje(personaje *entidad,int vida_base,float velocidad_base)
{
    int i;

    (*entidad).velocidad=velocidad_base;
    (*entidad).vida=vida_base;
    (*entidad).energia=100;
    (*entidad).ataque=false;
    (*entidad).tiempo_dano=0;
    (*entidad).contador_animacion=0;
    (*entidad).cuadro_actual=0;
    (*entidad).puntos=0;
    for(i=0;i<MAX_COSAS;i++)
    {
        (*entidad).inventario[i] = 0;
    }
    for(i=0;i<MAX_ITEMS;i++)
    {
        (*entidad).proyectiles[i].activo=false;
    }
}

void inicializar_enemigo(enemigo *villano,int tipo_char,float x_inicial,float y_inicial)
{
    (*villano).tipo=tipo_char;
    (*villano).x=x_inicial;
    (*villano).y=y_inicial;
    (*villano).ataque=false;
    (*villano).estado=ESTADO_CAMINAR;
    (*villano).cuadro_actual=0;
    (*villano).contador_animacion=0;
    (*villano).direccion_mirada=1.0;

    if(tipo_char=='2')
    {
        (*villano).vida=50;
        (*villano).velocidad=0.5;
        (*villano).herida=5;
        (*villano).alcance=50;
    }

    if(tipo_char=='5')
    {
        (*villano).vida=100;
        (*villano).velocidad=1.0;
        (*villano).herida=15;
        (*villano).alcance=400;
        (*villano).energia=0;
    }

    if(tipo_char=='9')
    {
        (*villano).vida=250;
        (*villano).velocidad=3.0;
        (*villano).herida=35;
        (*villano).alcance=100;
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
    int max_cuadros=8;
    int limite_cuadros=8;
    int i,j,frente_x,pies_y; 
    float dist_x_abs;
    float dist_y_abs;
    bool puede_moverse;

    for(i = 0; i < total; i++)
    {
        if(ejercito[i].vida <= 0) 
        {
            ejercito[i].estado = ESTADO_MUERTE;
            ejercito[i].velocidad = 0; 
            ejercito[i].ataque = false;
        }
        else
        {
            float dist_x=heroe.x-ejercito[i].x;
            float dist_y=heroe.y-ejercito[i].y;

            if (dist_x>0)
            {
                dist_x_abs=dist_x;
            }
            else
            {
                dist_x_abs=-dist_x;
            }

            if (dist_y>0)
            {
                dist_y_abs=dist_y;
            }
            else
            {
                dist_y_abs=-dist_y;
            }

            bool heroe_en_rango=(heroe.vida>0&&dist_x_abs<=ejercito[i].alcance&&dist_y_abs<40.0);

            if(ejercito[i].tipo=='2'||ejercito[i].tipo=='9')
            {
                if(heroe_en_rango)
                {
                    ejercito[i].ataque=true;
                    if (dist_x>0)
                    {
                        ejercito[i].direccion_mirada=1.0;
                    }
                    else
                    {
                        ejercito[i].direccion_mirada=-1.0;
                    }
                }
                else
                {
                    ejercito[i].ataque=false;
                }
            }
            
            else if(ejercito[i].tipo=='5')
            {
                if(heroe_en_rango)
                {
                    ejercito[i].ataque=true;
                    if (dist_x>0)
                    {
                        ejercito[i].direccion_mirada=1.0;
                    }
                    else
                    {
                        ejercito[i].direccion_mirada=-1.0;
                    }
            
                    if(ejercito[i].energia<=0)
                    {
                        ejercito[i].ataque=true;
                    }
                }
                else
                {
                    if(ejercito[i].energia>0)
                    {
                        ejercito[i].ataque=false;
                    }
                }
                
                if(ejercito[i].ataque) 
                {
                    if(ejercito[i].cuadro_actual==3&&ejercito[i].contador_animacion==0)
                    {
                        for(j= 0;j<MAX_LASERS;j++)
                        {
                            if(!laser_enemigos[j].activo)
                            {
                                laser_enemigos[j].activo=true;
                                laser_enemigos[j].x=ejercito[i].x+20;
                                laser_enemigos[j].y=ejercito[i].y-5;
                                laser_enemigos[j].velocidad=10.0*ejercito[i].direccion_mirada;
                                break;
                            }
                        }
                        ejercito[i].energia=90;
                        ejercito[i].ataque=false; 
                    }
                }
                else
                {   
                    if(ejercito[i].energia>0) 
                    {
                        ejercito[i].energia--;
                    }
                }

                if(ejercito[i].velocidad > 0) 
                {
                    frente_x=(ejercito[i].x+45)/TAM_TILE; 
                }
                else 
                {
                    frente_x=(ejercito[i].x-5)/TAM_TILE; 
                }
                
                pies_y=(ejercito[i].y + 45)/TAM_TILE; 
                
                if(pies_y<fila&&frente_x>=0&&frente_x<columna)
                {
                    if(mapa[pies_y][frente_x]!='1')
                    {
                        ejercito[i].velocidad=-ejercito[i].velocidad;
                    }
                }
            }

            if(ejercito[i].ataque) 
            {
                ejercito[i].estado=ESTADO_ATAQUE;
            } 
            else if(ejercito[i].velocidad!=0) 
            {
                ejercito[i].estado=ESTADO_CAMINAR;
                if(ejercito[i].velocidad>0) 
                {
                    ejercito[i].direccion_mirada=1.0;
                }
                if(ejercito[i].velocidad < 0) 
                {
                    ejercito[i].direccion_mirada=-1.0;
                }
            } 
            else 
            {
                ejercito[i].estado=ESTADO_IDLE;
            }

            puede_moverse=true;
            if(ejercito[i].tipo=='5'&&ejercito[i].ataque) 
            {
                puede_moverse = false; 
            }

            if(ejercito[i].ataque)
            {
                ejercito[i].estado=ESTADO_ATAQUE;
            }
            else if(ejercito[i].velocidad!=0&&puede_moverse)
            {
                ejercito[i].estado=ESTADO_CAMINAR;
                if(ejercito[i].velocidad>0) 
                {
                    ejercito[i].direccion_mirada=1.0;
                }
                if(ejercito[i].velocidad<0) 
                {
                    ejercito[i].direccion_mirada=-1.0;
                }

                ejercito[i].x+=ejercito[i].velocidad;

                if(colision(ejercito[i].x, ejercito[i].y))
                {
                    ejercito[i].x -= ejercito[i].velocidad;
                    ejercito[i].velocidad = -ejercito[i].velocidad; 
                }
            }
            else
            {
                ejercito[i].estado=ESTADO_IDLE;
            }

            if(ejercito[i].tipo=='5')
            {
                if(ejercito[i].estado==ESTADO_IDLE) 
                {
                    limite_cuadros=3;
                }
                else if(ejercito[i].estado==ESTADO_ATAQUE) 
                {
                    limite_cuadros=4;
                }
                else if(ejercito[i].estado==ESTADO_CAMINAR) 
                {
                    limite_cuadros=6;
                }
                else if(ejercito[i].estado==ESTADO_MUERTE) 
                {
                    limite_cuadros=5;
                }
            }

            if(ejercito[i].cuadro_actual>=limite_cuadros)
            {
                ejercito[i].cuadro_actual=0;
            }

            ejercito[i].y+=4.0;
            if(colision(ejercito[i].x, ejercito[i].y))
            {
                ejercito[i].y=((int)(ejercito[i].y + 39)/TAM_TILE)*TAM_TILE-39.1;
            }

            if(ejercito[i].estado==ESTADO_ATAQUE&&ejercito[i].cuadro_actual==4&&ejercito[i].contador_animacion==0)
            {
                if(heroe.tiempo_dano==0&&(ejercito[i].tipo=='2'||ejercito[i].tipo=='9'))
                {
                    heroe.vida-=ejercito[i].herida;
                    heroe.tiempo_dano=30;
                }
            }
        }

        ejercito[i].contador_animacion++;

        if(ejercito[i].contador_animacion>=6) 
        {
            ejercito[i].contador_animacion=0;
            ejercito[i].cuadro_actual++;
            
            if(ejercito[i].estado==ESTADO_IDLE) 
            {
                max_cuadros=3;
            }
            else if(ejercito[i].estado==ESTADO_MUERTE) 
            {
                max_cuadros=5;
            }
            else if(ejercito[i].estado==ESTADO_ATAQUE)
            {
                max_cuadros=4;
            }
            else if(ejercito[i].estado==ESTADO_CAMINAR)
            {
                max_cuadros=6;
            }

            if(ejercito[i].tipo=='5'&&ejercito[i].estado==ESTADO_IDLE)
            {
                max_cuadros=3;
            }
            
            if(ejercito[i].cuadro_actual>=max_cuadros)
            {
                if(ejercito[i].estado==ESTADO_MUERTE) 
                {
                    ejercito[i].cuadro_actual=max_cuadros-1; 
                } 
                else 
                {
                    ejercito[i].cuadro_actual=0;
                }
            }
        }
    }    
}

void mover_lasers(lasers mis_lasers[],int maximo)
{
    int i,j,k;

    for(i=0;i<maximo;i++)
    {
        if(mis_lasers[i].activo) 
        {
            mis_lasers[i].x+=mis_lasers[i].velocidad;

            if(colision_lasers(mis_lasers[i].x, mis_lasers[i].y)||mis_lasers[i].x>ancho_pantalla||mis_lasers[i].x<0)
            {
                mis_lasers[i].activo=false;
            }

            if(mis_lasers[i].activo)
            {
                for(j=0;j<total_enemigos;j++)
                {
                    if(enemigos[j].vida>0)
                    {
                        if(mis_lasers[i].x<enemigos[j].x+40&&mis_lasers[i].x+15>enemigos[j].x&&mis_lasers[i].y<enemigos[j].y+40&&mis_lasers[i].y+5>enemigos[j].y)
                        {
                            enemigos[j].vida-=25;
                            mis_lasers[i].activo=false;
                            printf("Impacto al enemigo, vida restante: %d\n",enemigos[j].vida);
                            if(enemigos[j].vida<=0&&enemigos[j].tipo=='5')
                            {
                                heroe.puntos+=50;
                                for(k=0;k<MAX_ITEMS;k++)
                                {
                                    if(!mis_items[k].activo)
                                    {
                                        mis_items[k].activo=true;
                                        mis_items[k].tipo=ITEM_VIDA;
                                        mis_items[k].x=enemigos[j].x+10;
                                        mis_items[k].y=enemigos[j].y;
                                        mis_items[k].velocidad_y=-8.0;
                                        mis_items[k].valor=25;
                                        printf("El enemigo solto una vida!\n");
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                    } 
                }
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
            
            if(colision_lasers(lasers_malos[i].x,lasers_malos[i].y)||lasers_malos[i].x>ancho_pantalla||lasers_malos[i].x<0)
            {
                lasers_malos[i].activo=false;
            }
            
            if(lasers_malos[i].activo && heroe.vida > 0)
            {
                if(lasers_malos[i].x<heroe.x+40&&lasers_malos[i].x+15>heroe.x&&lasers_malos[i].y<heroe.y+40&&lasers_malos[i].y+5>heroe.y-40)
                {
                    if(heroe.tiempo_dano==0) 
                    {
                        heroe.vida-=15;
                        heroe.tiempo_dano=30; 
                        printf("Te han disparado! Vida restante: %d\n",heroe.vida);
                    }
                    lasers_malos[i].activo = false; 
                }
            }
        }
    }
}

void dibujar_juego(contexto_dibujo *graficos)
{
    int i,j,ancho_alien5,alto_alien5,recorte_x,recorte_y,recorte_portal_x,recorte_moneda_x;
    int espejo_enemigo=0;
    float x_bloque,y_bloque,ajuste_y_alien5;
    float ancho_dibujo=60.0;
    float alto_dibujo=85.0;
    float ajuste_x=(ancho_dibujo-40.0)/2.0; 
    float ajuste_y=alto_dibujo-40.0;
    float ajuste_alien_x=(64.0-40.0)/2.0;
    float ajuste_alien_y=10.0;

    recursos *imgs=(*graficos).catalogo;

    al_clear_to_color(al_map_rgb(20, 30, 50));
    if((*imgs).fondo_nivel1!=NULL)
    {
        al_draw_scaled_bitmap((*imgs).fondo_nivel1,0,0,al_get_bitmap_width((*imgs).fondo_nivel1),al_get_bitmap_height((*imgs).fondo_nivel1),0,0,columna*TAM_TILE,fila*TAM_TILE,0);
    }

    for(i=0;i<fila;i++)
    {
        for(j=0;j<columna;j++)
        {
            x_bloque=j*TAM_TILE;
            y_bloque=i*TAM_TILE; 

            if(mapa[i][j]=='1')
            {
                
                if((*imgs).textura_luna!=NULL)
                {
                    al_draw_scaled_bitmap((*imgs).textura_luna,209,359,ANCHO_TEXTURA_LUNAR,ALTO_TEXTURA_LUNAR,x_bloque,y_bloque,TAM_TILE,TAM_TILE, 0);
                }
            }
        }
    }

    for(i=0;i<20;i++)
    {
        if(mis_trampas[i].activado&&(*imgs).puas!=NULL)
        {
            al_draw_scaled_bitmap((*imgs).puas,0,0,al_get_bitmap_width((*imgs).puas),al_get_bitmap_height((*imgs).puas),mis_trampas[i].x,mis_trampas[i].y,TAM_TILE,TAM_TILE,0);
        }
    }

    if((*graficos).img_heroe!=NULL)
    {
        al_draw_scaled_bitmap((*graficos).img_heroe,(*graficos).r_x,0,ANCHO_SPRITE,ALTO_SPRITE,heroe.x-AJUSTE_X,heroe.y-AJUSTE_Y,TAM_DIBUJO,TAM_DIBUJO,(*graficos).espejo);
    }

    for(i=0;i<total_enemigos;i++)
    {
        if(enemigos[i].tipo=='2') 
        { 
            if((*imgs).img_basico!=NULL)
            {
                int espejo_enemigo=0;
                int fila_y=0;
                if(enemigos[i].direccion_mirada==-1.0)
                {
                    espejo_enemigo=ALLEGRO_FLIP_HORIZONTAL;
                }

                if(enemigos[i].estado==ESTADO_IDLE) 
                {
                    fila_y=0;
                }

                else if(enemigos[i].estado==ESTADO_CAMINAR) 
                {
                    fila_y=1;
                }

                else if(enemigos[i].estado==ESTADO_ATAQUE) 
                {
                    fila_y=2;
                }

                else if(enemigos[i].estado==ESTADO_DANO) 
                {
                    fila_y=3;
                }

                else if(enemigos[i].estado==ESTADO_MUERTE) 
                {
                    fila_y=4;
                }

                recorte_x=enemigos[i].cuadro_actual*64;
                recorte_y=fila_y*64;

                al_draw_scaled_bitmap((*imgs).img_basico,recorte_x,recorte_y,64,64,enemigos[i].x-ajuste_alien_x,enemigos[i].y-ajuste_alien_y,64,64,espejo_enemigo);
            }
        }

        if(enemigos[i].tipo=='5') 
        {
            ajuste_y_alien5=ajuste_y;
            ALLEGRO_BITMAP *img_a_dibujar=NULL;
            if (enemigos[i].estado==ESTADO_IDLE)
            {
                img_a_dibujar=(*imgs).alien5_idle;
                ajuste_y_alien5=ajuste_y+1;
                ancho_alien5=235;
                alto_alien5=440;
            }
            else if(enemigos[i].estado==ESTADO_CAMINAR)
            {
                img_a_dibujar=(*imgs).alien5_caminar;
                ajuste_y_alien5=ajuste_y+1;
                ancho_alien5=200;
                alto_alien5=442;
            }
            else if(enemigos[i].estado==ESTADO_MUERTE)
            {
                img_a_dibujar=(*imgs).alien5_dead;
                ajuste_y_alien5=ajuste_y-20;
                ancho_alien5=413;
                alto_alien5=410;
            }
            else if(enemigos[i].estado==ESTADO_ATAQUE)
            {
                img_a_dibujar=(*imgs).alien5_ataque;
                ajuste_y_alien5=ajuste_y+1;
                ancho_alien5=534;
                alto_alien5=428;
            }

            if(img_a_dibujar!=NULL)
            {
                espejo_enemigo=0;
                if(enemigos[i].direccion_mirada==-1.0)
                {
                    espejo_enemigo=ALLEGRO_FLIP_HORIZONTAL;
                }
                recorte_x=enemigos[i].cuadro_actual*ancho_alien5;
                al_draw_scaled_bitmap(img_a_dibujar,recorte_x,0,ancho_alien5,alto_alien5,enemigos[i].x-ajuste_x,enemigos[i].y-ajuste_y_alien5,ancho_dibujo,alto_dibujo,espejo_enemigo);
            } 
        }

        if(enemigos[i].tipo=='9') 
        {
            al_draw_filled_rectangle(enemigos[i].x,enemigos[i].y,enemigos[i].x+40,enemigos[i].y+40,al_map_rgb(50, 50, 200));
        }
    }

    for(i=0;i<MAX_LASERS;i++)
    {
        if(laser_protagonista[i].activo)
        {
            al_draw_filled_rectangle(laser_protagonista[i].x,laser_protagonista[i].y,laser_protagonista[i].x+15,laser_protagonista[i].y+5,al_map_rgb(255,0,0));
        }
    }
    for(i=0;i<MAX_LASERS;i++)
    {
        if(laser_enemigos[i].activo)
        {
            al_draw_filled_rectangle(laser_enemigos[i].x,laser_enemigos[i].y,laser_enemigos[i].x+20,laser_enemigos[i].y+5,al_map_rgb(180,0,255));
        }
    }

    for(i=0;i<MAX_ITEMS;i++)
    {
        if(mis_items[i].activo)
        {
            if(mis_items[i].tipo==ITEM_VIDA&&(*imgs).pocion_vida!= NULL)
            {
                al_draw_scaled_bitmap((*imgs).pocion_vida,0,0,al_get_bitmap_width((*imgs).pocion_vida),al_get_bitmap_height((*imgs).pocion_vida),mis_items[i].x,mis_items[i].y,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_VENENO&&(*imgs).veneno!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).veneno,0,0,al_get_bitmap_width((*imgs).veneno),al_get_bitmap_height((*imgs).veneno),mis_items[i].x, mis_items[i].y,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_LLAVE_COMUN&&(*imgs).llave_basica!=NULL)
            {
                al_draw_scaled_bitmap((*imgs).llave_basica,0,0,al_get_bitmap_width((*imgs).llave_basica),al_get_bitmap_height((*imgs).llave_basica),mis_items[i].x, mis_items[i].y,30,30,0);
            }
            else if(mis_items[i].tipo==ITEM_PUNTOS&&(*imgs).punto!=NULL)
            {
                recorte_moneda_x=(*graficos).cuadro_moneda*563; 
                al_draw_scaled_bitmap((*imgs).punto,recorte_moneda_x,0,563,565,mis_items[i].x,mis_items[i].y,25,25,0);
            }
        }
    }

    for(i=0;i<MAX_ITEMS;i++)
    {
        if(heroe.proyectiles[i].activo&&(*imgs).veneno!=NULL)
        {
            al_draw_scaled_bitmap((*imgs).veneno,0,0,al_get_bitmap_width((*imgs).veneno),al_get_bitmap_height((*imgs).veneno),heroe.proyectiles[i].x,heroe.proyectiles[i].y,30,30,0);
        }
    }

    if(portal_activo&&(*imgs).portal!=NULL)
    {
        recorte_portal_x=(*graficos).cuadro_portal*32; 
        al_draw_scaled_bitmap((*imgs).portal,recorte_portal_x,0,32,32,x_portal,y_portal-TAM_TILE,TAM_TILE,TAM_TILE*2,0);
    }

    if((*graficos).fuente!=NULL)
    {
        al_draw_textf((*graficos).fuente,al_map_rgb(255,255,255),20,20,0,"VIDA: %d",heroe.vida);
        al_draw_textf((*graficos).fuente,al_map_rgb(255,215,0),20,50,0,"PUNTOS: %d",heroe.puntos);
    }

    al_flip_display();
}

bool colision_lasers(float x,float y)
{
    int izq=x/TAM_TILE;
    int der=(x+15)/TAM_TILE;
    int arr=y/TAM_TILE;
    int aba=(y+5)/TAM_TILE;

    if(izq<0||der>=columna||arr<0||aba>=fila)
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

void recolectar_items(item lista_items[],int maximo)
{
    int i,j;
    bool guardado;
    for(i=0;i<maximo;i++)
    {
        if(lista_items[i].activo)
        {
            lista_items[i].velocidad_y+=0.5;
            lista_items[i].y+=lista_items[i].velocidad_y;
            if(colision(lista_items[i].x, lista_items[i].y))
            {               
                lista_items[i].y =((int)(lista_items[i].y+39)/TAM_TILE)*TAM_TILE-39.1;
                lista_items[i].velocidad_y=0;
            }
            if(heroe.x<lista_items[i].x+40&&heroe.x+40>lista_items[i].x&&heroe.y<lista_items[i].y+40&&heroe.y+40>lista_items[i].y)
            {
                if(lista_items[i].tipo==ITEM_VIDA)
                {
                    heroe.vida+=lista_items[i].valor;
                    if(heroe.vida>100) 
                    {
                        heroe.vida=100; 
                    }
                    printf("¡Vida recogida! Vida actual: %d\n", heroe.vida);
                }
                else if(lista_items[i].tipo==ITEM_MUNICION)
                {
                    printf("¡Munición recogida!\n");
                }
                else if(lista_items[i].tipo==ITEM_VENENO||lista_items[i].tipo==ITEM_LLAVE_COMUN)
                {
                    guardado=false;
                    for(j=0;j<MAX_COSAS;j++)
                    {
                        if(heroe.inventario[j]==0)
                        {
                            heroe.inventario[j] = lista_items[i].tipo; 
                            
                            if(lista_items[i].tipo==ITEM_VENENO) 
                            {
                                printf("Veneno guardado en el bolsillo %d!\n",j);
                            }
                            if(lista_items[i].tipo==ITEM_LLAVE_COMUN) 
                            {
                                printf("Llave guardada en el bolsillo %d!\n",j);
                            }
                            
                            lista_items[i].activo=false; 
                            guardado=true;
                            break;
                        }

                    }
                    if(!guardado)
                    {
                        printf("Inventario lleno!\n");
                    }
                }
                else if(lista_items[i].tipo==ITEM_PUNTOS)
                {
                    heroe.puntos += lista_items[i].valor;
                    printf("Punto recogido! Puntuación: %d\n",heroe.puntos);
                }
                lista_items[i].activo = false;
            }
        }
    }
}

void mover_proyectiles(proyectil mis_proyectiles[], int maximo)
{
    int i,j;
    for(i=0;i<maximo;i++)
    {
        if(mis_proyectiles[i].activo)
        {
            mis_proyectiles[i].x+=mis_proyectiles[i].velocidad_x;
            mis_proyectiles[i].velocidad_y+=0.5; 
            mis_proyectiles[i].y+=mis_proyectiles[i].velocidad_y;

            if(mis_proyectiles[i].x>ancho_pantalla||mis_proyectiles[i].x<0||colision_proyectil(mis_proyectiles[i].x,mis_proyectiles[i].y))
            {
                mis_proyectiles[i].activo=false;
            }

            if(mis_proyectiles[i].activo)
            {
                for(j=0;j<total_enemigos;j++)
                {
                    if(enemigos[j].vida>0)
                    {
                        if(mis_proyectiles[i].x<enemigos[j].x+40&&mis_proyectiles[i].x+30>enemigos[j].x&&mis_proyectiles[i].y<enemigos[j].y+40&&mis_proyectiles[i].y+30>enemigos[j].y)
                        {
                            enemigos[j].vida -= 40; 
                            mis_proyectiles[i].activo = false;
                            printf("Impacto! Vida enemigo: %d\n", enemigos[j].vida);
                            break;
                        }
                    }
                }
            }
        }
    }
}

bool colision_proyectil(float x,float y)
{
    int izq=x/TAM_TILE;
    int der=(x+25)/TAM_TILE; 
    int arr=y/TAM_TILE;
    int aba=(y+25)/TAM_TILE;

    if(izq<0||der>=columna||arr<0||aba>=fila)
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

bool cargar_imagenes(recursos *imgs)
{
    (*imgs).fondo_nivel1 = al_load_bitmap("Fondo1.jpg");
    if(!(*imgs).fondo_nivel1)
    { 
        printf("Error fatal: No se encontro 'Fondo1.jpg'\n"); 
        return false; 
    }

    (*imgs).img_ataque = al_load_bitmap("prota_ataque.png");
    if(!(*imgs).img_ataque) 
    { 
        printf("Error fatal: No se encontro 'prota_ataque.png'\n"); 
        return false; 
    }

    (*imgs).img_dano = al_load_bitmap("prota_auch.png");
    if(!(*imgs).img_dano) 
    { 
        printf("Error fatal: No se encontro 'prota_auch.png'\n"); 
        return false; 
    }

    (*imgs).img_caminar = al_load_bitmap("prota_caminar.png");
    if(!(*imgs).img_caminar) 
    { 
        printf("Error fatal: No se encontro 'prota_caminar.png'\n"); 
        return false; 
    }

    (*imgs).img_correr = al_load_bitmap("prota_corre.png");
    if(!(*imgs).img_correr) 
    { 
        printf("Error fatal: No se encontro 'prota_corre.png'\n"); 
        return false; 
    }

    (*imgs).img_disparo = al_load_bitmap("prota_disparo.png");
    if(!(*imgs).img_disparo) 
    { 
        printf("Error fatal: No se encontro 'prota_disparo.png'\n"); 
        return false; 
    }

    (*imgs).img_muerte = al_load_bitmap("prota_muerte.png");
    if(!(*imgs).img_muerte) 
    { 
        printf("Error fatal: No se encontro 'prota_muerte.png'\n"); 
        return false; 
    }

    (*imgs).img_recarga = al_load_bitmap("prota_recarga.png");
    if(!(*imgs).img_recarga) 
    { 
        printf("Error fatal: No se encontro 'prota_recarga.png'\n"); 
        return false; 
    }

    (*imgs).img_salto = al_load_bitmap("prota_salto.png");
    if(!(*imgs).img_salto) 
    { 
        printf("Error fatal: No se encontro 'prota_salto.png'\n"); 
        return false; 
    }

    (*imgs).img_idle = al_load_bitmap("prota_Idle.png");
    if(!(*imgs).img_idle) 
    { 
        printf("Error fatal: No se encontro 'prota_Idle.png'\n"); 
        return false; 
    }

    (*imgs).img_basico = al_load_bitmap("alien_basico.png");
    if(!(*imgs).img_basico) 
    { 
        printf("Error fatal: No se encontro 'alien_basico.png'\n"); 
        return false; 
    }

    (*imgs).textura_luna=al_load_bitmap("tile_lunar.png");
    if(!(*imgs).textura_luna) 
    { 
        printf("Error fatal: No se encontro 'tile_lunar.png'\n"); 
        return false; 
    }

    (*imgs).pocion_vida=al_load_bitmap("vida.png");
    if(!(*imgs).pocion_vida) 
    { 
        printf("Error fatal: No se encontro 'vida.png'\n"); 
        return false; 
    }

    (*imgs).textura_metal=al_load_bitmap("tile_metal.png");
    if(!(*imgs).textura_metal)
    {
        printf("Error fatal: No se encontro 'tiles_metal.png'\n");
        return false;
    }

    (*imgs).alien5_idle=al_load_bitmap("alien5_idle.png");
    if(!(*imgs).alien5_idle)
    {
        printf("Error fatall: No se encontro 'alien5_idle.png'\n");
        return false;
    }

    (*imgs).alien5_caminar=al_load_bitmap("alien5_walk.png");
    if(!(*imgs).alien5_caminar)
    {
        printf("Error fatal: No se encontro 'alien5_walk.png\n");
        return false;
    }

    (*imgs).alien5_ataque=al_load_bitmap("alien5_atack.png");
    if(!(*imgs).alien5_ataque)
    {
        printf("Error fatal: No se encontro 'alien5_atack.png\n");
        return false;
    }

    (*imgs).alien5_dead=al_load_bitmap("alien5_dead.png");
    if(!(*imgs).alien5_dead)
    {
        printf("Error fatal: No se encontro 'alien5_dead'");
        return false;
    }

    (*imgs).veneno=al_load_bitmap("veneno.png");
    if(!(*imgs).veneno)
    {
        printf("Error fata: No se encontro 'veneno.png'\n");
        return false;
    }

    (*imgs).llave_basica=al_load_bitmap("llave_comun.png");
    if(!(*imgs).llave_basica)
    {
        printf("Error fatal: No se encontro'llave_comun.png'\n");
        return false;
    }

    (*imgs).llave_final=al_load_bitmap("llave_final.png");
    if(!(*imgs).llave_final)
    {
        printf("Error fatal: No se encontro 'llave_final.png'\n");
        return false;
    }

    (*imgs).puas=al_load_bitmap("puas.png");
    if(!(*imgs).puas)
    {
        printf("Error fatal: No se encontro 'puas.png'\n");
        return false;
    }

    (*imgs).portal=al_load_bitmap("portal.png");
    if(!(*imgs).portal)
    {
        printf("Error fatal: No se encontro 'portal.png'\n");
        return false;
    }

    (*imgs).punto=al_load_bitmap("point.png");
    if(!(*imgs).punto)
    {
        printf("Error fatal: No se encontro 'point.png'\n");
        return false;
    }

    return true; 
}