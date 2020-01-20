#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#define MAXSIZE 2048000

void inicializar_semilla() {
    srand(time(NULL));
    /* se establece la semilla de una nueva serie de enteros
       pseudo-aleatorios */
}

void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
    /* se generan números pseudoaleatorio entre -n y +n */
}

void ascendente(int v[],int n) {
    int i; // iterador
    for(i = 0; i < n; i++) v[i] = i;
}

void descendente(int v[],int n) {
    int i; // iterador
    for(i = 0; i < n; i++) v[i] = n-i-1;
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

typedef struct {
    int vec[MAXSIZE];
    int ultimo;
} monticulo;

/*Montículos en C
    Dada una posición i del vector del montículo
    El HijoIzquierdo es 2*i +1
    El HijoDerecho es 2*i +2
    El Padre es (i-1) div 2
*/

void intercambiar(int *a, int *b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void flotar(monticulo *M, int i){
  int pad,hijo;
  hijo = i;
  pad = (hijo-1)/2;
  //Cuidado con la condición del bucle,
  //puede ser necesario un centinela de posición
  while (M->vec[pad] < M->vec[hijo]) {
    intercambiar(&(M->vec[pad]),&(M->vec[hijo]));
    hijo = pad;
    pad /= 2;
  }
}

void crearMonticuloMaximos(int v[],int n,monticulo *M) {
  int i;
  for (i = 0; i < n; i++) {
    M->vec[++M->ultimo] = v[i];
    flotar(M,M->ultimo);
  }
}

void hundir(monticulo *M, int i){
    int HijoDer, HijoIzq, j;
    do
    {
        HijoIzq = 2*i +1;
        HijoDer = 2*i +2;
        j = i;
        if (HijoDer <= M->ultimo && M->vec[HijoDer] < M->vec[i]){
            i = HijoDer;
        }
        if (HijoIzq <= M->ultimo && M->vec[HijoIzq] < M->vec[i]){
            i = HijoIzq;
        }
        intercambiar(&(M->vec[i]),&(M->vec[j]));
    } while (i!=j);
}

void crearMonticuloMinimos(int v[],int n, monticulo * m) {
    int i;
    m->ultimo = -1;

    for (i = 0; (i < n) && (i < MAXSIZE); i++)
        m->vec[i] = v[i];
    m->ultimo = i;

    for (i = (m->ultimo-1) / 2 ; i >= 0; i--)
        hundir(m,i);
}

int consultarMenor(const monticulo *M){
    return M->vec[0];
}

int quitarMenor(monticulo *M){
    int x = M->vec[0];
    M->vec[0]=M->vec[M->ultimo--];
    hundir(M, 0);
    return x;
}

void heapsort(int v[], int tam){
    int i;
    monticulo M;
    crearMonticuloMinimos(v,tam,&M);
    for (i = 0; i < tam; i++)
    {
        v[i]=quitarMenor(&M);
    }
}

int esMonticuloMin(monticulo *M){
  int i;
  for (i = 1; i<=M->ultimo; i++)
    if(M->vec[i] > M->vec[(i-1)/2])
      return 0;
  return 1;
}

int esMonticuloMax(monticulo *M){
  int i;
  for (i = 1; i<=M->ultimo; i++)
    if(M->vec[i] < M->vec[(i-1)/2])
      return 0;
  return 1;
}

int estaOrdenado(int v[], int tam){
  int i;
  for (i=1; i<tam; i++)
    if(v[i-1]>v[i])
      return 0;
  return 1;
}

/*
void test_crearmonticulo(){
  int vector[25];
  monticulo* M;
  M = malloc(sizeof(monticulo));
  printf("Test algoritmo creación montículos:\n");
  printf("\tVector ordenado:\n");
  ascendente(vector,25);
  printv(vector,25);
  crearMonticuloMinimos(vector,25,M);
  printf("\tMontículo creado:\n");
  printv(M->vec,25);
  if (esMonticuloMin(M)) printf("ERROR: Creación de montículo\n");
  else printf("Montículo creado correctamente\n");
  printf("\n"); free(M);

  M = malloc(sizeof(monticulo));
  printf("\tVector ordenado al revés:\n");
  descendente(vector,25);
  printv(vector,25);
  crearMonticuloMinimos(vector,25,M);
  printf("\tMonticulo creado:\n");
  printv(M->vec,25);
  if (esMonticuloMin(M)) fprintf(stderr,"ERROR: Creación de montículo\n");
  else printf("Montículo creado correctamente\n");
  printf("\n");  free(M);

  M = malloc(sizeof(monticulo));
  printf("\tVector aleatorio:\n");
  aleatorio(vector,25);
  printv(vector,25);
  crearMonticuloMinimos(vector,25,M);
  printf("\tMonticulo creado:\n");
  printv(M->vec,25);
  if (esMonticuloMin(M)) fprintf(stderr,"ERROR: Creación de montículo\n");
  else printf("Montículo creado correctamente\n");
  printf("\n"); free(M);
}
*/

void test_crearmonticulo(void(funCrear)(int[], int, monticulo *),
                         int(esMonticulo)(monticulo *)){
  int vector[25];
  monticulo* M;
  M = malloc(sizeof(monticulo));
  printf("Test algoritmo creación montículos:\n");
  printf("\tVector ordenado:\n");
  ascendente(vector,25);
  printv(vector,25);
  funCrear(vector,25,M);
  printf("\tMontículo creado:\n");
  printv(M->vec,25);
  if (esMonticulo(M)) printf("ERROR: Creación de montículo\n");
  else printf("Montículo creado correctamente\n");
  printf("\n"); free(M);

  M = malloc(sizeof(monticulo));
  printf("\tVector ordenado al revés:\n");
  descendente(vector,25);
  printv(vector,25);
  funCrear(vector,25,M);
  printf("\tMonticulo creado:\n");
  printv(M->vec,25);
  if (esMonticulo(M)) fprintf(stderr,"ERROR: Creación de montículo\n");
  else printf("Montículo creado correctamente\n");
  printf("\n");  free(M);

  M = malloc(sizeof(monticulo));
  printf("\tVector aleatorio:\n");
  aleatorio(vector,25);
  printv(vector,25);
  funCrear(vector,25,M);
  printf("\tMonticulo creado:\n");
  printv(M->vec,25);
  if (esMonticulo(M)) fprintf(stderr,"ERROR: Creación de montículo\n");
  else printf("Montículo creado correctamente\n");
  printf("\n"); free(M);
}

void test_heapsort(){
    int vector[25];
    printf("Test algoritmo ordenación:\n");
    printf("\tVector ordenado:\n");
    ascendente(vector,25);
    printv(vector,25);
    heapsort(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    if (estaOrdenado(vector,25)) printf("Vector ordenado correctamente\n");
    else printf("ERROR: Ordenación del vector");
    printf("\n");

    printf("\tVector ordenado al revés:\n");
    descendente(vector,25);
    printv(vector,25);
    heapsort(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    if (estaOrdenado(vector,25)) printf("Vector ordenado correctamente\n");
    else printf("ERROR: Ordenación del vector");
    printf("\n");

    printf("\tVector aleatorio:\n");
    aleatorio(vector,25);
    printv(vector,25);
    heapsort(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    if (estaOrdenado(vector,25)) printf("Vector ordenado correctamente\n");
    else printf("ERROR: Ordenación del vector");
    printf("\n");
}

double medir_tiempo(void (* algoritmo)(int v[], int tam),
                    void (* f_init)(int v[], int tam), int tam, int k){
//K es el número de repeticiones en caso de tener que medir tiempos pequeños
    double t_inicio = 0.0; //tiempo de inicio
    double t_fin = 0.0; //tiempo de fin
    double t_test = 0.0; //tiempo del test
    double t_test_init = 0.0; //tiempo de test+inicializacion
    double t_init = 0.0; //tiempo de solo inicialización
    int i; //Iterador
    //f_init es la función de inicialización
    //algoritmo es la función de ordenación
    int *v;
    v = malloc(tam*sizeof(int));
    f_init(v, tam); //genera un vector de [tam] enteros
    t_inicio = microsegundos();
    algoritmo(v, tam); //aplica el algoritmo indicado
                       //por parámetro al vector generado
    t_fin = microsegundos();
    t_test = t_fin - t_inicio;
    if (t_test < 500){
        printf("(*)");
        t_inicio = microsegundos();
        for (i = 0; i < k; i++){
            f_init(v, tam);
            algoritmo(v,tam);
        }
        t_fin = microsegundos();
        t_test_init = t_fin-t_inicio;
        t_inicio = microsegundos();
        for (i = 0; i<k; i++){
            f_init(v, tam);
        }
        t_fin = microsegundos();
        t_init = t_fin - t_inicio;
        t_test = (t_test_init - t_init) /k;
    }
    free(v); return t_test;
}

void adaptadorMin(int v[],int n) {
  monticulo * m = malloc(sizeof(monticulo));
  crearMonticuloMinimos(v,n,m);
  free(m);
}

void adaptadorMax(int v[],int n) {
  monticulo * m = malloc(sizeof(monticulo));
  crearMonticuloMaximos(v,n,m);
  free(m);
}

void print_crearmonticulo(){
    int k = 1000;
    double tiempo = 0.0;
    int n; //Iterador
        //Creación de Monticulos
    printf("Creación de Montículos: \n");
    printf("\tVector de entrada ordenado:\n");
    printf("\t       n\t\t\t   t(n)\t\t     t(n)/n^0.9\t\t   \t t(n)/n\t\t"
           "     t(n)/n^1.2\n");
    for (n=500; n<=512000; n*=2){
        tiempo = medir_tiempo(adaptadorMin, ascendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.9)), tiempo/n,
               tiempo/(pow(n,1.2)));
    }

    printf("\n\tVector de entrada ordenado al revés:\n");
    printf("\t       n\t\t\t   t(n)\t\t     t(n)/n^0.8\t\t   \t t(n)/n\t\t"
           "     t(n)/n^1.2\n");
    for (n=500; n<=MAXSIZE; n*=2){
        tiempo = medir_tiempo(adaptadorMin, descendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)), tiempo/n,
               tiempo/(pow(n,1.2)));
    }

    printf("\n\tVector de entrada aleatorio:\n");
    printf("\t       n\t\t\t   t(n)\t\t     t(n)/n^0.8\t\t    \t t(n)/n\t\t"
           "     t(n)/n^1.2\n");
    for (n=500; n<=512000; n*=2){
        tiempo = medir_tiempo(adaptadorMin, aleatorio, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)), tiempo/n,
               tiempo/(pow(n,1.2)));
    }
    printf("\n\n\n"
    " (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
}

void print_heapsort(){
    int k = 1000;
    double tiempo = 0.0;
    int n; //Iterador //Algoritmo Ordenación Monticulos:
    printf("Ordenación por Montículos: \n\tVector de entrada ordenado:\n");
    printf("\t       n\t\t\t   t(n)\t   t(n)/(n^0.8)*log2(n)"
    "\t       t(n)/(n^0.98*log2(n))\t   t(n)/(n^1.2)*log2(n)\n");
    for (n=500; n<=256000; n*=2){
        tiempo = medir_tiempo(heapsort, ascendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)*log2(n)),
               tiempo/(pow(n,0.98)*log2(n)),
               tiempo/(pow(n,1.2)*log2(n)));
    }
    printf("\n\tVector de entrada ordenado al revés:\n");
    printf("\t       n\t\t\t   t(n)\t   t(n)/(n^0.8)*log2(n)"
    "\t       t(n)/(n^0.98*log2(n))\t   t(n)/(n^1.2)*log2(n)\n");
    for (n=500; n<=256000; n*=2){
        tiempo = medir_tiempo(heapsort, descendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)*log2(n)),
               tiempo/(pow(n,0.98)*log2(n)),
               tiempo/(pow(n,1.2)*log2(n)));
    }
    printf("\n\tVector de entrada aleatorio:\n");
    printf("\t       n\t\t\t   t(n)\t   t(n)/(n^0.8)*log2(n)"
    "\t       t(n)/(n^1.02*log2(n))\t   t(n)/(n^1.2)*log2(n)\n");
    for (n=500; n<=512000; n*=2){
        tiempo = medir_tiempo(heapsort, aleatorio, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)*log2(n)),
               tiempo/(pow(n,1.02)*log2(n)),
               tiempo/(pow(n,1.2)*log2(n)));
    }
    printf("\n");
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
}

int main(int argc, char const *argv[]){
    inicializar_semilla();
    test_crearmonticulo(crearMonticuloMaximos,esMonticuloMax);
    test_crearmonticulo(crearMonticuloMinimos,esMonticuloMin);
    //test_heapsort();
    //print_crearmonticulo();
    //print_heapsort();
    return 0;
}
