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
    struct Autonomie* auto_parcheggiate;
    struct Stazione* sinistro;
    struct Stazione* destro;

};
//struttura per le autonomie, BST
struct Autonomie{
    int autonomia;
    struct Autonomie* sinistro;
    struct Autonomie* destro;
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
typedef struct Autonomie autonomie;
typedef struct Stazione stazione;

stazione* new_node(int x, int y, autonomie* auto_parcheggiate, int maxAutonomia);
stazione* search(stazione* root, int x);
stazione* insert(stazione* root, int x, int y, autonomie* auto_parcheggiate, int maxAutonomia);
stazione* find_minimum(stazione* root);
stazione* delete(stazione* root, int x);
void inorder(stazione* root);
void freeStazione(stazione* root);

autonomie* new_node_autonomia(int x);
autonomie* search_autonomie(autonomie* root, int x);
autonomie* insert_autonomie(autonomie* root, int x);
autonomie* find_minimum_autonomie(autonomie* root);
autonomie* delete_autonomie(autonomie* root, int x);
void inorder_autonomie(autonomie* root);
void freeAutonomie(autonomie* root);
autonomie* find_maximum_autonomie(autonomie* root);

void decrease_number_auto(stazione* stazione);
void increase_number_auto(stazione* stazione);
void print_stazione_with_autonomie(stazione* root);
autonomie* copyAutonomie(autonomie* root);
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

int main() {
    stazione* radice = NULL;

    //file da leggere
    FILE *file = fopen("open_100.txt","r");
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
    int distanza=0;
    int numero_auto=0;
    int autonomia=0;

    int autonomie_temp[512];

    //numero di stazioni
    int numero_stazioni = 0;

    //leggere una riga di comando

    while (fscanf(file,"%s",operazione) != EOF){
        //printf("---------------------------------------------------------\n");
        //print_stazione_with_autonomie(radice);
        //printf("---------------------------------------------------------\n");

        if(strcmp(operazione,"aggiungi-stazione") == 0) {
            if(fscanf(file,"%d %d",&distanza, &numero_auto) != EOF){
                int maxAutonomiaTemp = 0;
                for (int i = 0; i < numero_auto; i++){
                    if(fscanf(file,"%d",&autonomie_temp[i]) != EOF){
                        //leggo una autonomia
                        if(autonomie_temp[i] > maxAutonomiaTemp){
                            maxAutonomiaTemp = autonomie_temp[i];
                        }
                    }
                }

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
                        //printf("aggiungo stazione\n");
                        //salvo le autonomie in un albero
                        autonomie* auto_parcheggiate = NULL;

                        for(int i = 0; i < numero_auto; i++){
                            auto_parcheggiate = insert_autonomie(auto_parcheggiate, autonomie_temp[i]);
                        }
                        radice = insert(radice, distanza, numero_auto, auto_parcheggiate, maxAutonomiaTemp);

                        //aggiorno il numero di stazioni
                        numero_stazioni++;
                        fprintf(file_out,"aggiunta\n");

                    }

                }

            }
        }

        else if(strcmp(operazione,"demolisci-stazione") == 0) {
            if(fscanf(file,"%d",&distanza) != EOF){

                //printf("--->demolisci stazione %d\n",distanza);
                if(search(radice,distanza) == NULL){
                    //non esiste la stazione

                    //printf("stazione non presente\n");
                    //printf("stazione %d non presente\n",distanza);
                    fprintf(file_out,"non demolita\n");
                } else {
                    //esiste la stazione e la cancello

                    //freeAutonomie(search(radice,distanza)->auto_parcheggiate);
                    //search(radice,distanza)->auto_parcheggiate = NULL;
                    //fixme: per le autonomie ho aggiunto una riga nella funzione delete per togliere tutte le autonomie
                    //funziona per tutti  i test senza leak
                    //fixme:Attenzione l'autonomia va a sostituire le autonomie di un'altra stazione
                    radice = delete(radice,distanza);

                    //aggiorno il numero di stazioni
                    numero_stazioni--;

                    fprintf(file_out,"demolita\n");

                    //printf("\n     le stazioni rimaste sono:\n");
                    //inorder(radice);
                    //printf("\n");
                }
            }
        }

        else if(strcmp(operazione,"aggiungi-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza, &autonomia) != EOF){

                //printf("--->aggiungi auto %d alla stazione %d\n",autonomia,distanza);
                //------------------------se esiste la stazione-------------------------------
                if(search(radice,distanza) == NULL){
                    //printf("aggiungi-auto  ->non esiste la stazione  %d\n",distanza);
                    fprintf(file_out,"non aggiunta\n");
                } else {
                    //se il parcheggio è già pieno
                    if(search(radice,distanza)->numero_auto < DIMENSIONE_PARCHEGGIO){
                        //printf("aggiungi-auto  -> aggiunta auto %d alla stazione %d\n",autonomia,distanza);
                        search(radice,distanza)->auto_parcheggiate = insert_autonomie(search(radice,distanza)->auto_parcheggiate, autonomia);
                        increase_number_auto(search(radice,distanza));
                        //aggiorno la maxAutonomia
                        if (autonomia>search(radice,distanza)->maxAutonomia){
                            search(radice,distanza)->maxAutonomia = autonomia;
                        }
                        fprintf(file_out,"aggiunta\n");
                        //printf("-----------aggiungi auto alla stazione %d-------------------\n",distanza);
                        //printf("\n");
                        //inorder_autonomie(search(radice,distanza)->auto_parcheggiate);
                        //printf("\n");
                    } else {
                        //printf("aggiungi-auto  -> parcheggio della stazione %d pieno\n",distanza);

                        fprintf(file_out,"non aggiunta\n");
                    }
                }
                //fprintf(file_out,"aggiungi auto\n");

            }
        }

        else if(strcmp(operazione, "rottama-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza,&autonomia) != EOF){
                //printf("--->rottama auto %d alla stazione %d\n",autonomia,distanza);
                //------------------------se esiste la stazione-------------------------------
                if(search(radice,distanza) == NULL){
                    //printf("rottama auto -> non esiste la stazione %d\n",distanza);
                    fprintf(file_out,"non rottamata\n");
                } else {
                    //se ha almeno un auto
                    if(search(radice,distanza)->numero_auto > 0){

                        //se esiste la stazione, stampo le sue auto
                        //todo test se la stazione esiste
                        if(search(radice,distanza)!=NULL){
                            //printf("\n\n");
                            //printf("|||||||||||||||| la stazione  esiste %d\n",distanza);
                            //printf("le sue auto sono:\n");
                            //inorder_autonomie(search(radice,distanza)->auto_parcheggiate);
                            //printf("\n\n");
                        }
                        //se non esiste l'autonomia
                        if(search_autonomie(search(radice,distanza)->auto_parcheggiate, autonomia) == NULL){
                            //printf("rottama auto ->  non esiste l'autonomia %d della stazione %d\n",autonomia,distanza);
                            fprintf(file_out,"non rottamata\n");
                        } else {
                            //printf("rottama auto -> esiste l'autonomia %d della stazione %d\n",autonomia,distanza);

                            //fixme problema con delete_autonomie !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                            search(radice,distanza)->auto_parcheggiate=delete_autonomie(search(radice,distanza)->auto_parcheggiate, autonomia);
                            //找到站点【获取站点的车辆们】 =删除站点【寻找到站点 获取要删除站点车辆们，传入要删除的某一车辆】
                            //se l'autonomia da togliere è quella massima, dopo averla tolta, aggiorno la maxAutonomia
                            if(search(radice,distanza)->maxAutonomia == autonomia){
                                search(radice,distanza)->maxAutonomia = find_maximum_autonomie(search(radice,distanza)->auto_parcheggiate)->autonomia;
                            }
                            //decrementare il numero di auto
                            decrease_number_auto(search(radice,distanza));
                            //aggiorno la maxAutonomia

                            fprintf(file_out,"rottamata\n");
                        }
                    } else {
                        fprintf(file_out,"non rottamata\n");
                    }
                }

                //fprintf(file_out,"rottama auto\n");
            }
        }

        else if(strcmp(operazione,"pianifica-percorso") == 0) {
            int distanza_destinazione;
            if(fscanf(file,"%d %d",&distanza,&distanza_destinazione) != EOF){
                //printf("--->pianifica percorso %d %d\n",distanza,distanza_destinazione);
                //fprintf(file_out,"(pianifica percorso)   partenza: %d  destinazione: %d\n",distanza,distanza_destinazione);
                printf("(pianifica percorso)   partenza: %d  destinazione: %d\n",distanza,distanza_destinazione);

                //todo --------------------------------------------------
                int numeroStazioniFiltrate;
                int indice=0;
                stazione** lista = createListStation(numero_stazioni);

                //se la distanza di partenza è minore della distanza di destinazione
                if(distanza < distanza_destinazione){
                    numeroStazioniFiltrate = inorderTraversalCrescente(radice,lista,&indice,distanza,distanza_destinazione);
                    //se la distanza di partenza è maggiore della distanza di destinazione

                    /*
                    printf("ci sono %d stazioni\n",numeroStazioniFiltrate);
                    for(int i=0 ; i< numeroStazioniFiltrate;i++) {
                        printf("%d ",lista[i]->distanza);
                    }
                    printf("\n\n");
                     */
                    //todo ------------------------------------------------------------------------------------------------------------------------
                    printPathCrescente(lista,numeroStazioniFiltrate,file_out);
                    //todo -------------------------------------------------------------------------------------------------------------------
                } else {
                    numeroStazioniFiltrate = inorderTraversalDecrescente(radice,lista,&indice,distanza,distanza_destinazione);

                    /*
                    printf("---------------------  INVERSO ---------------------\n\n");
                    printf("ci sono %d stazioni\n",numeroStazioniFiltrate);
                    for(int i=0 ; i< numeroStazioniFiltrate;i++) {
                        //printf("%d ",lista[i]->distanza);
                    }
                    printf("\n++++++++++++++++++++AUTONOMIE ++++++++++++++++++++\n");
                    for(int i=0; i<numeroStazioniFiltrate;i++){
                        //printf("%d ",lista[i]->maxAutonomia);
                    }*/
                    //printf("\n\n\n\n");

                    printDecrescente(lista,numeroStazioniFiltrate,file_out);
                    //fprintf(file_out,"pianifica percorso caso contrario\n");
                    //printPathDecrescente(lista,numeroStazioniFiltrate,file_out);
                    //BFS();
                }

                free(lista);
                //todo --------------------------------------------------

            }
        }
    }

    printf("---------------------------------------------------------\n");

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
stazione* new_node(int x, int y,autonomie* auto_parcheggiate, int maxAutonomia){
    stazione* temp;
    temp = malloc(sizeof(stazione));
    temp->distanza = x;
    temp->numero_auto = y;
    temp->auto_parcheggiate = auto_parcheggiate;
    temp->maxAutonomia = maxAutonomia;
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
stazione* insert(stazione* root, int x, int y, autonomie* auto_parcheggiate,int maxAutonomia){
    if (root == NULL)
        return new_node(x,y,auto_parcheggiate,maxAutonomia);
    else if (x > root->distanza)
        root->destro= insert(root->destro, x,y,auto_parcheggiate,maxAutonomia);
    else
        root -> sinistro = insert(root->sinistro, x,y,auto_parcheggiate,maxAutonomia);
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
            freeAutonomie(root->auto_parcheggiate);
            //fixme: prova
            root->auto_parcheggiate = NULL;
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
            freeAutonomie(root->auto_parcheggiate);
            //fixme: prova
            root->auto_parcheggiate = NULL;
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
            freeAutonomie(root->auto_parcheggiate);
            root->auto_parcheggiate = copyAutonomie(temp->auto_parcheggiate);
            root->destro = delete(root->destro, temp->distanza);
        }
    }
    return root;
}

//stampa in ordine
void inorder(stazione *root){
    if (root != NULL)
    {
        inorder(root->sinistro);
        printf(" %d ", root->distanza);
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
    freeAutonomie(root->auto_parcheggiate);
    free(root);
}
//-----------------------------stazione----------------------------
autonomie* new_node_autonomia(int x){
    autonomie *temp;
    temp = malloc(sizeof(autonomie));
    temp->autonomia = x;
    temp->sinistro = NULL;
    temp->destro = NULL;

    return temp;
}

//ricerca nodo
autonomie *search_autonomie(autonomie *root, int x){
    if (root == NULL || root->autonomia == x)
        return root;
    else if (x > root->autonomia)
        return search_autonomie(root->destro, x);
    else
        return search_autonomie(root->sinistro, x);
}

autonomie* insert_autonomie(autonomie* root, int x){
    if (root == NULL)
        return new_node_autonomia(x);
    else if (x > root->autonomia)
        root->destro = insert_autonomie(root->destro, x);
    else
        root -> sinistro = insert_autonomie(root->sinistro, x);
    return root;
}

autonomie* find_minimum_autonomie(autonomie* root) {
    if (root == NULL)
        return NULL;
    else if (root->sinistro != NULL)
        return find_minimum_autonomie(root->sinistro);
    return root;
}

autonomie* find_maximum_autonomie(autonomie* root) {
    if (root == NULL)
        return NULL;
    else if (root->destro != NULL)
        return find_maximum_autonomie(root->destro);
    return root;
}


autonomie* delete_autonomie(autonomie * root, int x) {

    if (root == NULL)
        return NULL;
    if (x > root->autonomia)
        root->destro = delete_autonomie(root->destro, x);
    else if (x < root->autonomia)
        root->sinistro= delete_autonomie(root->sinistro, x);
    else {
        if (root->sinistro == NULL && root->destro == NULL){
            free(root);
            return NULL;
        }
        else if (root->sinistro == NULL || root->destro == NULL){
            autonomie *temp = NULL;
            if (root->sinistro == NULL)
                temp = root->destro;
            else
                temp = root->sinistro;
            free(root);
            return temp;
        }
        else {
            autonomie* temp = find_minimum_autonomie(root->destro);
            root->autonomia= temp->autonomia;
            root->destro = delete_autonomie(root->destro, temp->autonomia);
        }
    }
    return root;
}

void inorder_autonomie(autonomie *root){
    if (root != NULL)
    {
        inorder_autonomie(root->sinistro);
        printf(" %d ", root->autonomia);
        inorder_autonomie(root->destro);
    }
}

void freeAutonomie(autonomie* root) {
    if (root == NULL)
        return;

    freeAutonomie(root->sinistro);
    freeAutonomie(root->destro);
    free(root);
}

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

void print_stazione_with_autonomie(stazione* root) {
    if (root == NULL){
        //printf("\n<<<Non ci sono stazioni>>>\n");
        return;

    } else {
        printf("-------------------------------------------------------------------------------------\n");
        printf("\n<<<Stazione %d ha %d autonomie>>>\n", root->distanza, root->numero_auto);
        printf("...Autonomie sono: ");
        inorder_autonomie(root->auto_parcheggiate);
        printf("\n\n");

        print_stazione_with_autonomie(root->sinistro);
        print_stazione_with_autonomie(root->destro);
    }


}
struct Autonomie* copyAutonomie(struct Autonomie* root) {
    if (root == NULL)
        return NULL;

    struct Autonomie* new_node = (struct Autonomie*) malloc(sizeof(struct Autonomie));
    new_node->autonomia = root->autonomia;
    new_node->sinistro = copyAutonomie(root->sinistro);
    new_node->destro = copyAutonomie(root->destro);
    return new_node;
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
            printf("---percorso: IMPOSSIBILE\n\n");
            fprintf(out,"nessun percorso\n");
            free(vettore);
            free(path);
            return;
        }
    }
    printf("---percorso: ");

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

    printf("\n");
    //partenza
    printf("%d ",lista[0]->distanza);
    fprintf(out,"%d ",lista[0]->distanza);

    for(int n=0;n<numeroStazioni;n++){
        if(path[n] != INT_MAX){
            printf("%d ",path[n]);
            fprintf(out,"%d ",path[n]);
        }
    }

    //destinazione
    printf("%d\n",lista[numeroStazioni-1]->distanza);
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
        printf("NESSUN PERCORSO\n");
        fprintf(out,"nessun percorso\n");
        free(visitato);
        freeTree(root);
        free(queue);
        return;
    }

    printPathToRoot(nodeFinal);
    printPathToRootToFile(nodeFinal,out);
    fprintf(out,"\n");
    printf("\n");
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
