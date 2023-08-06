#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
 * BST + maxHeap struttura dati
 *
 * array per salvare elementi per il percorso
 *
 * Queue + albero N-ario per il percorso inverso
 * */


//definisco la dimensione massima del parcheggio
#define DIMENSIONE_PARCHEGGIO 512
//procedimento su Github

//struttura per le stazioni, BST
struct Stazione{
    int distanza; //key
    int numero_auto;
    int maxAutonomia;

    //utilizzo un maxHeap per le autonomie
    int auto_parcheggiate[DIMENSIONE_PARCHEGGIO];
    struct Stazione* sinistro;
    struct Stazione* destro;
};

//struttura per il calcolo percorso
//il nodo è comune sia per Albero N-ario che per Queue
struct Node
{
    int step; //profondità nell'albero N-ario
    int index;//posizione nella lista
    int data;//key , cioè la distanza
    int autonomia;
    //per albero
    struct Node *parent;
    struct Node *firstChild; //realizzazione in modo intelligente
    struct Node *nextSibling;

    //voce che serve per Queue
    struct Node *next;
};

//per il calcolo percorso
struct Queue
{
    struct Node *front; // il primo elemento
    struct Node *back; // ultimo elemento
} ;

typedef struct Queue queue;
typedef struct Node node;
typedef struct Stazione stazione;

//---------- struttura dati per stazioni--------------------------------------------
stazione* new_node(int distanza,int numero_auto,int* auto_parcheggiate);
stazione* search(stazione* root, int distanza);
stazione* insert(stazione* root, int distanza ,int numero_auto,int* auto_parcheggiate);
stazione* find_minimum(stazione* root);
stazione* delete(stazione* root, int x);
void freeStazione(stazione* root);

stazione** createListStation(int size);


int inorderTraversalCrescente(stazione* radice, stazione** lista,int* indice,int partenza,int destinazione);
int inorderTraversalDecrescente(stazione* radice, stazione** lista,int* indice,int partenza,int destinazione);

//-----------------------------------------------------------

void printPathCrescente(stazione** lista,int numeroStazioni,FILE* out);

node* createNewNode(int data,int autonomia,int step,int index);
void addChild(node* parent,node* newChild);
void freeTree(node* root);

//------------------------------------------queue
queue* createQueue();
void enqueue(queue* queue,node* newNode);
node* dequeue(queue* queue);
void printDecrescente(stazione** lista,int numeroStazioni,FILE* out);
void printPathToRootToFile(node *node,FILE* out);

//-----------------------------------------------------------
void mergeSort(node** headRef);
void sortQueueVer3(queue* queue);

//-----------------------------maxHeap--------------------------------------
void swapHeap(int* x, int* y);
void heapifyHeap(stazione* station, int n, int i);
int deleteElementHeap(stazione* stazione, int* dimensione,int key);
void insertHeap(stazione* stazione, int new_num);

int main() {

    stazione* radice = NULL;

    //file da leggere
    FILE *file = stdin;
    //printf("---------------------------------------------------------\n");

    //file da scrivere
    FILE *file_out = stdout;

    //dichiarazione per lettura
    char operazione[20];
    int distanza=0;
    int numero_auto=0;
    int autonomia=0;

    int autonomie_temp[512];

    //numero di stazioni
    int numero_stazioni = 0;

    //leggere una riga di comando
    while (fscanf(file,"%s",operazione) != EOF){

        //OK funziona e si compila con il verificatore
        if(strcmp(operazione,"aggiungi-stazione") == 0) {
            if(fscanf(file,"%d %d",&distanza, &numero_auto) != EOF){
                //printf("--->aggiungi stazione %d con %d auto\n",distanza,numero_auto);

                //se il numero di auto è maggiore di 512 non aggiungo la stazione
                if(numero_auto>512){
                    //printf("numero auto maggiore di 512\n");
                    fprintf(file_out,"non aggiunta\n");
                } else {
                    //verifico se la stazione esiste già
                    if(search(radice,distanza) != NULL){
                        //printf("stazione già presente\n");
                        fprintf(file_out,"non aggiunta\n");
                    } else {

                        //leggo le autonomie
                        for (int i = 0; i < numero_auto; i++){
                            if(fscanf(file,"%d",&autonomie_temp[i]) != EOF){
                                //leggo una autonomia e salvo nella lista, eventuale miglioramento da fare
                            }
                        }

                        //ottengo il massimo valore dalla max heap
                        radice = insert(radice,distanza,numero_auto,autonomie_temp);

                        numero_stazioni++;
                        fprintf(file_out,"aggiunta\n");

                    }
                }
            }
        }

            //OK non c'è problema con valgrind funziona il verificatore
        else if(strcmp(operazione,"demolisci-stazione") == 0) {
            if(fscanf(file,"%d",&distanza) != EOF){

                //printf("--->demolisci stazione %d\n",distanza);
                if(search(radice,distanza) == NULL){
                    //non esiste la stazione

                    fprintf(file_out,"non demolita\n");
                } else {
                    radice = delete(radice,distanza);

                    //aggiorno il numero di stazioni
                    numero_stazioni--;

                    fprintf(file_out,"demolita\n");

                }
            }
        }

            //OK non c'è problema con valgrind
        else if(strcmp(operazione,"aggiungi-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza, &autonomia) != EOF){

                //------------------------se esiste la stazione-------------------------------
                stazione* temp = search(radice,distanza);
                if(temp == NULL){
                    //printf("aggiungi-auto  ->non esiste la stazione  %d\n",distanza);
                    fprintf(file_out,"non aggiunta\n");
                } else {
                    //se il parcheggio è già pieno

                    if(temp->numero_auto < DIMENSIONE_PARCHEGGIO){
                        //printf("aggiungi-auto  -> aggiunta auto %d alla stazione %d\n",autonomia,distanza);

                        //fixme aggiungo l'autonomia alla stazione, e l'autonomia massima dovrebbe essere già aggiornata
                        insertHeap(temp,autonomia);

                        fprintf(file_out,"aggiunta\n");

                    } else {
                        //printf("aggiungi-auto  -> parcheggio della stazione %d pieno\n",distanza);

                        fprintf(file_out,"non aggiunta\n");
                    }
                }
            }
        }

        else if(strcmp(operazione, "rottama-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza,&autonomia) != EOF){

                //metto la stazione trovata dentro stazione_temp per facilitare la lettura
                stazione* stazione_temp = search(radice,distanza);
                //------------------------se esiste la stazione-------------------------------
                if(stazione_temp == NULL){

                    fprintf(file_out,"non rottamata\n");
                } else {
                    //se ha almeno un auto
                    if(stazione_temp->numero_auto > 0){
                        //cancello l'autonomia dalla MaxHeap della stazione richiesta
                        if(deleteElementHeap(stazione_temp,&stazione_temp->numero_auto,autonomia) == -1){

                            fprintf(file_out,"non rottamata\n");
                        } else{
                            stazione_temp->maxAutonomia = stazione_temp->auto_parcheggiate[0];

                            if(stazione_temp->numero_auto == 0){
                                stazione_temp->maxAutonomia = 0;
                            }
                            fprintf(file_out,"rottamata\n");
                        }

                    } else {
                        fprintf(file_out,"non rottamata\n");
                    }
                }

            }
        }

        else if(strcmp(operazione,"pianifica-percorso") == 0) {
            int distanza_destinazione;
            if(fscanf(file,"%d %d",&distanza,&distanza_destinazione) != EOF){

                int numeroStazioniFiltrate;
                int indice=0;
                stazione** lista = createListStation(numero_stazioni);

                //se la distanza di partenza è minore della distanza di destinazione
                if(distanza < distanza_destinazione){
                    numeroStazioniFiltrate = inorderTraversalCrescente(radice,lista,&indice,distanza,distanza_destinazione);

                    printPathCrescente(lista,numeroStazioniFiltrate,file_out);
                }

                else if (distanza > distanza_destinazione){
                    numeroStazioniFiltrate = inorderTraversalDecrescente(radice,lista,&indice,distanza,distanza_destinazione);

                    printDecrescente(lista,numeroStazioniFiltrate,file_out);

                }
                else if(distanza == distanza_destinazione){
                    fprintf(file_out,"distanza\n");
                }

                free(lista);

            }
        }

    }
    freeStazione(radice);
    fclose(file);
    fclose(file_out);

    return 0;
}

//-----------------------------stazione----------------------------
//creazione nodo
//x  = distanza
//y = numero auto
stazione* new_node(int distanza,int numero_auto,int* auto_parcheggiate){
    stazione* temp = NULL;
    temp = malloc(sizeof(stazione));
    temp->distanza = distanza;
    temp->numero_auto = 0;
    memset(temp->auto_parcheggiate,0,DIMENSIONE_PARCHEGGIO*sizeof(int));
    for(int i=0;i<numero_auto;i++){
        insertHeap(temp,auto_parcheggiate[i]);
    }
    //è obbligatorio inizializzare la variabile maxAutonomia
    temp->maxAutonomia = 0;
    //printf("----------aggiungi stazione  -----------\n");
    for(int i=0;i<numero_auto;i++){
        //printf("auto_parcheggiate[%d] = %d\n",i,temp->auto_parcheggiate[i]);
    }
    temp->maxAutonomia = temp->auto_parcheggiate[0];
    //printf("assegnazione maxAutonomia adesso vale: %d\n",temp->maxAutonomia);
    temp->sinistro = NULL;
    temp->destro = NULL;

    return temp;
}
//ricerca nodo nel BST ricorsiva
//funzione per ispirazione di slide di lezione
stazione* search(stazione* root, int distanza){
    if (root == NULL || root->distanza == distanza)
        return root;
    else if (distanza > root->distanza)
        return search(root->destro, distanza);
    else
        return search(root->sinistro, distanza);
}

//x  = distanza
//y = numero auto
//inserimento nodo
//funzione per ispirazione di slide di lezione
stazione* insert(stazione* root,int distanza,int numero_auto,int* auto_parcheggiate) {
    stazione* newNode = new_node(distanza, numero_auto, auto_parcheggiate);
    if (root == NULL)
        return newNode;

    stazione* parent = NULL;
    stazione* current = root;

    while(current != NULL) {
        parent = current;
        if (distanza > current->distanza)
            current = current->destro;
        else
            current = current->sinistro;
    }

    if (distanza > parent->distanza)
        parent->destro = newNode;
    else
        parent->sinistro = newNode;

    return root;
}

//funzione ausiliare per trovare il minimo partendo dal nodo corrente
//funzione per ispirazione di slide di lezione
stazione* find_minimum(stazione* root) {
    if (root == NULL)
        return NULL;

    while(root->sinistro != NULL)
        root = root->sinistro;

    return root;
}


//cancello un nodo dall albero
stazione* delete(stazione* root, int x) {

    if (root == NULL)
        return NULL;
    //se il nodo da cancellare è maggiore del nodo corrente
    if (x > root->distanza)
        root->destro = delete(root->destro, x);
        //se il nodo da cancellare è minore del nodo corrente
    else if (x < root->distanza)
        root->sinistro = delete(root->sinistro, x);

        //se il nodo da cancellare è uguale al nodo corrente
    else {
        //se il nodo da cancellare è una foglia, non ha figli
        if (root->sinistro == NULL && root->destro == NULL){
            free(root);
            return NULL;
        }
            //se il nodo da cancellare ha un solo figlio
        else if (root->sinistro == NULL || root->destro == NULL){
            stazione* temp;
            if (root->sinistro == NULL)
                //figlio destro è il nuovo nodo temporaneo
                temp = root->destro;
            else
                //figlio destro è il nuovo nodo temporaneo
                temp = root->sinistro;
            free(root);
            return temp;
        }
            //se il nodo da cancellare ha due figli
        else {
            stazione *temp = find_minimum(root->destro);
            root->distanza= temp->distanza;
            root->numero_auto = temp->numero_auto;
            root->maxAutonomia = temp ->maxAutonomia;
            for(int i=0;i<DIMENSIONE_PARCHEGGIO;i++){
                root->auto_parcheggiate[i] = temp->auto_parcheggiate[i];
            }
            root->destro = delete(root->destro, temp->distanza);
        }
    }
    return root;
}

//   per cancellare i contenuti, devo avere una variabile che riceve il ritorno della funzione di creazione
//   e poi passare quella variabile alla funzione di cancellazione, altrimenti non posso cancellare
void freeStazione(stazione* root) {
    if (root == NULL)
        return;

    freeStazione(root->sinistro);
    freeStazione(root->destro);
    //freeAutonomie(root->auto_parcheggiate);
    free(root);
}
//------------------------------lista di adiacenza--------------------------------------------

// metto le stazioni in una lista
int inorderTraversalCrescente(stazione*  radice,  stazione** lista, int* indice,int partenza,int destinazione) {
    if (radice == NULL)
        return 0;

    //numero di stazioni tra partenza e destinazione incluse
    int count=0;
    count += inorderTraversalCrescente(radice->sinistro, lista, indice,partenza,destinazione);

    //solo le stazioni tra partenza e destinazione incluse
    if(radice->distanza >= partenza && radice->distanza <= destinazione){
        //metto nella lista
        lista[*indice] = radice;
        (*indice)++;
        count++;
    }
    count += inorderTraversalCrescente(radice->destro, lista, indice,partenza,destinazione);

    return count;
}
int inorderTraversalDecrescente(stazione*  radice,  stazione** lista, int* indice,int partenza,int destinazione) {
    if (radice == NULL)
        return 0;

    //numero di stazioni tra partenza e destinazione incluse
    int count=0;
    count += inorderTraversalDecrescente(radice->destro, lista, indice,partenza,destinazione);
    //solo le stazioni tra partenza e destinazione incluse
    if(radice->distanza <= partenza && radice->distanza >= destinazione){
        //metto nella lista
        lista[*indice] = radice;
        (*indice)++;
        count++;
    }
    count += inorderTraversalDecrescente(radice->sinistro, lista, indice,partenza,destinazione);

    return count;
}

//allocare memoria per la lista e restituisce la lista
stazione**  createListStation(int size){
    stazione** lista = (struct Stazione**)malloc(size * sizeof(stazione*));
    return lista;
}
///////////////////////////////////////////////////////////////////////////////

//funzione per il calcolo percorso CRESCENTE
void printPathCrescente(stazione** lista,int numeroStazioni,FILE* out) {

    //inizializzare un vettore dinamico
    int* vettore = (int*)malloc(numeroStazioni * sizeof(int));

    //inizializzare un vettore dinamico per il path
    int* path = (int*)malloc(numeroStazioni * sizeof(int));

    //inizializzare un vettore dinamico con il valore di default
    for(int i=0;i<numeroStazioni;i++){
        vettore[i] = INT_MAX;
        path[i] = INT_MAX;
    }

    //creazione vettore per le informazioni di percorso
    for(int i=0;i<=numeroStazioni-1;i++){

        //verifico tutte le stazioni raggiungibili dalla stazione i
        if(i!=numeroStazioni-1){
            for(int j=i+1; j<=numeroStazioni-1;j++){
                //se è raggiungibile
                if(lista[i]->distanza + lista[i]->maxAutonomia >= lista[j]->distanza){
                    //aggiorno
                    if(lista[i]->distanza < vettore[j]){
                        vettore[j] = lista[i]->distanza;
                    }
                } else {
                    //finisco di visitare le stazioni raggiungibili
                    break;
                }
            }
        }

    }
    //verifico se la stazione di destinazione è raggiungibile
    for(int i=1;i<numeroStazioni;i++){
        if(vettore[i] == INT_MAX){
            //printf("---percorso: IMPOSSIBILE\n\n");
            fprintf(out,"nessun percorso\n");
            free(vettore);
            free(path);
            return;
        }
    }
    //partenza
    int prev = vettore[numeroStazioni-1];


    for(int i=numeroStazioni-1; i>0;i--){
        //percorso in mezzo
        if(lista[i]->distanza == prev){
            //printf("-----> trovo una stazione %d ",lista[i]->distanza);
            path[i] = lista[i]->distanza;
            prev= vettore[i];
        }
    }

    //partenza
    fprintf(out,"%d ",lista[0]->distanza);

    for(int n=0;n<numeroStazioni;n++){
        if(path[n] != INT_MAX){
            //printf("%d ",path[n]);
            fprintf(out,"%d ",path[n]);
        }
    }

    //destinazione
    fprintf(out,"%d\n",lista[numeroStazioni-1]->distanza);
    //libero memoria
    free(vettore);
    free(path);
}

//---------------------------parte albero N ario------------------------------------------------------
//creazione di un nodo
node *createNewNode(int data,int autonomia,int step,int index)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->data = data;
    newNode->step = step;
    newNode->index = index;
    newNode->autonomia = autonomia;
    newNode->parent = NULL;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    newNode->next = NULL;
    return newNode;
}

//aggiungere un nodo come figlio al padre
void addChild(node *parent, node *newChild)
{
    //assegna indirizzo del padre al figlio
    newChild->parent = parent;

    //se il padre non aveva figlio -> allora questo nodo diventa il suo primo figlio
    if (parent->firstChild == NULL)
    {
        parent->firstChild = newChild;
    }
        //se il padre aveva altri figli -> allora questo nodo andrà in fondo
    else
    {
        //come realizzazione di lista
        node *sibling = parent->firstChild;
        while (sibling->nextSibling != NULL)
        {
            sibling = sibling->nextSibling;
        }

        //il nuovo nodo diventa ultimo figlio
        sibling->nextSibling = newChild;
    }
}



void freeTree(node *root)
{

    //se l'albero è vuoto
    if(root== NULL)
    {
        return;
    }

    //nodo temp
    node * currentChild = NULL;

    currentChild = root->firstChild;
    //come lista
    while(currentChild != NULL)
    {
        node *nextChild = currentChild->nextSibling;

        //libero memoria per ogni figlio presente nella lista
        freeTree(currentChild);
        currentChild = nextChild;
    }

    free(root); // libera il nodo corrente
}


//--------------------------------------------- QUEUE--------------------------------------------------
// creazione di una queue,inizializzare
queue *createQueue()
{
    queue* newQueue = (queue *)malloc(sizeof(queue));
    newQueue->front = NULL;
    newQueue->back = NULL;
    return newQueue;
}

// aggiungere un nodo alla queue
void enqueue(queue *queue, node *newNode)
{
    // se la queue è inizialmente vuota, metto il nuovo nodo come l'unico nodo
    if (queue->back == NULL)
    {
        queue->front = newNode;
        queue->back = newNode;
    }

// aggiungo il nodo come ultimo, inserisco in fondo
    else
    {
        queue->back->next = newNode;
        queue->back = newNode;
    }
}

// cancellare un nodo dalla queue, e ottenere il nodo
node *dequeue(queue *queue)
{
    //se non ci sono nodi nella queue
    if (queue->front == NULL)
    {
        return NULL;
    }

    //altrimenti togliere il primo nodo, e il secondo nodo diventa il primo nodo
    node *node = queue->front;
    queue->front = node->next;

    if (queue->front == NULL)
    {
        queue->back = NULL;
    }

    node->next = NULL;
    return node;
}

//attenzione qui uso il nodo sia per la queue che albero
//stampa dal basso verso alto il albero N ario creato per il percorso
void printPathToRootToFile(node *node,FILE* out)
{
    if(node->parent == NULL) // If root node
    {
        fprintf(out,"%d", node->data);
    }
    else
    {
        printPathToRootToFile(node->parent,out);
        fprintf(out," %d", node->data);
    }
}

//----funzione per il percorso DECRESCENTE---------
void printDecrescente(stazione** lista,int numeroStazioni,FILE* out){

    //creare una queue
    queue* queue = createQueue();

    //creare un nodo root
    node* root = createNewNode(lista[0]->distanza, lista[0]->maxAutonomia,0,0);

    //aggiungere il nodo root alla queue
    enqueue(queue, root);

    //array per segnare che la stazione è stata visitata
    int* visitato = (int*)malloc(sizeof(int)*numeroStazioni);

    //inizializzazione
    for(int i = 0; i < numeroStazioni; i++){
        visitato[i] = 0;
    }

    node* nodeFinal = NULL;

    //quando la queue non è vuota
    while(queue->front != NULL){
        node* nodeTemp = dequeue(queue);

        if(nodeTemp->data == lista[numeroStazioni-1]->distanza){
            nodeFinal = nodeTemp;
            break;
        }
        //ottengo il valore dell'indice del nodo corrente
        int indexNodeTemp = nodeTemp->index;

        for(int i = indexNodeTemp+1; i<numeroStazioni; i++){

            //se la stazione è la stazione di partenza, fine scorrimento
            if(nodeTemp->data - nodeTemp->autonomia > lista[i]->distanza){
                //printf("stazione non raggiungibile\n");
                break;
            }
            else {

                //se la stazione è raggiungibile dalla stazione node ottenuto dalla queue
                if(nodeTemp->data - nodeTemp->autonomia <= lista[i]->distanza && visitato[i] == 0){

                    //il nuovo nodo
                    node* nodeAdd = createNewNode(lista[i]->distanza, lista[i]->maxAutonomia,nodeTemp->step+1,i);

                    //aggiungere il nodo alla queue e all'albero
                    enqueue(queue, nodeAdd);

                    addChild(nodeTemp, nodeAdd);
                    //printf("stazione aggiunta\n");
                    visitato[i] = 1;
                    //printf("aggiunto un nodo\n");
                }
            }

        }
        //todo+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        //sortQueueVer2(queue);
        sortQueueVer3(queue);
        //printQueue(queue);
        //printf("\n\n");
    }

    if(nodeFinal == NULL){
        //printf("NESSUN PERCORSO\n");
        fprintf(out,"nessun percorso\n");
        free(visitato);
        freeTree(root);
        free(queue);
        return;
    }

    printPathToRootToFile(nodeFinal,out);
    fprintf(out,"\n");
    //printf("\n");
    free(visitato);
    freeTree(root);
    free(queue);
    //se usavo freeQueue(queue) mi dava errore, perché i nodi sono stati già deallocati con freeTree(root)
};

//---------------------------------------------
void split(node* head, node** front, node** back)
{
    if (head == NULL || head->next == NULL) {
        *front = head;
        *back = NULL;
        return;
    }

    node* a;
    node* b;
    b = head;
    a = head->next;

    while (a != NULL) {
        a = a->next;
        if (a != NULL) {
            b = b->next;
            a = a->next;
        }
    }

    *front = head;
    *back = b->next;
    b->next = NULL;
}
//funzione merge sort
node* merge(node* a, node* b)
{
    node* result = NULL;

    // gestione eccezione
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    // ricorsivo, confronto i valori dei nodi
    // priorità 1 = il nodo che ha il numero di step minore
    // priorità 2 = a parità di step, il nodo che ha il valore di data minore
    if (a->step < b->step || (a->step == b->step && a->data < b->data)) {
        result = a;
        result->next = merge(a->next, b);
    }
    else {
        result = b;
        result->next = merge(a, b->next);
    }
    return result;
}

//funzione presa da slides
void mergeSort(node** head)
{
    node* h = *head;
    node* a;
    node* b;

    // caso di base verifico se la -- lunghezza è 0 o 1,cioè la fine
    if ((h == NULL) || (h->next == NULL)) {
        return;
    }

    // metto in due pezzi
    split(h, &a, &b);

    // sort ricorsivo per le liste a e b, passo la testa dei segmenti da fare sort
    mergeSort(&a);
    mergeSort(&b);

    // merge delle due liste ordinate, passo la testa dei segmenti da fare merge
    *head = merge(a, b);
}

void sortQueueVer3(queue* queue)
{
    // sort della queue
    mergeSort(&(queue->front));

    // aggiornare il puntatore al nodo finale
    node* tempNode = queue->front;
    while (tempNode!= NULL && tempNode->next != NULL)
    {
        tempNode = tempNode->next;
    }
    queue->back = tempNode;
}
//----------------------------max heap---------------------------------

//scambiare due elementi nel max heap
void swapHeap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

//per mantenere la proprietà di maxHeap
//n = numero elementi
//i = indice del nodo
void heapifyHeap(stazione* station, int n, int i) {
    int current = i; // nodo corrente
    int sinistro = 2 * i + 1; // nodo sinistro
    int destro = 2 * i + 2; // nodo destro

    if (sinistro < n && station->auto_parcheggiate[sinistro] > station->auto_parcheggiate[current])
        current = sinistro;

    if (destro < n && station->auto_parcheggiate[destro] > station->auto_parcheggiate[current])
        current = destro;

    if (current != i) {
        swapHeap(&station->auto_parcheggiate[i], &station->auto_parcheggiate[current]);
        heapifyHeap(station, n, current);
    }
}


//per inserire un elemento
void insertHeap(stazione* stazione, int new_num) {
    //stazione ha raggiunto la capacità massima di autonomia
    if (stazione->numero_auto == DIMENSIONE_PARCHEGGIO) {
        return;
    }

    // inserire un nodo alla fine
    stazione->numero_auto++;
    stazione->auto_parcheggiate[stazione->numero_auto - 1] = new_num;

    // Heapify dal basso
    for (int i = stazione->numero_auto / 2 - 1; i >= 0; i--)
        heapifyHeap(stazione, stazione->numero_auto, i);

    stazione->maxAutonomia = stazione->auto_parcheggiate[0];
}

//per cancellare un elemento determinato nel max heap
int deleteElementHeap(stazione* stazione, int *dimensione, int key) {
    int i;
    //trovare prima il elemento da cancellare, l'indice
    for(i=0; i<*dimensione; i++) {
        if(key == stazione->auto_parcheggiate[i])
            break;
    }

    //se ha trovato l'elemento entro la lunghezza
    if(i < *dimensione) {

        //cambia posizione il primo e ultimo
        swapHeap(&stazione->auto_parcheggiate[i], &stazione->auto_parcheggiate[*dimensione - 1]);

        (*dimensione)--;
        heapifyHeap(stazione, *dimensione, 0);
        return key;
    }

    //restituisce -1 se non trova l'elemento
    return -1;
}