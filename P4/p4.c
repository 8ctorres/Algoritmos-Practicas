#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#define MAXSIZE 2048
#define true 1
#define false 0
typedef char byte;

/*
    Algoritmos
    Práctica 4
    Carlos Torres - carlos.torres@udc.es
    Daniel Sergio Vega - d.s.vega@udc.es
*/

typedef int ** matriz;;

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros
       pseudo-aleatorios */
}

double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void printv(int v[], int n) {
    int i;// iterador
    printf("[");
    for (i = 0; i < n; i++){
        printf("%3d", v[i]);
        if (i != n - 1){
            printf(",");
        }
    }
    printf("]\n");
}

matriz crearMatriz(int n) {
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}

/* Inicializacion pseudoaleatoria [1..MAXSIZE] de un grafo completo
no dirigido con n nodos, representado por su matriz de adayencia */
void iniMatriz(matriz m, int n) {
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % MAXSIZE + 1;

    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
            m[i][j] = m[j][i];
}

void liberarMatriz(matriz m, int n) {
    int i;
    for (i=0; i<n; i++)
        free(m[i]);
    free(m);
}

typedef matriz listad;

listad crearListaAd(int n) {
  int i,** aux;
  if ((aux = malloc(n*sizeof(int *))) == NULL)
    return NULL;
  for (i=0; i < n; i++) aux[i] = NULL;
  return aux;
}

void iniListaAd(listad l,int n) {
  //Una lista de adyacencia de un grafo completo es basicamente lo mismo que
  //representarlo con una matriz.
  int i;
  for (i = 0; i < n; i++) l[i] = malloc(sizeof(int)*n);
  //^^ No esta protegido frente a fallos
  //Mucho cuidado con la implementación requerida ya que puede variar
  iniMatriz(l,n);
}

void liberarListaAd(listad l, int n) {
  int i;
  for (i = 0; i < n; i++) free(l[i]);
}

void printm(matriz m, int n) {
    int i,j;
    for (i = 0; i < n; i++) {
        printf("[");
        for (j = 0; j < n; j++) {
            printf(" %2i ",m[i][j]);
        }
        printf("]\n");
    }
}

byte igualaref(matriz m) {
    int i,j;
    int r[][5] =
    {{0,1,3,4,6},
    {1,0,2,5,5},
    {3,2,0,7,5},
    {4,5,7,0,3},
    {6,5,5,3,0}};
    for (i = 0; i < 5; i++)
        for (j = 0; j < 5; j++) if (m[i][j] == r[i][j]) continue; else return false;
    return true;
 }

byte igualaref2(matriz m) {
    int i,j;
    int r[][4] =
    {{0,1,3,6},
    {1,0,2,5},
    {3,2,0,3},
    {6,5,3,0}};
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) if (m[i][j] == r[i][j]) continue; else return false;
    return true;
 }

void dijkstra(matriz grafo, matriz distancias, int tam){
    byte* noVisitados = malloc(sizeof(byte) * tam);
    int i,j,m,v,w, dist_min;
    for (m=0; m<tam; m++){ //Bucle principal
        for (i=0; i<tam; i++){
            noVisitados[i] = true; //Inicialización del conjunto de noVisitados
            distancias[m][i] = grafo[m][i]; //Se supone que la distancia entre dos nodos es la de la arista que los une
        }
        noVisitados[m] = false;

        for(i=0; i<(tam-2);i++){ //repetir n-2 veces
            //Buscar el nodo no visitado que minimiza la distancia al nodo M
            dist_min = -1;
            v = m;
            for(j=0; j<tam; j++){
               if (noVisitados[j] && (distancias[m][j] < dist_min || dist_min == -1)){
                    dist_min = distancias[m][j];
                    v = j;
                }
                //v es el índice del nodo que minimiza la distancia al nodo m
            }
            noVisitados[v] = false; //se quita el nodo v del conjunto de no visitados
            for(w=0; w<tam; w++){ //para cada w en el conjunto noVisitados:
                if(noVisitados[w]){
                    if (distancias[m][w] > (distancias[m][v] + grafo[v][w])){
                        distancias[m][w] = (distancias[m][v] + grafo[v][w]); //si se encuentra una distancia menor al camino directo, se actualiza
                      }
                }
            } //fin para cada w en el conjunto noVisitados
        } //fin repetir n-2
    } //Fin bucle principal
    free(noVisitados);
} //fin procedimiento

void test_dijkstra(){
  int i,j;
  matriz d,m;
  int temp[][5] = {{0,1,8,4,7},{1,0,2,6,5},{8,2,0,9,5},{4,6,9,0,3},{7,5,5,3,0}};
  d = crearMatriz(5);
  m = crearMatriz(5);
  iniMatriz(m,5);
  iniMatriz(d,5);
  for (i = 0; i < 5; i++)
    for (j = 0; j < 5; j++) m[i][j] = temp[i][j];
  printf("Test de funcionamiento del algoritmo de Dijkstra:\n");
  printf("Matriz de adyacencia del grafo:\n");
  printm(m,5);
  printf("Tabla de distancias mínimas:\n");
  dijkstra(m,d,5);
  printm(d,5);
  if (igualaref(d))
    printf("Es correcto\n");
  else printf("No es correcto\n");
  liberarMatriz(d,5);
  liberarMatriz(m,5);
}

void test_dijkstra_2(){
  int i,j;
  matriz d,m;
  int temp[][4] =
    {{0,1,4,7},
    {1,0,2,8},
    {4,2,0,3},
    {7,8,3,0}};
  d = crearMatriz(5);
  m = crearMatriz(5);
  iniMatriz(m,5);
  iniMatriz(d,5);
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++) m[i][j] = temp[i][j];
  printf("Test de funcionamiento del algoritmo de Dijkstra:\n");
  printf("Matriz de adyacencia del grafo:\n");
  printm(m,4);
  printf("Tabla de distancias mínimas:\n");
  dijkstra(m,d,4);
  printm(d,4);
  if (igualaref2(d))
    printf("Es correcto\n");
  else printf("No es correcto\n");
  liberarMatriz(d,5);
  liberarMatriz(d,5);
}

double medir_tiempo(int tam, int k){
//K es el número de repeticiones en caso de tener que medir tiempos pequeños
    double t_inicio = 0.0; //tiempo de inicio
    double t_fin = 0.0; //tiempo de fin
    double t_test = 0.0; //tiempo del test
    double t_test_init = 0.0; //tiempo de test+inicializacion
    double t_init = 0.0; //tiempo de solo inicialización
    int i; //Iterador
    //f_init es la función de inicialización
    //algoritmo es la función del algoritmo
    matriz mat;
    matriz distancias; //matrices iniciales y de salida
    mat = crearMatriz(tam);
    distancias = crearMatriz(tam);
    iniMatriz(mat, tam);
    t_inicio = microsegundos();
    dijkstra(mat, distancias, tam); //aplica el dijkstra indicado
                       //por parámetro a la matriz generada
    t_fin = microsegundos();
    t_test = t_fin - t_inicio;
    if (t_test < 500){
        printf("(*)");
        t_inicio = microsegundos();
        for (i = 0; i < k; i++){
            iniMatriz(mat, tam);
            dijkstra(mat, distancias, tam);
        }
        t_fin = microsegundos();
        t_test_init = t_fin-t_inicio;
        t_inicio = microsegundos();
        for (i = 0; i<k; i++){
            iniMatriz(mat, tam);
        }
        t_fin = microsegundos();
        t_init = t_fin - t_inicio;
        t_test = (t_test_init - t_init) /k;
    }
    liberarMatriz(mat, tam);
    liberarMatriz(distancias, tam);
    return t_test;
}

void print_dijkstra(){
    int k = 1000;
    double tiempo = 0.0;
    int n; //Iterador //Algoritmo Dijkstra:
    printf("\n\tAlgoritmo de Dijkstra:\n");
    printf("\t       n\t\t\t   t(n)\t\t   t(n)/(n^2.8)"
            "\t\t  t(n)/(n^2.98)\t\t   t(n)/(n^3.2)\n");
    for (n=8; n<=2000; n*=2){
        tiempo = medir_tiempo(n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,2.8)),
               tiempo/(pow(n,2.98)),
               tiempo/(pow(n,3.3)));
    }
    printf("\n");
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
}

int main(int argc, char const *argv[]){
    inicializar_semilla();
    test_dijkstra();
    printf("\n");
    test_dijkstra_2();
    print_dijkstra();
    return 0;
}
