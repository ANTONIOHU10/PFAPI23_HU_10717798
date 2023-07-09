#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//todo struttura max heap per le auto
struct Heap_auto{
    //è un tipo di search tree, ma utilizza un array dinamico per salvare i dati
    int *autonomie;
    int numero_auto;
    int capacita;
};


typedef struct Heap_auto heap_auto;

heap_auto* creazioneHeapAuto(int numero, int* valori);
void inserimentoAusiliare(heap_auto* h, int indice);
void maxHeapify(heap_auto* h, int indice);
int estrarreMassimo(heap_auto* h);
int ottengoMassimo(heap_auto* h);
void rimuoviElemento(heap_auto* h,int elemento);
void inserimento(heap_auto* h, int data);
void printHeap(heap_auto* h);

int main() {
    /*
    //test
    int arr[9] = {1,2,3,4,5,6,7,8,9};
    heap_auto* heap = creazioneHeapAuto(9,arr);
    printHeap(heap);
    estrarreMassimo(heap);
    printHeap(heap);
    */

    //file da leggere
    FILE *file = fopen("open_8.txt","r");

    //file da scrivere
    FILE *file_out = fopen("a_OUTPUT_TEST.txt","w");

    // = stdin; ottengo il caso di stdin
    if(file == NULL){
        //printf("Errore apertura file\n");
        perror("Errore apertura file");
        exit(1);
        //return -1;
    }
    //dichiarazione per lettura
    char operazione[20];
    int distanza;
    int numero_auto;
    int autonomia;

    int autonomie[512];
    // valore estremo

    //leggere una riga di comando
    while (fscanf(file,"%s",operazione) != EOF){

        if(strcmp(operazione,"aggiungi-stazione") == 0) {
            if(fscanf(file,"%d %d",&distanza, &numero_auto) != EOF){
                for (int i = 0; i < numero_auto; i++){
                    if(fscanf(file,"%d",&autonomie[i]) != EOF){
                        //leggo una autonomia
                    }

                }
                //finisce di leggere le autonomie
                fprintf(file_out,"(aggiungi stazione)  distanza: %d    numero_auto: %d\n",distanza,numero_auto);

                for(int j = 0; j < numero_auto; j++){
                    fprintf(file_out,"autonomia: %d\n",autonomie[j]);
                }

            }
        }

        if(strcmp(operazione,"demolisci-stazione") == 0) {
            if(fscanf(file,"%d",&distanza) != EOF){
                fprintf(file_out,"(demolisci stazione)   distanza: %d\n",distanza);
            }
        }

        if(strcmp(operazione,"aggiungi-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza, &autonomia) != EOF){
                fprintf(file_out,"(aggiungi auto)   distanza: %d    autonomia: %d\n",distanza,autonomia);
            }
        }

        if(strcmp(operazione, "rottama-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza,&autonomia) != EOF){
                fprintf(file_out,"(rottama auto)   stazione: %d  autonomia: %d\n",distanza,autonomia);
            }
        }

        if(strcmp(operazione,"pianifica-percorso") == 0) {
            int distanza_destinazione;
            if(fscanf(file,"%d %d",&distanza,&distanza_destinazione) != EOF){
                fprintf(file_out,"(pianifica percorso)   partenza: %d  destinazione: %d\n",distanza,distanza_destinazione);
            }
        }

    }

    fclose(file);
    fclose(file_out);

    return 0;
}

heap_auto* creazioneHeapAuto(int numero, int* valori){

    //allocare memoria per la struttura heap iniziale
    heap_auto *heap = (heap_auto*)malloc(sizeof(heap_auto));

    //verifico se la memoria è stata allocata correttamente
    if(heap == NULL){
        printf("Errore allocazione memoria heap\n");
        return NULL;
    }
    //inizializzo la struttura
    heap-> numero_auto = 0;
    heap-> capacita = numero;
    //allocare memoria per le auto
    heap->autonomie = (int*)malloc(numero * sizeof(int));
    //verifico se la memoria è stata allocata correttamente
    if(heap->autonomie == NULL){
        printf("Errore allocazione memoria heap\n");
        return NULL;
    }
    int i;
    for(i = 0; i < numero; i++){
        heap->autonomie[i] = valori[i];
    }
    heap->numero_auto = i;
    i = (heap->numero_auto - 2) / 2;
    while (i >= 0){
        maxHeapify(heap,i);
        i--;
    }
    return heap;
}

//funzione che confronta due valori (padre e attuale), se è maggiore l'attuale, allora scambio
void inserimentoAusiliare(heap_auto* h, int indice){
    //trova l'indice del padre dell'indice attuale
    int padre = (indice - 1) / 2;

    if(h->autonomie[padre] < h->autonomie[indice]){
        //scambio
        int temp = h->autonomie[padre];
        h->autonomie[padre] = h->autonomie[indice];
        h->autonomie[indice] = temp;

        //richiamo la funzione per il padre
        inserimentoAusiliare(h,padre);
    }
}

void maxHeapify(heap_auto* h, int indice){
    //trova l'indice del figlio sinistro e destro
    int sinistro = indice * 2 + 1;
    int destro = indice * 2 + 2;
    // indice del valore massimo, suppongo che quello attuale sia il massimo
    int max = indice;
    //fixme ---------------------------------------------------
    //verifico se l'indice del figlio sinistro e destro supera il limite, se supera il limite, allora non esiste, metto -1
    if(sinistro >= h->numero_auto || sinistro <0){
        sinistro = -1;
    }

    if(destro >= h->numero_auto || destro <0){
        destro = -1;
    }

    if(sinistro != -1 && h->autonomie[sinistro] > h->autonomie[max]){
        max = sinistro;
    }
    if(destro != -1 && h->autonomie[destro] > h->autonomie[max]){
        max = destro;
    }
    //cambio i nodi
    if(max != indice){
        int temp = h->autonomie[max];
        h->autonomie[max] = h->autonomie[indice];
        h->autonomie[indice] = temp;
        //richiamo la funzione per il figlio
        maxHeapify(h,max);
    }
}

//fixme: da controllare
int ottengoMassimo(heap_auto* h) {
    if (h->numero_auto == 0) {
        printf("Heap è vuoto\n");
        return -999;
    }

    return h->autonomie[0];
}

int estrarreMassimo(heap_auto* h){
    int elementoDaTogliere;

    //se il heap è vuoto
    if(h->numero_auto == 0){
        printf("Heap vuoto\n");
        return -999;
    }

    //salvo il valore da togliere in elementoDaTogliere
    elementoDaTogliere = h->autonomie[0];

    //scambio il nodo del valore tolto con l'ultimo
    h->autonomie[0] = h->autonomie[h->numero_auto - 1];
    //decremento la dimensione di heap
    h->numero_auto--;

    //ricorsione
    maxHeapify(h,0);
    return elementoDaTogliere;
}

//fixme: da controllare
void rimuoviElemento(heap_auto* h, int elemento) {
    // trovare l'indice dell'elemento da togliere
    int indice = -1;
    for (int i = 0; i < h->numero_auto; i++) {
        if (h->autonomie[i] == elemento) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Elemento non trovato");
        return;
    }

    // Scambia l'elemento da togliere con l'ultimo elemento
    int indiceUltimo = h->numero_auto - 1;
    int temp = h->autonomie[indice];
    h->autonomie[indice] = h->autonomie[indiceUltimo];
    h->autonomie[indiceUltimo] = temp;

    // Riduco la sua dimensione
    h->numero_auto--;

    // Modello la struttura heap
    maxHeapify(h, indice);
}

void inserimento(heap_auto* h, int data){
    //controllo se il heap è pieno o no
    if(h->numero_auto < h->capacita){
        //inserisco il valore in una array
        h->autonomie[h->numero_auto] = data;

        //chiamo la funzione per modellare la struttura
        inserimentoAusiliare(h,h->numero_auto);

        //incremento il numero di elementi
        h->numero_auto++;
    }
}

void printHeap(heap_auto* h){
    int i;
    printf("Heap: ");
    for(i = 0; i < h->numero_auto; i++){
        printf("%d ",h->autonomie[i]);
    }
    printf("\n");
}

