#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

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

void ord_ins(int vec[], int n) {
    int i;// iterador
    int x;// variable temporal
    int j;// iterador
    for (i  = 1; i < n; i++) {
        x = vec[i];
        j = i-1;
        for (;j >= 0 && vec[j] > x;j--) vec[j+1] = vec[j];
        vec[j+1] = x;
    }
}

void ord_shell(int vec[], int n){
    int incremento = n;
    int tmp = 0;
    int j = 0;
    int seguir;
    int i; //Iterador
    do{
        incremento /= 2;
        for (i = incremento ; i < n; i++){
            tmp = vec[i];
            j = i;
            seguir = 1;
            while ((j-incremento >= 0) && seguir){
                if (tmp < vec[j-incremento]){
                    vec[j] = vec[j-incremento];
                    j=j-incremento;
                }
                else seguir=0;
            }
            vec[j]=tmp;
        }
    } while (incremento > 1);
}

void test_insercion(){
    int vector[25];
    printf("Test algoritmo ordenación:\n");
    printf("\tVector ordenado:\n");
    ascendente(vector,25);
    printv(vector,25);
    ord_ins(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    printf("\n");

    printf("\tVector ordenado al revés:\n");
    descendente(vector,25);
    printv(vector,25);
    ord_ins(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    printf("\n");

    printf("\tVector aleatorio:\n");
    aleatorio(vector,25);
    printv(vector,25);
    ord_ins(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    printf("\n");
}

void test_shell(){
    int vector[25];
    printf("Test algoritmo shell:\n");
    printf("\tVector ordenado:\n");
    ascendente(vector,25);
    printv(vector,25);
    ord_shell(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    printf("\n");

    printf("\tVector ordenado al revés:\n");
    descendente(vector,25);
    printv(vector,25);
    ord_shell(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
    printf("\n");

    printf("\tVector aleatorio:\n");
    aleatorio(vector,25);
    printv(vector,25);
    ord_shell(vector,25);
    printf("\tDespués de aplicar el algoritmo:\n");
    printv(vector,25);
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
    int *v;
    //f_init es la función de inicialización
    //algoritmo es la función de ordenación

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
    free(v);
    return t_test;
}

void print_ins(){
    int k = 1000; //ejecuciones del algoritmo para tiempos pequeños
    double tiempo = 0.0;
    int n;
    //Algoritmo 1
    printf("Ordenación por inserción: \n");
    printf("\tVector de entrada ordenado:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^0.8\t\t         t(n)/n\t\t"
           "     t(n)/n^1.2\n");
    for (n=500; n<=2048000; n*=2){
        tiempo = medir_tiempo(ord_ins, ascendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
        n, tiempo, tiempo/(pow(n,0.8)), tiempo/n, tiempo/(pow(n,1.2)));
    }
    printf("\n");

    printf("\tVector de entrada ordenado al revés:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^1.8\t\t     t(n)/n^2.0\t\t"
           "     t(n)/n^2.2\n");
    for (n=500; n<=64000; n*=2){
        tiempo = medir_tiempo(ord_ins, descendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
        n, tiempo, tiempo/(pow(n,1.8)), tiempo/(pow(n,2)),
        tiempo/(pow(n,2.2)));
    }
    printf("\n");

    printf("\tVector de entrada aleatorio:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^1.8\t\t     t(n)/n^2.0\t\t"
           "     t(n)/n^2.2\n");
    for (n=500; n<=128000; n*=2){
        tiempo = medir_tiempo(ord_ins, aleatorio, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
        n, tiempo, tiempo/(pow(n,1.8)),
        tiempo/(pow(n,2)), tiempo/(pow(n,2.2)));
    }
    printf("\n");
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);

}

void print_shell(){
    int k = 1000;
    double tiempo = 0.0;
    int n; //Iterador
        //Algoritmo Shell
    printf("Ordenación de Shell: \n");
    printf("\tVector de entrada ordenado:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^0.8"
    "\t  t(n)/(n^1.01*log2(n))\t\t     t(n)/n^1.2\n");
    for (n=500; n<=2048000; n*=2){
        tiempo = medir_tiempo(ord_shell, ascendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/(pow(n,0.8)), tiempo/(pow(n,1.01)*log2(n)),
               tiempo/(pow(n,1.2)));
    }
    printf("\n");

    printf("\tVector de entrada ordenado al revés:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^1.0\t\t    t(n)/n^1.09\t\t"
           "     t(n)/n^1.3\n");
    for (n=500; n<=2048000; n*=2){
        tiempo = medir_tiempo(ord_shell, descendente, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
               n, tiempo, tiempo/n,
               tiempo/(pow(n,1.09)), tiempo/(pow(n,1.3)));
    }
    printf("\n");

    printf("\tVector de entrada aleatorio:\n");
    printf("\t      n\t\t\t\t  t(n)\t\t     t(n)/n^1.0\t\t    t(n)/n^1.24\t\t"
           "     t(n)/n^1.4\n");
    for (n=500; n<=2048000; n*=2){
        tiempo = medir_tiempo(ord_shell, aleatorio, n, k);
        printf("\t% 8d\t\t% 15.4f\t\t% 14.12f\t\t% 14.12f\t\t% 14.12f\n",
        n, tiempo, tiempo/n, tiempo/(pow(n,1.24)),
               tiempo/(pow(n,1.4)));
    }
    printf("\n");
    printf("\n\n (*) Tiempo promedio en %d ejecuciones del algoritmo\n\n",k);
}

int main(int argc, char const *argv[]){
    inicializar_semilla();
    //test_insercion();
    //test_shell();
    print_ins();
    print_shell();
    return 0;
}
