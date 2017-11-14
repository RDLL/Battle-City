/****************************************BIBLIOTECAS****************************************/
#include <stdio.h> //Contiene prototipos de funciones para manipular datos de entrada y salida
#include <graphics.h> //Contiene prototipos de funciones para preparar y manipular gráficos en el entorno MS-DOS
#include <stdlib.h> //Contiene prototipos de funciones para utilidades de uso general
#include <time.h> //Contiene prototipos de funciones para uso de tiempo
#include <windows.h>


/*******************************************************************************************/

/****************************************CONSTANTES****************************************/
#define X 1280 //Limite para pantalla de informacion de partida
#define Y 760 //Limite para la creacion de nodos en posicion vertical
#define L 40 //Tamaño de cada cuadro
#define T X-200 //Limite para creacion de nodos en posicion horizontal
/******************************************************************************************/

/****************************************ESTRUCTURAS****************************************/
typedef struct mal{ //Declaracion de estructura
    int x,y,tp; //Coordenadas para tamaño de cuadro
    struct mal *sig,*ant,*up,*dw; //Nodos arriba,abajo,dercha e izquierda
    struct mal *obj; //Nodo auxiliar para saber si hay un enemigo u obstaculo sera NULL en caso de no haber nada
}MALLA; //Nombre de estructura

typedef struct bal{ //Declaracion de estructura
    int x,y;
    struct bal *sig; //Nodo para la bala siguiente
}BALA; //Nombre de estructura

typedef struct{ //Declaracion de estructura
    MALLA *pos; //Nodo para moverse por la malla
    int m; //Cantidad de balas
    int tp; //Tipo de bala
    BALA *b; //Nodo para creacion de bala
}JUGADOR; //Nombre de estructura

typedef struct nod{ //Declaracion de estructura
    int x,y; //Coordenadas para graficar enemigo
    int tp; //Tipo de enemigo
    int vida; //Vida de enemigo
    MALLA *p; //Nodos para recorrer o eliminar enemigos
    struct nod *sig,*ant; //Nodo para enlazamiento doble
}NODOD; //Nombre de estructura

typedef struct{ //Declaracion de estructura
    int tp; //Tipo de obstaculo
    int vida; //Vida en caso de ser de tipo m
    MALLA *pl; //Enlazamiento con la malla
}BLOQUES; //Nombre de estructura

typedef struct{ //Declaracion de estructura
    NODOD *prim,*ult; //Nodos derecho e izquierdo y nodo que apunta a la malla
}ENEMIGO; //Nombre de estructura
/*******************************************************************************************/

/****************************************VARIABLES GLOBALES***************************************************/
int ie=0; //Variable para cambio de orientacion de personaje 0-3 (arriba,abajo,izquierda,derecha)
int ne=3; //Variable para numero de enemigos
int po=0; //Variable para puntos
int vidas=3; //Variable para vidas
int iem=0; //Variable para cambio de orientacion de enemigo 0-3(arriba,abajo,izquierda,derecha)
int level=0; //Variable para saber el nivel actual
int h=0; //Variable auxiliar para saber que un archivo ya fue abierto
int cl1[10][10]; //Colores para archivo de icono de vidas de personaje principal
int cl2[10][10]; //Colores para archivo de icono de cantidad de enemigos
/*******************************************************************************************/

/****************************************PROTOTIPOS_DE_FUNCIONES****************************************/
MALLA* CreaNodo(int d,int e); //Ingresa datos en un nodo de tipo MALLA
BALA* CreaBala(int x,int y); //Creacion de bala
int insBala(BALA **cab,int x,int y); //Ingresa datos en un nodo de tipo bala
void iniListaD(ENEMIGO *E); //Inicializa los nodos para creacion de enemigos
NODOD* CreaNodoD(int d,int e,int t, int v,MALLA *pos); //Ingresa datos en un nodo para una lista doblemente enlazada
void CreaEnemigo(ENEMIGO *cab,int d,int e,int t,int v,MALLA *pos);
void CreaMalla(MALLA **cab); //Creacion de malla de nodos
void DibujaMalla(MALLA *cab); //Graficacion de malla de nodos*/
void movimiento(MALLA **cab,JUGADOR *P,ENEMIGO *E,BLOQUES *B); //Captura de teclas para mover personaje
int abrir(int col[L-1][L-1]); //Apertura de archivo de personaje principal
void abriren(int col[L-1][L-1]); //Apertura de archivo de enemigo
void borra(int x,int y); //Cambio de color de personaje al de fondo
void dibuja(int col[L-1][L-1],int x,int y); //Graficacion de personaje
void dibujae(int col[L-1][L-1],int x,int y); //Graficacion de enemigo
void enemigos(MALLA **cab,ENEMIGO *E,int cole[L-1][L-1]); //Creacion de enemigos para cada nivel
void balas(JUGADOR *P,int bal[L-1][L-1],ENEMIGO *E); //Crecion de balas con movimiento
void escenario(BLOQUES *B,MALLA **cab,int col[L-1][L-1],int ld[L-1][L-1]); //Creacion y graficacion de obstaculos
int ElimBala(BALA **cab); //Graficacion de balas y eliminacion de la misma
/*******************************************************************************************************/

/****************************************FUNCION_PRINCIPAL****************************************/
int main(){ //Inicio de funcion principal
    initwindow(1280,800,"MALLANODOS"); //Inicializacion de modo grafico
    rectangle(T,0,X-5,Y); //Tamaño de tablero de datos de partia en curso
    rectangle(0,0,T,Y); //Tamaño de ventana de juego
    MALLA *M=NULL; //Declaracion de malla
    JUGADOR  P; //Declaracion de jugador
    P.b=NULL;
    P.m=100; //Cantidad inicial de balas
    BLOQUES LD; //Declaracion para ladrillos
    ENEMIGO E;//Declaracion de enemigo
    CreaMalla(&M); //Llamado a funcion de creacion de malla
    iniListaD(&E); //Llamado a funcion para inicializar lista de enemigos
    movimiento(&M,&P,&E,&LD); //Llamado a funcion de movimiento
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
        nuevo->tp=0;
        nuevo->sig=nuevo->ant=nuevo->dw=nuevo->up=NULL; //Declaracion para saber donde continua el nodo siguiente
        nuevo->obj=NULL; //Nodo para saber si hay un enemigo u objeto bloqueando el paso
    } //Fin de comprobación
    return(nuevo); //Retorno de nodo con datos
} //Fin de funcion

int ElimBala(BALA **cab){
    BALA *aux;
    if(*cab){
        aux=*cab;
        *cab=aux->sig;
        free(aux);
        return 1;
    }
    return 0;
}

int ElimEne(ENEMIGO *cab){
    NODOD *aux;
    if(cab){
        aux=cab->prim;
        cab->prim=aux->sig;
        free(aux);
    }
}

BALA* CreaBala(int x,int y){
    BALA *nuevo;
    nuevo=(BALA*)malloc(sizeof(BALA));
    if (nuevo){
        nuevo->x=x;
        nuevo->y=y;
        nuevo->sig=NULL;
    }
    return nuevo;
}

int insBala(BALA **cab,int x,int y){
    BALA *nuevo,*aux;
    nuevo=CreaBala(x,y);
    if(nuevo){
        if(!*cab){
            *cab=nuevo;
        }else{
            for(aux=*cab;aux->sig!=NULL;aux=aux->sig);
            aux->sig=nuevo;
        }
        return 1;
    }
    return 0;
}

void iniListaD(ENEMIGO *E){ //Inicio de funcion para inicialzar lista de enemigos
    E->prim=NULL; //Nodo izquierdo inicializado como nulo
    E->ult=NULL; //Nodo derecho inicializado como nulo
} //Fin de funcion

NODOD* CreaNodoD(int d,int e,int t, int v,MALLA *pos){ //Inicio de funcion para creacion de nodo paso por valor de coordenadas x,y
    NODOD *nuevo; //Declaracion de nodo
    nuevo=(NODOD*)malloc(sizeof(NODOD)); //Creacion de nodo
    if(nuevo){ //Comprobación de la existencia de nodo nuevo
        nuevo->x=d; //Declaracion de coordenada en x
        nuevo->y=e; //Declaracion de coordenada en y
        nuevo->tp=t; //Declaracion de  tipo de enemigo
        nuevo->vida=v; //Declaracion de vida de enemigo
        nuevo->p=pos; //Enlazamiento con malla
        nuevo->sig=nuevo->ant=NULL; //Declaracion para saber donde continua el nodo siguiente o el anterior
    } //Fin de comprobación
    return(nuevo); //Retorno de nodo con datos
} //Fin de funcion

void CreaMalla(MALLA **cab){ //Inicio de funcion para creacion de datos de la malla de nodos
    int i,j,n,m,x,y; //Declaracion de variables de contadores
    m=(T)/L; //Numero maximo de nodos en horizontal
    n=Y/L; //Numero maximo de nodos en vertical
    x=y=1; //Inicializacion de x e y(Inicializado en 1 para evitar borrar los margenes de ventana de juego y de informacion de partida)
    MALLA *nuevo; //Declaracion de nodo principal
    MALLA *auxarriba=NULL; //Declaracion de nodo auxiliar para vinculacion de nodo arriba
    MALLA *auxanterior=NULL; //Declaracion de nodo auxiliar para vinculacion de nodo anterior
    MALLA *aux=NULL; //Declaracion de nodo auxiliar para recorrido de nodos
    MALLA *aux2=NULL;
    for(i=0;i<n;i++){ //Inicio de ciclo para recorrido en y
        x=1; //Inicializacion de x para evitar borrar limite de ventana de juego
        if(i==18){ //Comprobacion en recorrido vertical para evitar borrar limite de la ventana de juego
            y=-1; //Se le resta uno a y para no llegar al limite de la ventana de juego
        } //Fin de comprobacion
        for(j=0;j<m+1;j++){ //Inicio de ciclo para recorrido en x
            if(j==25){ //Limite en x para evitar borrar limite de ventana de juego
                x=-1; //Se le resta uno a x para no llegar al limite de la ventana de juego
            }//Fin de comprobacion
            nuevo=CreaNodo(x+(j*L),y+(i*L)); //Llamado a funcion para crear nodo
            if(nuevo){ //Comprobacion de la existencia del nodo para empezar a crear nodos
                if(!*cab){ //Iteracion para enlazamiento de nodo
                    *cab=nuevo; //Enlazamiento de cabecera
                    aux=nuevo; //Declaracion de nodo auxiliar para ir al siguiente nodo
                    auxarriba=nuevo; //Declaracion de auxiliar para recorrer hilera de arriba
                    aux2=nuevo; //Declaracion de inicio de hilera de arriba
                    auxanterior=nuevo; //Declaracion de nodo anterior
                } //Fin de iteracion
                else{ //Iteracion para enlazamiento de nodos
                    if(!aux) //Iteracion para crear nuevamente nodo auxiliar
                    aux=nuevo; //Creacion de nodo auxiliar
                    if(j==0) //Iteracion para inicializar nodo anterior
                        auxanterior=NULL; //Inicializacion de nodo anterior
                    nuevo->ant=auxanterior; //Enlazamiento con nodo anterior
                    auxanterior=nuevo; //Enlazamiento de nodo anterior con nodo actual;
                    aux->sig=nuevo; //Creacion de nodo siguiente
                    if(j==m) //Iteracion para inicializar nodo auxiliar
                        aux->sig=NULL; //Inicializacion de nodo auxiliar
                    aux=aux->sig; //Declaracion de nodo siguiente
                    if(j!=0 && i==0){ //Iteracion creacion de hilera de arriba
                        auxarriba->sig=nuevo; //Creacion del siguiente de hilera de arriba
                        if(j==m) //Iteracion para inicializar nodo de arriba
                            auxarriba->sig=NULL; //Inicializacion de nodo de arriba
                        auxarriba=auxarriba->sig; // Declaracion de hilera de arriba
                    } //Fin de iteracion para creacion de hilera de arriba
                    if(j==0){ //Iteracion para inicio de auxarriba;
                        auxarriba=aux2; //Inicio de recorrido de hilera arriba
                    } //Fin de iteracion para inicializacion de ciclo
                    if(auxarriba && nuevo && i>0){ //Iteracion para recorido y enlazamiento d hilera arriba
                        auxarriba->dw=nuevo; //Enlazamiento de nodo arriba con nodo actual
                        //printf("%d\t%d\n",auxarriba->x,auxarriba->y);
                        nuevo->up=auxarriba; //Enlazamiento de nodo actual con nodo arriba
                        if(j==m)
                            auxarriba->sig=NULL;
                        auxarriba=auxarriba->sig; //Recorrido de nodo arriba
                    } //Fin de iteracion para recorrido de hilera arriba
                    if(j==0){ //Iteracion para iniciar hilera de arriba
                        aux2=nuevo; //Creacion de inicio de hilera de arriba
                    } //fin de iteracion para iniciar hilera de arriba
                } //Fin de iteracion para enlazamiento de nodos
            } //Fin de creacion de nodos
        } //Fin de ciclo de recorrido en x
    } //Fin de ciclo de recorrido en y
} //Fin de funcion

int abrir(int col[L-1][L-1]){ //funcion para abrir archivo de personaje principal
    FILE *arch;
    int i,j,cl,N,M; //Declaracion de contadores, N y M(valores no usados debido a que ya se sabe el tamaño del personaje; el cual es igual al de un cuadro de la malla
    if(h==1){ //Iteracion para nombre de personaje principal
        arch=fopen("tanke","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del icono
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del icono
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del icono
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del icono
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{ //Iteracion en caso de no existir el archivo
            printf("No existe el archivo"); //Impresion de no existir el archivo
        } //Fin de iteracion
    fclose(arch); //Cierre de archivo
    h++;
    return 1; //return
    }
    if(h==2){
        arch=fopen("tankp","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del personaje
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del personaje
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{
            printf("No existe el archivo");
        }
    fclose(arch); //Cierre de archivo
    h++;
    return 1;
    }
    if(h==3){
        arch=fopen("bal","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del personaje
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del personaje
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{
            printf("No existe el archivo");
        }
    fclose(arch); //Cierre de archivo
    h++;
    return 1;
    }
    if(h==4){
        arch=fopen("balb","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del personaje
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del personaje
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{
            printf("No existe el archivo");
        }
    fclose(arch); //Cierre de archivo
    h++;
    return 1;
    }
    if(h==5){
        arch=fopen("balf","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del personaje
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del personaje
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{
            printf("No existe el archivo");
        }
    fclose(arch); //Cierre de archivo
    h++;
    return 1;
    }
    if(h==0){
        arch=fopen("ladn","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del personaje
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del personaje
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{
            printf("No existe el archivo");
        }
    fclose(arch); //Cierre de archivo
    h++; //Aumento en h para no volver a entrar
    return 1; //Salida de funcion
    }
    if(h==6){
        arch=fopen("ladm","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del personaje
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del personaje
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{
            printf("No existe el archivo");
        }
    fclose(arch); //Cierre de archivo
    h++;
    return 1;
    }
    if(h==7){
        arch=fopen("base","r");//Apertura de archivo
        if(arch){ //Comprobacion de apertura de archivo para paso de valores
            fscanf(arch,"%d\n",&N); //Paso de valor n, el cual es el tamaño vertical del personaje
            fscanf(arch,"%d\n",&M); //Paso de valor m,el cual es el tamaño horizontal del personaje
            for(i=0;i<N;i++){ //Ciclo en vertical para asignar los colores del personaje
                for(j=0;j<M;j++){ //Ciclo en horizontal para asignar los colores del personaje
                    fscanf(arch,"%d\n",&cl); //Paso de valor de color el cual va de 0-15
                    col[i][j]=cl; //Paso de valor al arreglo que almacenara los colores
                } //Fin de recorrido horizontal
            } //Fin de recorrido vertical
        } //Fin de paso de valores del archivo
        else{
            printf("No existe el archivo");
        }
    fclose(arch); //Cierre de archivo
    h++;
    return 1;
    }
} //Fin de funcion


void CreaEnemigo(ENEMIGO *cab,int d,int e,int t,int v,MALLA *pos){ //Inicio de funcion para crear enemigos(Insercion al final)
    NODOD *nuevo; //Nodo principal
    nuevo=CreaNodoD(d,e,t,v,pos); //Llamada a funcion CreaNodoD para ingresas datos en nodo
    if(nuevo){ //Enlazamiento de nodos
        if(!cab->ult){ //Enlazamiento de cabecera
            cab->prim=nuevo; //Declaracion de inicio por izquierda de nodo
            cab->ult=nuevo; //Declaracion de inicio por derecha de nodo
        } //Fin de enlazamiento de cabecera
        else{ //Recorrido de nodo
            cab->ult->sig=nuevo; //Recorrido de nodo
        }//Fin de recorrido de nodo
        nuevo->ant=cab->ult; //Enlazamiento de nodo anterior
        cab->ult=nuevo; //Declaracion de nodo
    } //Fin de Enlazamientos
} //Fin de funcion

void movimiento(MALLA **cab,JUGADOR *P,ENEMIGO *E,BLOQUES *B){ //Funcion de movimiento
    MALLA *aux; //Nodo auxiliar para guardar posicion inicial de la malla
    aux=*cab; //Inicializacion de nodpo auxiliar
    int tecla; //Variable para capturar una tecla
    int col[L-1][L-1]; //Arreglo para colores de personaje principal
    int cole[L-1][L-1]; //Arreglo para colores de enemigo;
    int ld1[L-1][L-1]; //Arreglo para colores de ladrillo normal
    int ld2[L-1][L-1]; //Arreglo para colores de larillo metalico
    int b1[L-1][L-1]; //Arreglo para colores de bala normal
    int b2[L-1][L-1]; //Arreglo para colores de bala explosiva
    int b3[L-1][L-1]; //Arreglo para colores de bala de fuego
    for(int i=0;i<L-1;i++){ //Ciclo para inicializar los colores de las balas en negro (Recorrido vertical)
        for(int j=0;j<L-1;j++){  //Ciclo para inicializar los colores de las balas en negro (Recorrido horizontal)
            b1[i][j]=0; //Inicializacion en color negro
            b2[i][j]=0; //Inicializacion en color negro
            b3[i][j]=0; //Inicializacion en color negro
        } //Fin de recorrido horizontal
    } //Fin de recorrido vertical
    int base[L-1][L-1]; //Arreglo para colores de base
    abrir(ld1); //Llamado a funcion para abrir archivo de ladrillo normal
    abrir(cole); //Llamado a funcion para abrir archivo de enemigos
    abrir(col); //Llamado a funcion para abrir el archivo del personaje principal
    abrir(b1); //Llamado a funcion para abrir el archivo de bala normal
    abrir(b2); //Llamado a funcion para abrir el archivo de bala explosiva
    abrir(b3); //Llamado a funcion para abrir el archivo de bala de fuego
    abrir(ld2); //Llamado a funcion para abiri el archivo de ladrillo metalico
    abrir(base); //Llamado a funcion para abrir el archivo de base
    for(int i=0;i<6;i++){ //Ciclo para crear obstaculos
    escenario(B,cab,ld1,ld2); //Llamado a funcion para crear obstaculos
    *cab=aux; //Inicializacion de cabecera
    }
     P->pos=*cab; //Enlace de personaje con la malla
     B->pl=*cab; //Enlace de obstaculos con la malla

    enemigos(cab,E,cole); //Llamado a funcion para crear enemigos
    for(int i=0;i<8;i++){ //Recorrido para ubicar al personaje en determinda posicion horizontal
        P->pos=P->pos->sig; //Declaracion de nodo como nodo siguiente
        B->pl=B->pl->sig;
    } //Fin de recorrido
    for(P->pos=P->pos->dw;P->pos->dw!=NULL;P->pos=P->pos->dw); //Ciclo para ubicar al personaje en determinada posicion vertical
    aux=P->pos;
    for(B->pl=B->pl->dw->up;B->pl->dw!=NULL;B->pl=B->pl->dw); //Recorrido para enlazar los bloques con la malla
    dibuja(base,P->pos->sig->sig->x,P->pos->sig->sig->y); //Llamado a funcion para graficar la base
    B->pl->sig->sig->tp=1;
    P->pos->sig->sig->obj=B->pl->sig->sig; //Enlazamiento de jugador con base
    B->pl->sig->sig->obj=P->pos->sig->sig; //Enlazamiento de base con jugador
    dibuja(ld1,P->pos->sig->x,P->pos->sig->y); //Llamado a funcion para graficar un muro de proteccion de la base
    B->pl->sig->tp=1;
    P->pos->sig->obj=B->pl->sig; //Enlazamiento de jugador con muro
    B->pl->sig->obj=P->pos->sig; //Enlazamiento de muro con jugador
    dibuja(ld1,P->pos->sig->up->x,P->pos->sig->up->y); //Llamado a funcion para graficar un muro de proteccion de la base
    B->pl->sig->up->tp=1;
    P->pos->sig->up->obj=B->pl->sig->up; //Enlazamiento de jugador con muro
    B->pl->sig->up->obj=P->pos->sig->up; //Enlazamiento de muro con jugador
    dibuja(ld1,P->pos->sig->up->sig->x,P->pos->sig->up->sig->y); //Llamado a funcion para graficar un muro de proteccion de la base
    B->pl->sig->up->sig->tp=1;
    P->pos->sig->up->sig->obj=B->pl->sig->up->sig; //Enlazamiento de jugador con muro
    B->pl->sig->up->sig->obj=P->pos->sig->up->sig; //Enlazamiento de muro con jugador
    dibuja(ld1,P->pos->sig->up->sig->sig->x,P->pos->sig->up->sig->sig->y); //Llamado a funcion para graficar un muro de proteccion de la base
    B->pl->sig->up->sig->sig->tp=1;
    P->pos->sig->up->sig->sig->obj=B->pl->sig->up->sig->sig; //Enlazamiento de jugador con muro
    B->pl->sig->up->sig->sig->obj=P->pos->sig->up->sig->sig; //Enlazamiento de muro con jugador
    dibuja(ld1,P->pos->sig->sig->sig->x,P->pos->sig->sig->sig->y); //Llamado a funcion para graficar un muro de proteccion de la base
    B->pl->sig->sig->sig->tp=1;
    P->pos->sig->sig->sig->obj=B->pl->sig->sig->sig; //Enlazamiento de jugador con muro
    B->pl->sig->sig->sig->obj=P->pos->sig->sig->sig; //Enlazamiento de muro con jugador
    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar un personaje(personaje principal en este caso)
    P->pos=aux;
    do{ //Ciclo para capturar teclas
        tecla=getch(); //Captura de tecla
        switch(tecla){ //Inicio de opciones
        case 72: //Tecla flecha arriba
            if(P->pos->up && !P->pos->up->obj){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=0){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=0;  //Cambio de orientacion a arriba
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra orientado
                P->pos=P->pos->up; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                } //Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        case 80: //Tecla flecha abajo
            if(P->pos->dw && !P->pos->dw->obj){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=1){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=1; //Cambio de orientacion a abajo
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra orientado
                P->pos=P->pos->dw; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                }//Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        case 75: //Tecla Flecha izquierda
            if(P->pos->ant && !P->pos->ant->obj){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=2){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=2; //Cambio de orientacion a izquierda
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra en la posicion
                P->pos=P->pos->ant; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                } //Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        case 77: //Tecla Flecha derecha
            if(P->pos->sig && !P->pos->sig->obj){ //Verificacion de que existe el nodo al cual se quiere mover y movimiento
                borra(P->pos->x,P->pos->y); //Borrado del dibujo actual de personaje
                if(ie!=3){ //Comprobacion de orientacion en la que se encuentra el personaje y cambio de orientacion
                    ie=3; //Cambio de orientacion a derecha
                    dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje en nueva orientacion
                } //Fin de cambio de orientacion
                else{ //Comprobacion de que el personaje ya se encuentra en la posicion
                P->pos=P->pos->sig; //Movimiento de personaje
                dibuja(col,P->pos->x,P->pos->y); //Llamado a funcion para graficar personaje
                } //Fin de comprobacion de posicion
            } //Fin de movimiento
            break; //Fin de opcion de tecla
        case 32:
            if(P->m>0){
                balas(P,b1,E);
            }
            break;
        } //Fin de opciones para la variable tecla
    }while(tecla!=27); //Fin de ciclo el cual se rompe hasta presionar la tecla Esc
    closegraph(); //Cierre de ventana de moo grafico
} //Fin de funcion

void borra(int x,int y){ //Funcion para borrar personaje recibe posicion de pixel inicial
    int i,j,ax,ay; //Declaracion de varibles de contadores y auxiliares para coordenadas
    ax=x; //Inicializacion de auxiliar para x
    ay=y; //Inicializacion de auxiliar para y
    for(i=0;i<L;i++){ //Recorrido en vertical
        ax=x; //Inicializacion de x
        for(j=0;j<L;j++){ //Recorrido en horizontal
            putpixel(ax,ay,0); //Graficado de un pixel igual al color del fondo
            ax+=1; //Aumento en x
        } //Fin de recorrido horizontal
        ay+=1; //Aumento en y
    } //Fin de recorrido vertical
} //Fin de funcion
void enemigos(MALLA **cab,ENEMIGO *E,int cole[L-1][L-1]){
    MALLA *aux; //Nodo auxiliar para recorrer malla sin modificarla
    aux=*cab; //Posicion inicial para recorrido de malla para creacion de enemigos
    int tp=0; //Variables para numero de enemigos, posicion de enemigos y tipo de enemigos
    if(level>0 && level <8) //Comprobacion de nivel y aumento de enemigos
        ne+=2; //Aumento de enemigos
    for(int i=0;i<ne;i++){ //Creacion de enemigos
        if(level>4 && level <8 && i==ne-2) //Comprobacion de nivel y cambio en el tipo de enemigos
        tp=1; //Cambio de tipo de enemigo
        CreaEnemigo(E,aux->x,aux->y,tp,tp,aux); //Llamado a funcion para crear enemigo con esos datos
        aux->obj=E->ult->p; //Enlazamiento de malla con posicion de enemigo en la malla
        E->ult->p->obj=aux->obj; //Enlazamiento de enemigo con su posicion en la malla
        aux->obj->tp=3;
        if(i==0)
            *cab=aux;
        dibujae(cole,aux->x,aux->y); //Llamado a funcion para graficar enemigos
        aux=aux->sig; //Recorrido de malla
    } //Fin de creacion de enemigos
}

void dibuja(int col[L-1][L-1],int x,int y){ //Funcion para graficar personaje u enemigo en diferentes orientaciones
    int i,j,ax,ay; //Declaracion de contadores y auxiliares para coordenadas
    ax=x; //Iniciliazacion de auxiliar para x
    ay=y; //Inicializacion de auxiliar para y
    if(ie==0){ //Comprobacion de orientacion arriba y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){  //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion arriba
    if(ie==1){ //Comprobacion de orientacion abajo y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion abajo
    if(ie==2){ //Comprobacion de orientacion izquierda y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
    if(ie==3){ //Comprobacion de orientacion derecha y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
} //Fin de funcion

void dibujae(int col[L-1][L-1],int x,int y){ //Funcion para graficar personaje u enemigo en diferentes orientaciones
    int i,j,ax,ay; //Declaracion de contadores y auxiliares para coordenadas
    ax=x; //Iniciliazacion de auxiliar para x
    ay=y; //Inicializacion de auxiliar para y
    if(iem==1){ //Comprobacion de orientacion arriba y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){  //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion arriba
    if(iem==0){ //Comprobacion de orientacion abajo y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido en vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[i][j]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion abajo
    if(iem==2){ //Comprobacion de orientacion izquierda y graficacion de personaje en orientacion
        for(i=0;i<L-1;i++){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=0;j<L-1;j++){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
    if(iem==3){ //Comprobacion de orientacion derecha y graficacion de personaje en orientacion
        for(i=L-1;i>=0;i--){ //Recorrido vertical
            ay=y; //Inicializacion de y
            for(j=L-1;j>=0;j--){ //Recorrido horizontal
                putpixel(ax,ay,col[j][i]); //Graficado de pixel de color guardado en arreglo y recorrido de arreglo
                ay+=1; //Aumento en y
            } //Fin de recorrido horizontal
            ax+=1; //Aumento en x
        } //Fin de recorrido vertical
    } //Fin de graficacion con orientacion izquierda
} //Fin de funcion

void escenario(BLOQUES *B,MALLA **cab,int col[L-1][L-1],int ld[L-1][L-1]){
    MALLA *aux,*aux2; //Nodo auxiliar para recorrer malla sin modificarla
    aux=aux2=*cab; //Inicializacion de nodo auxiliar
    int m,a,n; //Variables para creacion de bloques en posicion aleatoria
    a=(T)/L; //Inicializacion de variable auxiliar para evitar pasar los limites de la malla en x
    n=(Y/L)-4; //Inicializacion de variable auxiliar para evitar pasar los limites de la malla en y y en la posicion de la  base
    time_t t; //Variable de tiempo para posiciones aleatorias de bloques
    srand((unsigned) time(&t)); //Semilla para posiciones aleatorias
    B->pl=*cab; //Inicializacion de malla contenida en la estructura BLOQUES
    for(int i=0;i<n;i++){ //Inicio de ciclo para recorrido vertical para creacion de bloques
        (*cab)->obj=B->pl; //Enlazamiento de malla con bloque
        B->pl->obj=(*cab)->obj; //Enlazamiento de bloque con malla
        aux=aux->dw; //Recorrido en vertical
        B->pl=aux; //Recorrido en vertical
        *cab=aux; //Recorrido en vertical
        m=rand()%a; //Inicializacion de variables para la creacion de bloques en posicion aleatoria en x
        for(int j=0;j<m;j++){ //Inicio de ciclo en horizontal para creacion de bloques en horizontal
            if(level<4)
            (*cab)=(*cab)->sig; //Nodo auxiliar para modificar cabecera
            B->pl=B->pl->sig; //Recorrido de malla de b
        } //Fin de recorrido horizontal
         (*cab)->obj=B->pl; //Enlazamiento de malla con bloque
            B->pl->obj=(*cab)->obj; //Enlazamiento de bloque con malla
         dibuja(col,B->pl->x,B->pl->y); //Llamado a funcion para graficar bloque
    } //Fin de recorrido vertical
} //Fin de funcion

void balas(JUGADOR *P,int bal[L-1][L-1],ENEMIGO *E){ //Funcion para crear balas
    MALLA *aux;
    aux=P->pos;
        if(ie==0){ //Iteracion para disparar hacia arriba
            if(P->pos->up) //Iteracion para evitar borrar personaje
               P->pos=P->pos->up; //Cambio de nodo
            while(P->pos->up && !P->pos->obj){ //Ciclo para creacio de recorrido de bala
                insBala(&P->b,P->pos->x,P->pos->y); //Llamado a funcion para ingresar coordenadas
                P->pos=P->pos->up; //Recorrido de malla
            } //Fin de ciclo
        } //Fin de disparo hacia arriba
        if(ie==1){ //Iteracion para disparar hacia abajo
            if(P->pos->dw) //Iteracion para evitar borrar personaje
               P->pos=P->pos->dw; //Cambio de nodo
            while(P->pos->dw && !P->pos->obj){ //Ciclo para creacion de recorrido de bala
                insBala(&P->b,P->pos->x,P->pos->y); //Llamado a funcion para ingresar coordenadas
                P->pos=P->pos->dw; //Recorrido de malla
            } //Fin de ciclo
        } //Fin de disparo hacia arriba
        if(ie==2){ //Iteracion para disparar a la izquierda
            if(P->pos->ant) //Iteracion para evitar borrar personaje
               P->pos=P->pos->ant; //Cambio de nodo
            while(P->pos->ant && !P->pos->obj){ //Ciclo para creacion de recorrido de bala
                insBala(&P->b,P->pos->x,P->pos->y); //Llamado a funcion para ingresar coordenadas
                P->pos=P->pos->ant; //Recorrido de malla
            } //Fin de ciclo
        } //Fin de disparo a la izquierda
        if(ie==3){ //Iteracion para disparar a la derecha
            if(P->pos->sig) //Iteracion para evitar borrar personaje
               P->pos=P->pos->sig; //Cambio de nodo
            while(P->pos->sig && !P->pos->obj){ //Ciclo para creacion de recorrido de bala
                insBala(&P->b,P->pos->x,P->pos->y); //Llamado a funcion para ingresar coordenadas
                P->pos=P->pos->sig; //Recorrido de malla
            } //Fin de ciclo
        } //Fin de disparo hacia arriba
        P->m--; //Se elimina una municion
        if(P->b){
            while(P->b->sig){ //Ciclo e iteracion para graficar bala
                borra(P->b->x,P->b->y); //Graficado de bala al color de fondo
                ElimBala(&P->b); //Eliminacion y recorrido de nos
                dibuja(bal,P->b->x,P->b->y); //Graficado de bala
            } //Fin de ciclo
            borra(P->b->x,P->b->y);
                borra(P->pos->obj->x,P->pos->obj->y);
                po+=100;
                P->pos->obj=NULL;
            if(P->pos->obj && P->pos->obj->tp==3){
                if(E->prim){
                    printf("%d\t%d\n",E->prim->x,E->prim->y);
                ElimEne(E);
                }
                else
                    iniListaD(E);
                borra(P->pos->obj->x,P->pos->obj->y);
                po+=100;
                P->pos->obj=NULL;
            }
        }
        P->b=NULL; //Inicializacion de bala como nulo para su posterior uso
        P->pos=aux;
} //Fin de funcion

