/****************************************BIBLIOTECAS****************************************/
#include <stdio.h> //Contiene prototipos de funciones para manipular datos de entrada y salida
#include <graphics.h> //Contiene prototipos de funciones para preparar y manipular gráficos en el entorno MS-DOS
#include <stdio.h> //Contiene prototipos de funciones para utilidades de uso general
/*******************************************************************************************/

/****************************************CONSTANTES****************************************/
#define X 1280 //Limite para la creacion de nodos en posicion horizontal
#define Y 760 //Limite para la creacion de nodos en posicion vertical
#define L 40 //Tamaño de cada cuadro
#define T X-200
/******************************************************************************************/

/****************************************ESTRUCTURAS****************************************/
typedef struct mal{
    int x,y;
    struct mal *sig,*ant,*up,*dw;
}MALLA;

typedef struct{
    int x,y,c;
}JUGADOR;
/*******************************************************************************************/

/****************************************PROTOTIPOS_DE_FUNCIONES****************************************/
MALLA* CreaNodo(int d,int e); //Ingresa datos en un nodo
void CreaMalla(MALLA **cab); //Creacion de malla de nodos
void DibujaMalla(MALLA *cab); //Graficacion de malla de nodos*/
void movimiento(MALLA *cab,JUGADOR P);
/*******************************************************************************************************/

/****************************************FUNCION_PRINCIPAL****************************************/
int main(){ //Inicio de funcion principal
    initwindow(1280,800,"MALLANODOS"); //Inicializacion de modo grafico
    rectangle(T,0,X-5,Y);
    rectangle(0,0,T,Y);
    MALLA *M=NULL;
    JUGADOR  P;
    CreaMalla(&M); //Llamado a funcion de creacion de malla
    DibujaMalla(M); //Llamado a funcion para dibujar malla
    movimiento(M,P);
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
    m=(T)/L;
    n=Y/L;
    MALLA *nuevo;
    MALLA *auxarriba=NULL; //Declaracion de nodo auxiliar para vinculacion de nodo arriba
    MALLA *auxanterior=NULL; //Declaracion de nodo auxiliar para vinculacion de nodo anterior
    MALLA *aux=NULL; //Declaracion de nodo auxiliar para recorrido de nodos
    MALLA *aux2=NULL;
    for(i=0;i<n;i++){ //Inicio de ciclo para recorrido en y
        for(j=0;j<m;j++){ //Inicio de ciclo para recorrido en x
            nuevo=CreaNodo(j*L,i*L); //Llamado a funcion para crear nodo
            if(nuevo){ //Comprobacion de la existencia del nodo
                if(!*cab){ //Iteracion para enlazamiento de nodo
                    *cab=nuevo; //Enlazamiento de cabecera
                    aux=nuevo;
                    auxarriba=nuevo;
                    aux2=nuevo;
                    auxanterior=nuevo;
                } //Fin de iteracion
                else{ //Iteracion para enlazamiento de nodos
                    nuevo->ant=auxanterior; //Enlazamiento con nodo anterior
                    auxanterior=nuevo; //Enlazamiento de nodo anterior con nodo actual;
                    auxanterior->sig=NULL; //Creacion de nodo siguiente del nodo anterior
                    aux->sig=nuevo;
                    aux=aux->sig;
                    if(j!=0 && i==0){ //Iteracion creacion de hilera de arriba
                        auxarriba->sig=nuevo; //Recorrido de hilera de arriba;
                        auxarriba=auxarriba->sig;
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
                    if(j==0){
                        aux2=nuevo;
                    }
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
            setfillstyle(SOLID_FILL,1);
            fillpoly(4,coor); //Graficacion de cuadrado con relleno
        } //Fin de ciclo para graficar malla
    } //Fin de comprobacion de malla
} //Fin de funcion para graficar malla

void movimiento(MALLA *cab,JUGADOR P){
    int tecla,coor[8];
    P.x=cab->x;
    P.y=cab->y;
    coor[0]=P.x;
    coor[1]=P.y;
    coor[2]=P.x+L;
    coor[3]=P.y;
    coor[4]=P.x+L;
    coor[5]=P.y+L;
    coor[6]=P.x;
    coor[7]=P.y+L;
    setfillstyle(SOLID_FILL,15);
    fillpoly(4,coor);
    do{
        tecla=getch();
        switch(tecla){
        case 72:
            if(cab->up){
                setfillstyle(SOLID_FILL,0);
                fillpoly(4,coor);
                cab=cab->up;
                P.x=cab->x;
                P.y=cab->y;
                coor[0]=P.x;
                coor[1]=P.y;
                coor[2]=P.x+L;
                coor[3]=P.y;
                coor[4]=P.x+L;
                coor[5]=P.y+L;
                coor[6]=P.x;
                coor[7]=P.y+L;
                setfillstyle(SOLID_FILL,15);
                fillpoly(4,coor);
            }
            break;
        case 80:
            if(cab->dw){
                cab=cab->dw;
                setfillstyle(SOLID_FILL,0);
                fillpoly(4,coor);
                P.x=cab->x;
                P.y=cab->y;
                coor[0]=P.x;
                coor[1]=P.y;
                coor[2]=P.x+L;
                coor[3]=P.y;
                coor[4]=P.x+L;
                coor[5]=P.y+L;
                coor[6]=P.x;
                coor[7]=P.y+L;
                setfillstyle(SOLID_FILL,15);
                fillpoly(4,coor);
            }
            break;
        case 75:
            if(cab->ant){
                cab=cab->ant;
                setfillstyle(SOLID_FILL,0);
                fillpoly(4,coor);
                P.x=cab->x;
                P.y=cab->y;
                coor[0]=P.x;
                coor[1]=P.y;
                coor[2]=P.x+L;
                coor[3]=P.y;
                coor[4]=P.x+L;
                coor[5]=P.y+L;
                coor[6]=P.x;
                coor[7]=P.y+L;
                setfillstyle(SOLID_FILL,15);
                fillpoly(4,coor);
            }
            break;
        case 77:
            if(cab->sig){
                cab=cab->sig;
                setfillstyle(SOLID_FILL,0);
                fillpoly(4,coor);
                P.x=cab->x;
                P.y=cab->y;
                coor[0]=P.x;
                coor[1]=P.y;
                coor[2]=P.x+L;
                coor[3]=P.y;
                coor[4]=P.x+L;
                coor[5]=P.y+L;
                coor[6]=P.x;
                coor[7]=P.y+L;
                setfillstyle(SOLID_FILL,15);
                fillpoly(4,coor);
            }
        }
    }while(tecla!=27);
}
