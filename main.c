#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DIMENSIONE_PARCHEGGIO 512
//struttura per le auto
struct Heap_auto{
    //è un tipo di search tree, ma utilizza un array dinamico per salvare i dati
    int *autonomie;
    int numero_auto;
    int capacita;
};
typedef struct Heap_auto heap_auto;

//struttura per le stazioni, AVL tree
struct Stazione{
    int distanza;
    struct Stazione* sinistro;
    struct Stazione* destro;
    int altezza;
    struct Heap_auto* heap_auto;
};
typedef struct Stazione stazione;


heap_auto* creazioneHeapAuto(int numero, int* valori);
void inserimentoAusiliare(heap_auto* h, int indice);
void maxHeapify(heap_auto* h, int indice);
int estrarreMassimo(heap_auto* h);
int ottengoMassimo(heap_auto* h);
void rimuoviElemento(heap_auto* h,int elemento);
void inserimento(heap_auto* h, int data);
void printHeap(heap_auto* h);



int ottieniAltezza(stazione* nodo);
int massimo(int a, int b);
stazione* creaNodo(int distanza, heap_auto* heapAuto);
int ottieniDifferenza(stazione* nodo);
void aggiornoAltezza(stazione* nodo);
stazione* rotazioneDestra(stazione* y);
stazione* rotazioneSinistra(stazione* x);
stazione* inserimentoAVL(stazione* stazione, int distanza, heap_auto* heapAuto);
stazione* trovaMinimo(stazione* nodo);
stazione* cancellaNodo(stazione* root, int distanza);
void freeAVL(stazione* root);
stazione* trovaStazione(stazione* root, int distanza);
void printAVL(stazione* root);
int esisteStazione(stazione* root, int distanza);

int main() {


    //TODO----------------TEST AVL-------------------------------
    stazione* radice = NULL;
    /*
    //fixme, forse perché non ho specificato capacita quindi c'è il problema di confronto

    int array1[10] = {1,2,3,4,5,6,7,8,9,10};
    int array2[9] = {1,2,3,4,5,6,7,8,9};
    int array3[8] = {1,2,3,4,5,6,7,8};
    heap_auto* heap1 = creazioneHeapAuto(10,array1);
    heap_auto* heap2 = creazioneHeapAuto(9,array2);
    heap_auto* heap3 = creazioneHeapAuto(8,array3);
    int distanza1 = 10;
    int distanza2 = 20;
    int distanza3 = 30;
    radice = inserimentoAVL(radice,distanza1,heap1);
    radice = inserimentoAVL(radice,distanza2,heap2);
    radice = inserimentoAVL(radice,distanza3,heap3);

    //printf("%d\n",ottengoMassimo(trovaStazione(radice, 10)->heap_auto));
    inserimento(heap1, 999);
    inserimento(heap2, 999);
    inserimento(heap3, 999);
    printHeap(trovaStazione(radice, 10)->heap_auto);
    printHeap(trovaStazione(radice, 20)->heap_auto);
    printHeap(trovaStazione(radice, 30)->heap_auto);

    //cancellaNodo(radice, 10);
    printf("---------------------------------------------------------\n");
    printAVL(radice);

    //freeAVL(radice);
    */

    //TODO----------------TEST AVL-------------------------------
    //file da leggere
    FILE *file = fopen("open_111.txt","r");
    printf("---------------------------------------------------------\n");

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
                /*
                //finisce di leggere le autonomie
                fprintf(file_out,"(aggiungi stazione)  distanza: %d    numero_auto: %d\n",distanza,numero_auto);

                for(int j = 0; j < numero_auto; j++){
                    fprintf(file_out,"autonomia: %d\n",autonomie[j]);
                }
                */

               //non devo resettare il vettore, perché comunque legge solo N auto, quindi quelle rimaste non vengono lette
               //quelle che servono vengono sovrascritti
               //quindi devo passare il numero di auto che voglio leggere

               //verifico se il numero di auto è superiore di 512
               if(numero_auto > DIMENSIONE_PARCHEGGIO) {
                   printf( "numero auto superiore a 512\n");
               } else {
                   //verifico se esiste già la stazione
                   if(trovaStazione(radice,distanza)!=NULL){
                       printf("esiste già la stazione %d \n",distanza);
                       fprintf(file_out,"non aggiunta\n");
                   }
                   else {
                       heap_auto* heapAuto = creazioneHeapAuto(numero_auto,autonomie);
                       radice = inserimentoAVL(radice,distanza,heapAuto);
                       fprintf(file_out,"aggiunta\n");
                   }
               }

            }
        }

        if(strcmp(operazione,"demolisci-stazione") == 0) {
            if(fscanf(file,"%d",&distanza) != EOF){

                //fprintf(file_out,"(demolisci stazione)   distanza: %d\n",distanza);

                //esiste la stazione da togliere
                if(esisteStazione(radice,distanza) == 1){
                    cancellaNodo(radice,distanza);
                    fprintf(file_out,"demolita\n");
                }
                //non esiste la stazione da togliere
                else {
                    fprintf(file_out,"non demolita\n");
                }



            }
        }

        if(strcmp(operazione,"aggiungi-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza, &autonomia) != EOF){
                //fprintf(file_out,"(aggiungi auto)   distanza: %d    autonomia: %d\n",distanza,autonomia);


                //------------------------------------------se non esiste la stazione, non posso aggiungere l'auto----------------------------------------------
                if(esisteStazione(radice,distanza) == 1){
                    //-------------------------se supera il limite-----------------------------------------
                   if(trovaStazione(radice,distanza)->heap_auto->numero_auto < DIMENSIONE_PARCHEGGIO){
                       int a = trovaStazione(radice,distanza)->heap_auto->numero_auto;
                       //printf("numero auto stazione %d prima: %d\n",distanza,a);
                       inserimento(trovaStazione(radice,distanza)->heap_auto,autonomia);

                       int b = trovaStazione(radice,distanza)->heap_auto->numero_auto;
                       //printf("numero auto stazione %d dopo: %d\n",distanza,b);
                       if(a < b){
                           fprintf(file_out,"aggiunta\n");
                       }
                       else {
                           fprintf(file_out,"non aggiunta\n");
                       }
                    } else {
                       fprintf(file_out, "non aggiunta\n");
                        }

                } else {
                    printf("stazione %d non esiste\n",distanza);
                    fprintf(file_out,"non aggiunta\n");
                }


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
    //todo ---------------------------------------------------stampa AVL-----------------------------------------------------
    /*
    printAVL(radice);
    rimuoviElemento(trovaStazione(radice, 235)->heap_auto,20);
    printf("---------------------------------------------------------\n");
    printAVL(radice);
    */
    printf("---------------------------------------------------------\n");
    //printAVL(radice);
    freeAVL(radice);
    fclose(file);
    fclose(file_out);

    return 0;
}

heap_auto* creazioneHeapAuto(int numero, int* valori){
    //printf("numero di auto richiesto per inserire: %d\n",numero);
    //allocare memoria per la struttura heap iniziale
    heap_auto *heap = (heap_auto*)malloc(sizeof(heap_auto));

    //verifico se la memoria è stata allocata correttamente
    if(heap == NULL){
        printf("Errore allocazione memoria heap\n");
        return NULL;
    }
    //inizializzo la struttura
    heap-> numero_auto = 0;
    //ma non so se spreca memoria
    heap-> capacita = DIMENSIONE_PARCHEGGIO;
    //allocare memoria per le auto
    heap->autonomie = (int*)malloc(DIMENSIONE_PARCHEGGIO* sizeof(int));
    //verifico se la memoria è stata allocata correttamente
    if(heap->autonomie == NULL){
        printf("Errore allocazione memoria heap\n");
        return NULL;
    }
    int i;
    //eventuale problema con variabile numero / DIMENSIONE_PARCHEGGIO
        //con numero funziona fino a test 5
        //con DIMENSIONE_PARCHEGGIO lunghezza output strano,numero auto strano,  ma non da errore

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



void maxHeapify(heap_auto* h, int indice){
    //trova l'indice del figlio sinistro e destro
    int sinistro = indice * 2 + 1;
    int destro = indice * 2 + 2;
    // indice del valore massimo, suppongo che quello attuale sia il massimo
    int max = indice;
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

void printHeap(heap_auto* h){
    int i;
    printf("Heap: ");
    for(i = 0; i < h->numero_auto; i++){
        printf("%d ",h->autonomie[i]);
    }
    printf("\n");
}

int ottieniAltezza(stazione* nodo){
    if(nodo == NULL){
        return 0;
    }
    return nodo->altezza;
}

int massimo(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}
//creazione un nodo nuovo
stazione* creaNodo(int distanza, heap_auto* heapAuto){
    stazione *nodo = (stazione*)malloc(sizeof(stazione));
    nodo->distanza = distanza;
    nodo->sinistro = NULL;
    nodo->destro = NULL;
    nodo->altezza = 1;

    nodo->heap_auto = heapAuto;
    return nodo;
}

//funzione che tiene conto della differenza di altezza tra il nodo sinistro e destro
//perché in un albero AVL, la differenza di altezza tra il nodo sinistro e destro non può essere maggiore di 1
int ottieniDifferenza(stazione* nodo){
    if(nodo == NULL){
        return 0;
    }
    return ottieniAltezza(nodo->sinistro) - ottieniAltezza(nodo->destro);
}

void aggiornoAltezza(stazione* nodo){
    if(nodo == NULL){
        return;
    }
    nodo->altezza = 1 + massimo(ottieniAltezza(nodo->sinistro),ottieniAltezza(nodo->destro));
}

//rotazione destra
stazione* rotazioneDestra(stazione* y){
    stazione* x = y->sinistro;
    stazione* T2 = x->destro;

    //rotazione
    x->destro = y;
    y->sinistro = T2;

    //aggiorno altezza
    aggiornoAltezza(y);
    aggiornoAltezza(x);

    return x;
}
//rotazione sinistra
stazione* rotazioneSinistra(stazione* x){
    stazione* y = x->destro;
    stazione* T2 = y->sinistro;

    //rotazione
    y->sinistro = x;
    x->destro = T2;

    //aggiorno altezza
    aggiornoAltezza(x);
    aggiornoAltezza(y);

    return y;
}

//funzione che inserisce un nodo nell'albero AVL
stazione* inserimentoAVL(stazione* stazione, int distanza, heap_auto* heapAuto){
    //inserimento normale
    if(stazione == NULL){
        return creaNodo(distanza,heapAuto);
    }

    //inserimento sinistro
    if(distanza < stazione->distanza){
        stazione->sinistro = inserimentoAVL(stazione->sinistro,distanza,heapAuto);
    }
    //inserimento destro
    else if(distanza > stazione->distanza){
        stazione->destro = inserimentoAVL(stazione->destro,distanza,heapAuto);
    }
    //inserimento non permesso
    else{
        return stazione;
    }

    //aggiorno altezza
    aggiornoAltezza(stazione);

    //controllo la differenza di altezza
    int differenza = ottieniDifferenza(stazione);

    //controllo se la differenza è maggiore di 1
    //e se il nodo inserito è minore del nodo sinistro
    //allora faccio una rotazione a destra
    if(differenza > 1 ){
        if(distanza < stazione->sinistro->distanza){
            return rotazioneDestra(stazione);
        } else if(distanza > stazione->sinistro->distanza){
            //altrimenti faccio una rotazione a sinistra
            stazione->sinistro = rotazioneSinistra(stazione->sinistro);
            return rotazioneDestra(stazione);
        }
    } else if(differenza < -1){
        if(distanza > stazione->destro->distanza){
            return rotazioneSinistra(stazione);
        } else if(distanza < stazione->destro->distanza){
            stazione->destro = rotazioneDestra(stazione->destro);
            return rotazioneSinistra(stazione);
        }
    }

    return stazione;
}

stazione* trovaMinimo(stazione* nodo){
    stazione* current = nodo;

    while(current->sinistro != NULL){
        current = current->sinistro;
    }

    return current;
}

stazione* cancellaNodo(stazione* root, int distanza){
    if(root == NULL){
        return root;
    }

    if(distanza < root->distanza) {
        root->sinistro = cancellaNodo(root->sinistro,distanza);
    } else if(distanza > root->distanza){
        root->destro = cancellaNodo(root->destro,distanza);
    } else{
        if(root->sinistro == NULL || root->destro == NULL){
            stazione* temp = root->sinistro ? root->sinistro : root->destro;

            if(temp == NULL){
                temp = root;
                root = NULL;
            } else{
                *root = *temp;
            }
            free(temp);
        } else{
            stazione* temp = trovaMinimo(root->destro);

            root->distanza = temp->distanza;
            root->heap_auto = temp->heap_auto;
            root->destro = cancellaNodo(root->destro,temp->distanza);

        }
    }
    aggiornoAltezza(root);

    int differenza = ottieniDifferenza(root);
    if(differenza >1) {
        if(ottieniDifferenza(root->sinistro) >= 0){
            return rotazioneDestra(root);
        } else{
            root->sinistro = rotazioneSinistra(root->sinistro);
            return rotazioneDestra(root);
        }
    } else if (differenza<-1){
        if(ottieniDifferenza(root->destro) <= 0){
            return rotazioneSinistra(root);
        } else{
            root->destro = rotazioneDestra(root->destro);
            return rotazioneSinistra(root);
        }
    }

    return root;
}

void freeAVL(stazione* root){
    if(root == NULL){
        return;
    }
    freeAVL(root->sinistro);
    freeAVL(root->destro);
    free(root->heap_auto->autonomie);
    free(root->heap_auto);
    free(root);
}

//trovare la stazione con la distanza indicata
stazione* trovaStazione(stazione* root, int distanza){
    if(root == NULL){
        return NULL;
    }
    if(root->distanza == distanza){
        return root;
    }
    if(root->distanza > distanza){
        return trovaStazione(root->sinistro,distanza);
    } else {
        return trovaStazione(root->destro,distanza);
    }

}

//funzione che stampa l'intero albero AVL
void printAVL(stazione* root){
    if(root == NULL){
        return;
    }
    printAVL(root->sinistro);

    printf("Distanza: %d\n",root->distanza);
    printf("Numero veicoli: %d\n",root->heap_auto->numero_auto);
    printHeap(root->heap_auto);

    printAVL(root->destro);
}

//funzione che verifica se la stazione esiste
//1 = esiste, 0 = non esiste
int esisteStazione(stazione* root, int distanza) {
    if (root == NULL) {
        return 0;
    }

    if (distanza < root->distanza) {
        return esisteStazione(root->sinistro, distanza);
    } else if (distanza > root->distanza) {
        return esisteStazione(root->destro, distanza);
    } else {
        return 1;  // stazione trovata
    }
}


//todo quando aggiungo una stazione con numero di auto = 0 , problema per aggiungi auto