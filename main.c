#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define DIMENSIONE_PARCHEGGIO 512
#define MAXN 200
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

struct Graph {
    int numeroNodi;
    int** matriceAdiacenza;
};

//------------------------------------ per pianifica percorso
// 节点结构体
typedef struct {
    int index;
    int steps;
} Node;

// 队列结构体
typedef struct {
    Node* nodes;
    int front;
    int rear;
    int size;
} Queue;

typedef struct Graph grafo;
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
void rilasciaGrafo(grafo* grafo);

int inorderTraversalCrescente(stazione* radice, stazione** lista,int* indice,int partenza,int destinazione);
int inorderTraversalDecrescente(stazione* radice, stazione** lista,int* indice,int partenza,int destinazione);

//-----------------------------------------------------------

void printPathCrescente(stazione** lista,int numeroStazioni);

int main() {
    stazione* radice = NULL;

    //file da leggere
    FILE *file = fopen("open_1.txt","r");
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


                            search(radice,distanza)->auto_parcheggiate=delete_autonomie(search(radice,distanza)->auto_parcheggiate, autonomia);

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

                    printf("ci sono %d stazioni\n",numeroStazioniFiltrate);
                    for(int i=0 ; i< numeroStazioniFiltrate;i++) {
                        printf("%d ",lista[i]->distanza);
                    }
                    printf("\n\n");
                    //todo ------------------------------------------------------------------------------------------------------------------------
                    printPathCrescente(lista,numeroStazioniFiltrate);
                    //todo -------------------------------------------------------------------------------------------------------------------
                } else {
                    numeroStazioniFiltrate = inorderTraversalDecrescente(radice,lista,&indice,distanza_destinazione,distanza_destinazione);
                    fprintf(file_out,"pianifica percorso caso contrario\n");
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
            autonomie *temp;
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

void printPathCrescente(stazione** lista,int numeroStazioni) {

    //inizializzare un vettore dinamico
    int* vettore = (int*)malloc(numeroStazioni * sizeof(int));

    //inizializzare un vettore dinamico per il path
    int* path = (int*)malloc(numeroStazioni * sizeof(int));

    //inizializzare un vettore dinamico con il valore -1
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

    for(int n=0;n<numeroStazioni;n++){
        if(path[n] != INT_MAX){
            printf("%d ",path[n]);
        }
    }

    //destinazione
    printf("%d\n",lista[numeroStazioni-1]->distanza);
    //libero memoria
    free(vettore);
    free(path);
    return;
}