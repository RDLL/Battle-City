/****************************************BIBLIOTECAS****************************************/
#include <stdio.h> //Contiene prototipos de funciones para manipular datos de entrada y salida
#include <graphics.h> //Contiene prototipos de funciones para preparar y manipular gráficos en el entorno MS-DOS
#include <stdio.h> //Contiene prototipos de funciones para utilidades de uso general
/*******************************************************************************************/

/****************************************CONSTANTES****************************************/
#define X 1280 //Limite para pantalla de juego
#define Y 760 //Limite para la creacion de nodos en posicion vertical
#define L 40 //Tamaño de cada cuadro
#define T X-200 //Limite para la creacion de nodos en posicion horizontal
/******************************************************************************************/

/****************************************ESTRUCTURAS****************************************/
typedef struct mal{ //Declaracion de estructura
    int x,y; //Posiciones iniciales del nodo
    struct mal *sig,*ant,*up,*dw; //Nodos arriba(up),abajo(dw),izquierda(ant),derecha(sig)
}MALLA; //Nombre de estructura

typedef struct{ //Declaracion de estructura
    int x,y,c; //Posiciones para movimiento y graficacion del cubo
}JUGADOR; //Nombre de estructura
/*******************************************************************************************/

/****************************************PROTOTIPOS_DE_FUNCIONES****************************************/
MALLA* CreaNodo(int d,int e); //Ingresa datos en un nodo
void CreaMalla(MALLA **cab); //Creacion de malla de nodos
void DibujaMalla(MALLA *cab); //Graficacion de malla de nodos*/
void movimiento(MALLA *cab,JUGADOR P); //Movimiento de cubo
/*******************************************************************************************************/

/****************************************FUNCION_PRINCIPAL****************************************/
int main(){ //Inicio de funcion principal
    initwindow(1280,800,"MALLANODOS"); //Inicializacion de modo grafico y ventana de juego
    rectangle(T,0,X-5,Y); //Creacion de pantalla de informacion
    rectangle(0,0,T,Y); //Creacion de pantalla de juego
    MALLA *M=NULL; //Inicializacion de malla
    JUGADOR  P; //Declaracion de jugador
    CreaMalla(&M); //Llamado a funcion de creacion de malla
    DibujaMalla(M); //Llamado a funcion para dibujar malla
    movimiento(M,P); //Llamado a funcion de movimiento
    system("pause"); //Pausa del sistema para visualizacion del modo grafico
} //Fin de funcion principal
/*************************************************************************************************/

/****************************************FUNCIONES****************************************/
MALLA* CreaNodo(int d,int e){ //Inicio de funcion para creacion de nodo paso por valor de coordenadas x,y
    MALLA *nuevo; //Declaracion de nodo
    nuevo=(MALLA*)malloc(sizeof(MALLA)); //Creacion de nodo
    if(nuevo){ //Comprobación de la existencia de nodo nuevo
        nuevo->x=d; //Inicialización de coordenada en x
        nuevo->y=e; //Inicialización de coordena en y
        nuevo->sig=nuevo->ant=nuevo->dw=nuevo->up=NULL; //Declaracion para saber donde continua el nodo siguiente
    } //Fin de comprobación
    return(nuevo); //Retorno de nodo con datos
} //Fin de funcion

void CreaMalla(MALLA **cab){ //Inicio de funcion para creacion de datos de la malla de nodos
    int i,j,n,m; //Declaracion de variables de contadores
    m=(T)/L; //Numero maximo de nodos en posicion horizontal
    n=Y/L; //Numero maximo de nodos en posicion vertical
    MALLA *nuevo; //Declaracion de nodo principal
    MALLA *auxarriba=NULL; //Declaracion de nodo de arriba de nodo principal
    MALLA *auxanterior=NULL; //Declaracion de nodo anterior al principal
    MALLA *aux=NULL; //Declaracion de nodo auxiliar para enlazamiento de nodos
    MALLA *aux2=NULL;  //Declaracion de nodo auxilar para inicializacion de nodo de arriba
    for(i=0;i<n;i++){ //Inicio de ciclo para recorrido en y
        for(j=0;j<m;j++){ //Inicio de ciclo para recorrido en x
            nuevo=CreaNodo(j*L,i*L); //Llamado a funcion para crear nodo
            if(nuevo){ //Comprobacion de la existencia del nodo
                if(!*cab){ //Iteracion para creacion de cabecera
                    *cab=nuevo; //Enlazamiento de cabecera
                    aux=nuevo; //Inicializacion de nodo auxiliar
                    auxarriba=nuevo; //Inicializacion de nodo arriba
                    aux2=nuevo; //Inicializacion de auxiliar para nodo arriba
                } //Fin de iteracion
                else{ //Iteracion para enlazamiento de nodos
                    nuevo->ant=auxanterior; //Enlazamiento con nodo anterior
                    auxanterior=nuevo; //Enlazamiento de nodo anterior con nodo actual;
                    auxanterior->sig=NULL; //Creacion de nodo siguiente del nodo anterior
                    aux->sig=nuevo; //Enlazamiento de nodo auxilar con nodo principal
                    aux=aux->sig; //Creacion de nodo siguiente
                    if(j!=0 && i==0){ //Iteracion creacion de hilera de arriba
                        auxarriba->sig=nuevo; //Recorrido de hilera de arriba;
                        auxarriba=auxarriba->sig; //Creacion de nodo siguiente de arriba
                    } //Fin de iteracion para creacion de hilera de arriba
                    if(j==0){ //Iteracion para inicio de auxarriba;
                        auxarriba=aux2; //Inicio de recorrido de hilera arriba
                    } //Fin de iteracion para inicializacion de ciclo
                    if(auxarriba && nuevo && i>0){ //Iteracion para recorido y enlazamiento d hilera arriba
                        auxarriba->dw=nuevo; //Enlazamiento de nodo arriba con nodo actual
                        //printf("%d\t%d\n",auxarriba->x,auxarriba->y);
                        nuevo->up=auxarriba; //Enlazamiento de nodo actual con nodo arriba
                        auxarriba=auxarriba->sig; //Recorrido de nodo arriba
                    } //Fin de iteracion para recorrido de hilera arriba
                    if(j==0){ //Iteracion para comenzar hilera de arriba
                        aux2=nuevo; //Inicilazacion de hilera de arriba
                    } //Fin de iteracion
                }
            }
        } //Fin de ciclo de recorrido en x
    } //Fin de ciclo de recorrido en y
} //Fin de funcion

void DibujaMalla(MALLA *cab){ //Inicio de funcion para graficar malla
    int coor[8]; //Arreglo con coordenadas de cada cuadro
    if(cab){ //Comprobacion de nodo
        for(cab=cab;cab;cab=cab->sig){ //Ciclo para recorrido de malla
            coor[0]=cab->x; //Coordenada x de primer punto de cuadrado
            coor[1]=cab->y; //Coordenada y de primer punto de cuadrado
            coor[2]=cab->x+L; //Coordenada x de segundo punto de cuadrado
            coor[3]=cab->y; //Coordenada y de segundo punto de cuadrado
            coor[4]=cab->x+L; //Coordenada x de tercer punto de cuadrado
            coor[5]=cab->y+L; //Coordenda y de tercer punto de cuadrado
            coor[6]=cab->x; //Coordenada x de cuarto punto de cuadrado
            coor[7]=cab->y+L; //Coordenada y de cuarto punto de cuadrado
            setfillstyle(SOLID_FILL,0); //Seleccion de color de fondo
            fillpoly(4,coor); //Graficacion de cuadrado
        } //Fin de ciclo para graficar malla
    } //Fin de comprobacion de malla
} //Fin de funcion para graficar malla

void movimiento(MALLA *cab,JUGADOR P){
    int tecla,coor[8]; //Declaracion de coordenadas y tecla para movimiento
    P.x=cab->x; //Inicializacion de posicion x en estructura
    P.y=cab->y; //Inicializacion de posicion y en estructura
    coor[0]=P.x; //Primer punto de x
    coor[1]=P.y; //Primer punto de y
    coor[2]=P.x+L; //Segundo punto de x
    coor[3]=P.y; //Segundo punto de y
    coor[4]=P.x+L; //Tercer punto de x
    coor[5]=P.y+L; //Tercer punto de y
    coor[6]=P.x; //Cuarto punto de x
    coor[7]=P.y+L; //Cuarto punto de y
    setfillstyle(SOLID_FILL,15); //Seleccion de color y tipo de relleno
    fillpoly(4,coor); //Graficacion de cuadrado 
    do{ //Ciclo para mover cuadrado
        tecla=getch(); //Obtencion de tecla
        switch(tecla){ //Casos de tecla
        case 72: //Tecla arriba
            if(cab->up){ //Comprobacion de que existe la posicion de arriba
                setfillstyle(SOLID_FILL,0); //Seleccion de color igual al del fondo
                fillpoly(4,coor); //Graficado de cuadro 
                cab=cab->up; //Dezplazamiento de nodo
                P.x=cab->x; //Inicializacion de posicion x en estructura
                P.y=cab->y; //Inicializacion de posicion y en estructura
                coor[0]=P.x; //Primer punto de x
                coor[1]=P.y; //Primer punto de y
                coor[2]=P.x+L; //Segundo punto de x
                coor[3]=P.y; //Segundo punto de y
                coor[4]=P.x+L; //Tercer punto de x
                coor[5]=P.y+L; //Tercer punto de y
                coor[6]=P.x; //Cuarto punto de x
                coor[7]=P.y+L; //Cuarto punto de y
                setfillstyle(SOLID_FILL,15); //Seleccion de color difente al del fondo
                fillpoly(4,coor); //Graficado de cuadro
            } //Fin de comprobacion
            break; //Fin de opcion
        case 80: //Tecla abajo
            if(cab->dw){ //Comprobacion de que existe la posicion de abajo
                setfillstyle(SOLID_FILL,0); //Seleccion de color igual al del fondo 
                fillpoly(4,coor); //Graficado de cuadro
                cab=cab->dw; //Dezplazamiento de nodo
                P.x=cab->x; //Inicializacion de posicion x en estructura
                P.y=cab->y; //Inicializacion de posicion y en estructura
                coor[0]=P.x; //Primer punto de x
                coor[1]=P.y; //Primer punto de y
                coor[2]=P.x+L; //Segundo punto de x
                coor[3]=P.y; //Segundo punto de y
                coor[4]=P.x+L; //Tercer punto de x
                coor[5]=P.y+L; //Tercer punto de y
                coor[6]=P.x; //Cuarto punto de x
                coor[7]=P.y+L; //Cuarto punto de y
                setfillstyle(SOLID_FILL,15); //Seleccion de color difente al del fondo
                fillpoly(4,coor); //Graficado de cuadro
            } //Fin de comprobacion
            break; //Fin de opcion
        case 75: //Tecla izquierda
            if(cab->ant){ //Comprobacion de que existe la posicion izquierda
                setfillstyle(SOLID_FILL,0); //Seleccion de color igual al del fondo 
                fillpoly(4,coor); //Graficado de cuadro
                cab=cab->ant; //Dezplazamiento de nodo
                P.x=cab->x; //Inicializacion de posicion x en estructura
                P.y=cab->y; //Inicializacion de posicion y en estructura
                coor[0]=P.x; //Primer punto de x
                coor[1]=P.y; //Primer punto de y
                coor[2]=P.x+L; //Segundo punto de x
                coor[3]=P.y; //Segundo punto de y
                coor[4]=P.x+L; //Tercer punto de x
                coor[5]=P.y+L; //Tercer punto de y
                coor[6]=P.x; //Cuarto punto de x
                coor[7]=P.y+L; //Cuarto punto de y
                setfillstyle(SOLID_FILL,15); //Seleccion de color difente al del fondo
                fillpoly(4,coor); //Graficado de cuadro
            } //Fin de comprobacion
            break; //Fin de opcion
        case 77:
            if(cab->sig){ //Comprobacion de que existe la posicion derecha 
                setfillstyle(SOLID_FILL,0); //Seleccion de color igual al del fondo
                fillpoly(4,coor); //Graficado de cuadro
                cab=cab->sig; //Dezplazamiento de nodo
                P.x=cab->x; //Inicializacion de posicion x en estructura
                P.y=cab->y; //Inicializacion de posicion y en estructura
                coor[0]=P.x; //Primer punto de x
                coor[1]=P.y; //Primer punto de y
                coor[2]=P.x+L; //Segundo punto de x
                coor[3]=P.y; //Segundo punto de y
                coor[4]=P.x+L; //Tercer punto de x
                coor[5]=P.y+L; //Tercer punto de y
                coor[6]=P.x; //Cuarto punto de x
                coor[7]=P.y+L; //Cuarto punto de y
                setfillstyle(SOLID_FILL,15); //Seleccion de color difente al del fondo
                fillpoly(4,coor); //Graficado de cuadro
            } //Fin de comprobacion
            break; //Fin de opcion
        } //Fin de opciones
    }while(tecla!=27); //Fin de ciclo en caso de oprimir la tecla Esc
} //Fin de funcion
