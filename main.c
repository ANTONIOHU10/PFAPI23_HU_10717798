#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DIMENSIONE_PARCHEGGIO 512
//struttura per le auto

//struttura per le stazioni, AVL tree
struct Stazione{
    int distanza; //key
    int numero_auto;
    struct Autonomie* auto_parcheggiate;
    struct Stazione* sinistro;
    struct Stazione* destro;

};

struct Autonomie{
    int autonomia;
    struct Autonomie* sinistro;
    struct Autonomie* destro;
};

typedef struct Autonomie autonomie;
typedef struct Stazione stazione;

stazione* new_node(int x, int y, autonomie* auto_parcheggiate);
stazione* search(stazione* root, int x);
stazione* insert(stazione* root, int x, int y, autonomie* auto_parcheggiate);
stazione* find_minimum(stazione* root);
stazione* delete(stazione* root, int x);
void inorder(stazione* root);
void freeStazione(stazione* root);

autonomie* new_node_autonomie(int x);
autonomie* search_autonomie(autonomie* root, int x);
autonomie* insert_autonomie(autonomie* root, int x);
autonomie* find_minimum_autonomie(autonomie* root);
autonomie* delete_autonomie(autonomie* root, int x);
void inorder_autonomie(autonomie* root);
void freeAutonomie(autonomie* root);


int main() {

    //TODO----------------TEST AVL-------------------------------
    stazione* radice = NULL;
    autonomie* auto_parcheggiate = NULL;

    //TODO   per cancellare i contenuti, devo avere una variabile che riceve il ritorno della funzione di creazione
    //TODO   e poi passare quella variabile alla funzione di cancellazione , altrimenti non posso cancellare
    auto_parcheggiate = insert_autonomie(auto_parcheggiate, 10);
    radice = insert(radice, 10, 10, auto_parcheggiate);

    freeStazione(search(radice,10));

    //TODO----------------TEST AVL-------------------------------

    /*
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
                fprintf(file_out,"(aggiungi stazione)   distanza: %d    numero auto: %d\n",distanza,numero_auto);

            }
        }

        else if(strcmp(operazione,"demolisci-stazione") == 0) {
            if(fscanf(file,"%d",&distanza) != EOF){
                   fprintf(file_out,"(demolisci stazione)   distanza: %d\n",distanza);

            }
        }

        else if(strcmp(operazione,"aggiungi-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza, &autonomia) != EOF){

                fprintf(file_out,"aggiungi auto\n");

            }
        }

        else if(strcmp(operazione, "rottama-auto") == 0) {
            if(fscanf(file,"%d %d",&distanza,&autonomia) != EOF){

                fprintf(file_out,"rottama auto\n");
            }
        }

        else if(strcmp(operazione,"pianifica-percorso") == 0) {
            int distanza_destinazione;
            if(fscanf(file,"%d %d",&distanza,&distanza_destinazione) != EOF){
                fprintf(file_out,"(pianifica percorso)   partenza: %d  destinazione: %d\n",distanza,distanza_destinazione);
            }
        }

    }

    printf("---------------------------------------------------------\n");

    fclose(file);
    fclose(file_out);
    */

    return 0;
}



//-----------------------------stazione----------------------------
//creazione nodo
//x  = distanza
//y = numero auto
stazione* new_node(int x, int y,autonomie* auto_parcheggiate){
    stazione* temp;
    temp = malloc(sizeof(stazione));
    temp->distanza = x;
    temp->numero_auto = y;
    temp->auto_parcheggiate = auto_parcheggiate;
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

//inserimento nodo
stazione* insert(stazione* root, int x, int y, autonomie* auto_parcheggiate){
    if (root == NULL)
        return new_node(x,y,auto_parcheggiate);
    else if (x > root->distanza)
        root->destro= insert(root->destro, x,y,auto_parcheggiate);
    else
        root -> sinistro = insert(root->sinistro, x,y,auto_parcheggiate);
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
    if (x > root->distanza)
        root->destro = delete(root->destro, x);
    else if (x < root->distanza)
        root->sinistro = delete(root->sinistro, x);
    else {
        if (root->sinistro == NULL && root->destro == NULL){
            free(root);
            return NULL;
        }
        else if (root->sinistro == NULL || root->destro == NULL){
            stazione* temp;
            if (root->sinistro == NULL)
                temp = root->destro;
            else
                temp = root->sinistro;
            free(root);
            return temp;
        }
        else {
            stazione *temp = find_minimum(root->destro);
            root->distanza= temp->distanza;
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