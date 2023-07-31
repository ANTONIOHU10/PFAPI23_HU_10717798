#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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

struct Node
{
    //fixme
    int step;
    int index;
    int data;
    int autonomia;
    //per albero
    struct Node *parent;
    struct Node *firstChild;
    struct Node *nextSibling;

    //voce che serve per Queue
    struct Node *next;
} ;

struct Queue
{
    struct Node *front; // il primo elemento
    struct Node *back; // ultimo elemento
} ;

typedef struct Queue queue;
typedef struct Node node;
typedef struct Stazione stazione;

stazione* new_node(int x,int numero_auto,int* auto_parcheggiate);
stazione* search(stazione* root, int x);
stazione* insert(stazione* root, int distanza ,int numero_auto,int* auto_parcheggiate);
stazione* find_minimum(stazione* root);
stazione* delete(stazione* root, int x);
void inorder(stazione* root);
void freeStazione(stazione* root);

stazione** createListStation(int size);


int inorderTraversalCrescente(stazione* radice, stazione** lista,int* indice,int partenza,int destinazione);
int inorderTraversalDecrescente(stazione* radice, stazione** lista,int* indice,int partenza,int destinazione);

//-----------------------------------------------------------

void printPathCrescente(stazione** lista,int numeroStazioni,FILE* out);

//fixme parte di lista per il ritorno
node* createNewNode(int data,int autonomia,int step,int index);
void addChild(node* parent,node* newChild);
void printNode(node* root,int depth);
void printPathToRoot(node *node);
void freeTree(node* root);

//------------------------------------------queue
queue* createQueue();
void enqueue(queue* queue,node* newNode);
node* dequeue(queue* queue);
int isEmpty(queue* queue);
void freeQueue(queue* queue);
void printQueue(queue* queue);
void sortQueue(queue* queue);
void BFS();

void printDecrescente(stazione** lista,int numeroStazioni,FILE* out);

void sortQueueVer2(queue* queue);
void printPathToRootToFile(node *node,FILE* out);

//-----------------------------------------------------------
void splitList(node* head,node** firstHalf,node** secondHalf);
node* mergeList(node* a,node* b);
void mergeSort(node** headRef);
void sortQueueVer3(queue* queue);

//-----------------------------maxHeap--------------------------------------
void swapHeap(int* x, int* y);
void heapifyHeap(stazione* station, int n, int i);
void printArrayHeap(stazione* station);
int deleteElementHeap(stazione* stazione, int* heap_size,int key);
void insertHeap(stazione* station, int new_num);

int* copyAutonomie(int* autonomia);

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
                    //fixme:Attenzione l'autonomia va a sostituire le autonomie di un'altra stazione
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
                if(search(radice,distanza) == NULL){
                    //printf("aggiungi-auto  ->non esiste la stazione  %d\n",distanza);
                    fprintf(file_out,"non aggiunta\n");
                } else {
                    //se il parcheggio è già pieno

                    if(search(radice,distanza)->numero_auto < DIMENSIONE_PARCHEGGIO){
                        //printf("aggiungi-auto  -> aggiunta auto %d alla stazione %d\n",autonomia,distanza);

                        //fixme aggiungo l'autonomia alla stazione, e l'autonomia massima dovrebbe essere già aggiornata
                        insertHeap(search(radice,distanza),autonomia);

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
                            search(radice,distanza)->maxAutonomia = search(radice,distanza)->auto_parcheggiate[0];

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

                int numeroStazioniFiltrate=0;
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

        //printf("\n------------informazioni -----------------\n");
        //inorder(radice);
    }

    //inorder(radice);

    freeStazione(radice);
    fclose(file);
    fclose(file_out);

    return 0;
}

//-----------------------------stazione----------------------------
//creazione nodo
//x  = distanza
//y = numero auto
stazione* new_node(int x,int numero_auto,int* auto_parcheggiate){
    stazione* temp = NULL;
    temp = malloc(sizeof(stazione));
    temp->distanza = x;
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
//ricerca nodo
stazione* search(stazione* root, int x){
    if (root == NULL || root->distanza == x)
        return root;
    else if (x > root->distanza)
        return search(root->destro, x);
    else
        return search(root->sinistro, x);
}

//x  = distanza
//y = numero auto
//inserimento nodo
stazione* insert(stazione* root,int distanza,int numero_auto,int* auto_parcheggiate) {
    if (root == NULL)
        return new_node(distanza,numero_auto,auto_parcheggiate);
    else if (distanza > root->distanza)
        root->destro= insert(root->destro,distanza,numero_auto,auto_parcheggiate);
    else
        root -> sinistro = insert(root->sinistro, distanza,numero_auto,auto_parcheggiate);
    return root;
}

stazione* find_minimum(stazione* root) {
    if (root == NULL)
        return NULL;
    else if (root->sinistro != NULL)
        return find_minimum(root->sinistro);
    return root;
}


//todo > devo considerare anche l'albero l'interno di essa
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
            //fixme aggiunta free, per togliere le autonomie, non so se c'è eventuale errore
            //freeAutonomie(root->auto_parcheggiate);
            //fixme: prova
            //root->auto_parcheggiate = NULL;
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
            //fixme aggiunta free, per togliere le autonomie, non so se c'è eventuale errore
            //freeAutonomie(root->auto_parcheggiate);
            //fixme: prova
            //root->auto_parcheggiate = NULL;
            free(root);
            return temp;
        }
            //se il nodo da cancellare ha due figli
        else {
            stazione *temp = find_minimum(root->destro);
            root->distanza= temp->distanza;
            root->numero_auto = temp->numero_auto;
            root->maxAutonomia = temp ->maxAutonomia;
            //fixme per prova
            //freeAutonomie(root->auto_parcheggiate);
            for(int i=0;i<DIMENSIONE_PARCHEGGIO;i++){
                root->auto_parcheggiate[i] = temp->auto_parcheggiate[i];
            }
            //root->auto_parcheggiate = copyAutonomie(temp->auto_parcheggiate);
            root->destro = delete(root->destro, temp->distanza);
        }
    }
    return root;
}

int* copyAutonomie(int* autonomia) {
    // Allocate memory for the new array
    int* new_autonomia = malloc(DIMENSIONE_PARCHEGGIO * sizeof(int));
    if(new_autonomia == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    // Copy elements from the old array to the new one
    for(int i = 0; i < DIMENSIONE_PARCHEGGIO; i++) {
        new_autonomia[i] = autonomia[i];
    }

    return new_autonomia;
}


//stampa in ordine
void inorder(stazione *root){
    if (root != NULL)
    {
        inorder(root->sinistro);
        printf(" %d autonomie (maxAutonomia = %d )-> ha %d autonomie  ->", root->distanza,root->maxAutonomia,root->numero_auto);
        for(int i =0;i<root->numero_auto;i++){
            printf(" %d ",root->auto_parcheggiate[i]);
        }
        printf("\n");
        inorder(root->destro);
    }
}


//   per cancellare i contenuti, devo avere una variabile che riceve il ritorno della funzione di creazione
//   e poi passare quella variabile alla funzione di cancellazione , altrimenti non posso cancellare
void freeStazione(stazione* root) {
    if (root == NULL)
        return;

    freeStazione(root->sinistro);
    freeStazione(root->destro);
    //freeAutonomie(root->auto_parcheggiate);
    free(root);
}
//-----------------------------stazione----------------------------

void decrease_number_auto(stazione* stazione){
    if(stazione != NULL) {
        stazione->numero_auto--;
    }
}
void increase_number_auto(stazione* stazione){
    if(stazione != NULL) {
        stazione->numero_auto++;
    }
}


//------------------------------lista di adiacenza--------------------------------------------

// 中序遍历二叉树，将节点按从小到大的顺序放入列表
int inorderTraversalCrescente(stazione*  radice,  stazione** lista, int* indice,int partenza,int destinazione) {
    if (radice == NULL)
        return 0;
    int count=0;
    count += inorderTraversalCrescente(radice->sinistro, lista, indice,partenza,destinazione);
    if(radice->distanza >= partenza && radice->distanza <= destinazione){
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

    int count=0;
    count += inorderTraversalDecrescente(radice->destro, lista, indice,partenza,destinazione);
    if(radice->distanza <= partenza && radice->distanza >= destinazione){
        lista[*indice] = radice;
        (*indice)++;
        count++;
    }
    count += inorderTraversalDecrescente(radice->sinistro, lista, indice,partenza,destinazione);

    return count;
}


stazione**  createListStation(int size){
    stazione** lista = (struct Stazione**)malloc(size * sizeof(stazione*));
    return lista;
}
///////////////////////////////////////////////////////////////////////////////

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
    //printf("---percorso: ");

    //partenza
    int prev = vettore[numeroStazioni-1];


    for(int i=numeroStazioni-1; i>0;i--){
        //percorso in mezzo
        //printf("\n---%d ",vettore[i]);
        //printf("%d  ",prev);
        if(lista[i]->distanza == prev){
            //printf("-----> trovo una stazione %d ",lista[i]->distanza);
            path[i] = lista[i]->distanza;
            prev= vettore[i];
        }
    }

    //printf("\n");
    //partenza
    //printf("%d ",lista[0]->distanza);
    fprintf(out,"%d ",lista[0]->distanza);

    for(int n=0;n<numeroStazioni;n++){
        if(path[n] != INT_MAX){
            //printf("%d ",path[n]);
            fprintf(out,"%d ",path[n]);
        }
    }

    //destinazione
    //printf("%d\n",lista[numeroStazioni-1]->distanza);
    fprintf(out,"%d\n",lista[numeroStazioni-1]->distanza);
    //libero memoria
    free(vettore);
    free(path);
    //return;
}

//////////////////////////////////////////////////////////////////////


//fixme------------------------------------------------------------------------------
//------------------------------------------------TREE-----------------------------------------------
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
        node *sibling = parent->firstChild;
        while (sibling->nextSibling != NULL)
        {
            sibling = sibling->nextSibling;
        }
        //il nuovo nodo diventa ultimo figlio
        sibling->nextSibling = newChild;
    }
}

//BFS print
void printNode(node *root, int depth)
{
    for(int i = 0; i < depth; i++){
        printf("  ");
    }

    printf("%d\n", root->data);

    //se ha il figlio
    if(root->firstChild != NULL)
        printNode(root->firstChild, depth + 1);

    //se ha dei nodi vicini
    if(root->nextSibling != NULL)
        printNode(root->nextSibling, depth);
}

// print path from a node to root
void printPathToRoot(node *node)
{
    if(node->parent == NULL) // If root node
    {
        printf("%d", node->data);
    }
    else
    {
        printPathToRoot(node->parent);
        printf(" -> %d", node->data);
    }
}

void freeTree(node *root)
{
    //se l'albero è vuoto
    if(root== NULL)
    {
        return;
    }

    node* currentChild = root->firstChild;
    while(currentChild != NULL) // stampa tutti i figli
    {
        node *nextChild = currentChild->nextSibling;
        freeTree(currentChild);
        currentChild = nextChild;
    }

    free(root); // 最后释放节点本身
}


//--------------------------------------------- QUEUE--------------------------------------------------
// creazione di una queue
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

// aggiungo il nodo come ultimo
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

// se la queue è vuota
int isEmpty(queue *queue)
{
    return queue->front == NULL;
}

// liberare la queue
void freeQueue(queue *queue)
{
    while (!isEmpty(queue))
    {
        node *node = dequeue(queue);
        free(node);
    }
    free(queue);
}

void printQueue(queue* queue)
{
    node* temp = queue->front;

    // 遍历整个队列并打印每个节点的数据
    while (temp != NULL)
    {
        printf("%d (step = %d) ", temp->data,temp->step);
        temp = temp->next;
    }

    printf("\n");
}

void sortQueue(queue* queue)
{
    // 边界情况：空队列或只有一个元素的队列已经是有序的
    if (queue->front == NULL || queue->front->next == NULL)
    {
        return;
    }

    node *sorted = NULL;  // 已排序部分的头结点
    node *current = queue->front; // 当前节点

    while (current != NULL)
    {
        node *nextNode = current->next;  // 记录下一个节点

        // 将 current 节点插入到 sorted 链表的正确位置
        if (sorted == NULL || current->data < sorted->data)
        {
            // current 节点应该被插入到 sorted 链表的前面
            current->next = sorted;
            sorted = current;
        }
        else
        {
            // current 节点应该被插入到 sorted 链表的中间位置
            node *sortedCurr = sorted;
            while (sortedCurr->next != NULL && sortedCurr->next->data < current->data)
            {
                sortedCurr = sortedCurr->next;
            }
            current->next = sortedCurr->next;
            sortedCurr->next = current;
        }

        // 继续处理下一个节点
        current = nextNode;
    }

    // 更新队列的 front 和 back 指针
    queue->front = sorted;
    node *tempNode = sorted;
    while (tempNode->next != NULL)
    {
        tempNode = tempNode->next;
    }
    queue->back = tempNode;
}

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

//todo prova
void BFS (){
    printf("\n\n----------------------------------\n");

    queue* queue = createQueue();
    node* node1= createNewNode(1,0,1,0);
    node* node2= createNewNode(1,0,0,0);
    node* node3= createNewNode(2,0,1,0);
    node* node4= createNewNode(2,0,2,0);
    node* node5= createNewNode(2,0,0,0);
    enqueue(queue,node1);
    enqueue(queue,node2);
    enqueue(queue,node3);
    enqueue(queue,node4);
    enqueue(queue,node5);

    printQueue(queue);
    printf("----after sort---------\n");

    sortQueueVer2(queue);

    printQueue(queue);
    freeQueue(queue);

}

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

    //printPathToRoot(nodeFinal);
    printPathToRootToFile(nodeFinal,out);
    fprintf(out,"\n");
    //printf("\n");
    free(visitato);
    freeTree(root);
    free(queue);
    //se usavo freeQueue(queue) mi dava errore, perché i nodi sono stati già deallocati con freeTree(root)
};


void sortQueueVer2(queue* queue)
{
    // 边界情况：空队列或只有一个元素的队列已经是有序的
    if (queue->front == NULL || queue->front->next == NULL)
    {
        return;
    }

    node *sorted = NULL;  // 已排序部分的头结点
    node *current = queue->front; // 当前节点

    while (current != NULL)
    {
        node *nextNode = current->next;  // 记录下一个节点

        // 将 current 节点插入到 sorted 链表的正确位置
        if (sorted == NULL || current->step < sorted->step || (current->step == sorted->step && current->data < sorted->data))
        {
            // current 节点应该被插入到 sorted 链表的前面
            current->next = sorted;
            sorted = current;
        }
        else
        {
            // current 节点应该被插入到 sorted 链表的中间位置
            node *sortedCurr = sorted;
            while (sortedCurr->next != NULL && (current->step > sortedCurr->next->step || (current->step == sortedCurr->next->step && current->data > sortedCurr->next->data)))
            {
                sortedCurr = sortedCurr->next;
            }
            current->next = sortedCurr->next;
            sortedCurr->next = current;
        }

        // 继续处理下一个节点
        current = nextNode;
    }

    // 更新队列的 front 和 back 指针
    queue->front = sorted;
    node *tempNode = sorted;
    while (tempNode->next != NULL)
    {
        tempNode = tempNode->next;
    }
    queue->back = tempNode;
}

//---------------------------------------------
void split(node* head, node** front, node** back)
{
    node* fast;
    node* slow;
    slow = head;
    fast = head->next;

    // Advance 'fast' two nodes, and advance 'slow' one node
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    // 'slow' is before the midpoint in the list, so split it in two at that point.
    *front = head;
    *back = slow->next;
    slow->next = NULL;
}

node* merge(node* a, node* b)
{
    node* result = NULL;

    // Base cases
    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    // Recursively sort
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

void mergeSort(node** head)
{
    node* h = *head;
    node* a;
    node* b;

    // Base case -- length 0 or 1
    if ((h == NULL) || (h->next == NULL)) {
        return;
    }

    // Split head into 'a' and 'b' sublists
    split(h, &a, &b);

    // Recursively sort sublists
    mergeSort(&a);
    mergeSort(&b);

    // Answer = merge the two sorted lists together
    *head = merge(a, b);
}

void sortQueueVer3(queue* queue)
{
    // Sort the queue by 'step' and then by 'data' using Merge Sort
    mergeSort(&(queue->front));

    // Update the 'back' pointer of the queue
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
void heapifyHeap(stazione* station, int n, int i) {
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && station->auto_parcheggiate[left] > station->auto_parcheggiate[largest])
        largest = left;

    if (right < n && station->auto_parcheggiate[right] > station->auto_parcheggiate[largest])
        largest = right;

    if (largest != i) {
        swapHeap(&station->auto_parcheggiate[i], &station->auto_parcheggiate[largest]);
        heapifyHeap(station, n, largest);
    }
}
//per inserire un elemento
void insertHeap(stazione* station, int new_num) {
    if (station->numero_auto == DIMENSIONE_PARCHEGGIO) {
        printf("\nOverflow: Could not insert Key\n");
        return;
    }

    // Insert the new number at the end
    station->numero_auto++;
    station->auto_parcheggiate[station->numero_auto - 1] = new_num;

    // Heapify the tree from bottom to top
    for (int i = station->numero_auto / 2 - 1; i >= 0; i--)
        heapifyHeap(station, station->numero_auto, i);


    station->maxAutonomia = station->auto_parcheggiate[0];
}

//per stampare il max heap
void printArrayHeap(stazione* station) {
    for (int i = 0; i < station->numero_auto; ++i)
        printf("%d ", station->auto_parcheggiate[i]);
    printf("\n");
}

//per cancellare un elemento determinato nel max heap
int deleteElementHeap(stazione* stazione, int *heap_size, int key) {
    int i;
    for(i=0; i<*heap_size; i++) {
        if(key == stazione->auto_parcheggiate[i])
            break;
    }

    if(i < *heap_size) {
        swapHeap(&stazione->auto_parcheggiate[i], &stazione->auto_parcheggiate[*heap_size - 1]);
        (*heap_size)--;
        heapifyHeap(stazione, *heap_size, 0);
        return key;
    }

    return -1;
}