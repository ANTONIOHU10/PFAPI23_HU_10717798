    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    #define INT_MAX 2147483647
    /**
     * 0. in ogni stazione è costretto di cambiare auto
     * 1. il numero di salto minore possibile
     * 2. stazione non ripetuta
     * 3. automobili possono avere la stessa autonomia
     * 4. quando si stampa il percorso, si deve includere anche la partenza e l'arrivo
     * 5. due sensi di percorrenza
     * 6. non tornare indietro
     * 7. ogni stazione può contenere al massimo 512 veicoli
     * 8. ogni stazione può anche non contenere veicoli
     * utilizzo struttura dati:
     * todo lista collegata per le stazioni
     * todo array dinamico per auto
     *
     */

    typedef struct Stazione Stazione;
    struct Stazione{
        //distanza dall'origine
        int distanza;
        int numeroAuto;
        int *Auto;
        struct Stazione *precedente;
        struct Stazione *successiva;
    } ;
    Stazione *creaStazione(int distanza, int numeroAuto, int *Auto);
    int aggiungiStazione(Stazione **head, int distanza, int numeroAuto,int *Auto);
    void stampaStazioni(Stazione *head);
    int rimuoviStazione(Stazione **head, int distanza);
    int aggiungiAuto(Stazione *head, int distanza, int autonomia);
    int rimuoviAuto(Stazione *head, int distanza, int autonomia);
    void stampaAuto(Stazione *head);

    int **creaMatriceAdiacenza(int numStazioni);
    void riempiMatriceAdiacenza(Stazione* head,int **matrice,int numStazioni);
    Stazione* getStazioneByIndex(Stazione* head,int index);
    int trovaAutonomiaMassima(Stazione* head);
    void dijkstra(int **matriceAdiacenza, int *distanza, int *precedente, int numStazioni, int stazionePartenza, int stazioneArrivo);
    void stampa_percorso(int *precedente, int stazionePartenza, int stazioneArrivo);

    void stampaListaStazioni(Stazione* head);
    int main() {

        //dichiarazione per lettura
        char riga[5000];
        //separatore di riga
        const char separatore[] = " ";
        char *token;
        int numeroStazioni=0;
        //dichiarazione stazione di partenza
        Stazione *head = NULL;
        //leggere una riga di comando
        while (fgets(riga, 5000, stdin) != NULL) {

            //tolgo ultimo \n newline letto da fgets
            riga[strlen(riga) - 1] = '\0';

            //separa la riga in token
            token= strtok(riga, separatore);
            if(strcmp(token,"aggiungi-stazione") ==0 ){
                printf("+Caso aggiungi stazione\n");

                //ottengo la distanza stazione
                token = strtok(NULL, separatore);
                int distanza = atoi(token);
                printf("Distanza: %d\n",distanza);
                //ottengo il numero di auto
                token = strtok(NULL, separatore);
                //fixme
                int numeroAuto = atoi(token);
                printf("Numero auto: %d\n",numeroAuto);

                //ottengo la autonomia di ogni auto
                int* listaAutonomia = malloc(numeroAuto*sizeof(int));
                for(int i=0;i<numeroAuto;i++){
                    //ottengo l'autonomia
                    token = strtok(NULL, separatore);
                    int autonomia = atoi(token);
                    printf("Autonomia: %d\n",autonomia);
                    listaAutonomia[i] = autonomia;
                }
                //aggiungo la stazione
                int risultato = aggiungiStazione(&head,distanza,numeroAuto,listaAutonomia);
                if(risultato == 1){
                    printf("Stazione aggiunta\n");
                    numeroStazioni++;
                }
                else{
                    printf("Stazione non aggiunta\n");
                    free(listaAutonomia);
                }
                //stampa stazioni per verifica
                stampaStazioni(head);
                stampaAuto(head);
                //free(listaAutonomia);
            }

            else if(strcmp(token,"demolisci-stazione") ==0 ){
                printf("+Caso rimuovi stazione\n");
                //ottengo la distanza stazione
                token = strtok(NULL, separatore);
                int distanza = atoi(token);
                printf("Distanza: %d\n",distanza);
                int risultato= rimuoviStazione(&head,distanza);
                if(risultato == 1){
                    printf("Stazione rimossa\n");
                    numeroStazioni--;
                }
                else{
                    printf("Stazione non rimossa\n");
                }
            }
            else if(strcmp(token,"aggiungi-auto") ==0 ){
                printf("+Caso aggiungi auto\n");
                //ottengo distanza stazione(stazione)
                token = strtok(NULL, separatore);
                int distanza = atoi(token);
                printf("distanza-stazione: %d\n",distanza);
                //ottengo la autonomia
                token = strtok(NULL, separatore);
                int autonomia = atoi(token);
                printf("Autonomia: %d\n",autonomia);
                //aggiungo l'auto
                int risultato =aggiungiAuto(head,distanza,autonomia);
                if(risultato == 1){
                    printf("Auto aggiunta\n");
                }
                else{
                    printf("Auto non aggiunta\n");
                }
                stampaStazioni(head);
            }
            else if(strcmp(token,"rottama-auto") ==0 ){
                printf("+Caso rimuovi auto\n");
                //ottengo la distanza-stazione
                token = strtok(NULL, separatore);
                int distanza = atoi(token);
                printf("Distanza-stazione: %d\n",distanza);
                //ottengo l'autonomia
                token = strtok(NULL, separatore);
                int autonomia = atoi(token);
                printf("Autonomia: %d\n",autonomia);


                stampaAuto(head);
                int risultato = rimuoviAuto(head,distanza,autonomia);
                if(risultato == 1){
                    printf("Auto rimossa\n");
                }
                else{
                    printf("Auto non rimossa\n");
                }
                stampaStazioni(head);
            }
            else if(strcmp(token,"pianifica-percorso") ==0 ){
                printf("+Caso stampa percorso\n");
                //ottengo la stazione di partenza
                token = strtok(NULL, separatore);
                int stazione_partenza = atoi(token);
                printf("Stazione di partenza: %d\n",stazione_partenza);
                //ottengo la stazione di destinazione
                token = strtok(NULL, separatore);
                int stazione_destinazione = atoi(token);
                printf("Stazione di destinazione: %d\n",stazione_destinazione);

                stampaAuto(head);
                //todo stampa percorso
                int **matrice = creaMatriceAdiacenza(numeroStazioni);
                printf("------------------------prova 0----------------------------------\n");
                riempiMatriceAdiacenza(head,matrice,numeroStazioni);
                int *dist = malloc(numeroStazioni*sizeof(int));
                printf("------------------------prova 0.1----------------------------------\n");
                int *pred = malloc(numeroStazioni*sizeof(int));
                printf("------------------------prova 0.2----------------------------------\n");
                dijkstra(matrice,dist,pred,numeroStazioni,stazione_partenza,stazione_destinazione);
                printf("------------------------prova 1----------------------------------\n");
                stampa_percorso(pred,stazione_partenza,stazione_destinazione);
                printf("------------------------prova 2----------------------------------\n");

                free(dist);
                free(pred);
                for(int i=0;i<numeroStazioni;i++){
                    free(matrice[i]);
                }
                free(matrice);

                printf("---------------------Fine stampa percorso----------------\n");
            }
            else if(strcmp(token,"fine") ==0 ){
                printf("+Caso fine\n");
                break;
            }
            else{
                printf("Caso non riconosciuto\n");
            }
        }

        return 0;
    }

    Stazione* creaStazione(int distanza, int numeroAuto, int *Auto){
        //creo la nuova stazione
        Stazione *nuovaStazione = malloc(sizeof(Stazione));
        nuovaStazione->distanza = distanza;
        nuovaStazione->numeroAuto = numeroAuto;
        nuovaStazione->Auto = Auto;
        nuovaStazione->precedente = NULL;
        nuovaStazione->successiva = NULL;

        return nuovaStazione;
    }

    //passo la prima stazione e la distanza della nuova stazione
    //1 = inserimento successo, 0 = inserimento fallito
    int aggiungiStazione(Stazione **head, int distanza, int numeroAuto,int *Auto) {
        Stazione *newStazione = creaStazione(distanza, numeroAuto, Auto);
        Stazione *current = *head;

        // se l'autostrada è vuota, aggiungi il nuovo nodo all'inizio
        if (*head == NULL) {
            *head = newStazione;
            return 1;
        }

        // se la nuova stazione ha distanza minore rispetto alla prima stazione
        if ((*head)->distanza > distanza) {
            newStazione->successiva = *head;
            (*head)->precedente = newStazione;
            *head = newStazione;
            return 1;
        }

        // cerca la posizione corretta nella lista
        while (current->successiva != NULL && current->successiva->distanza < distanza) {
            current = current->successiva;
        }

        // se stazione con stessa distanza esiste gia', restituisci senza aggiungere
        if(current->distanza == distanza || (current->successiva && current->successiva->distanza == distanza)){
            free(newStazione);
            return 0;
        }

        // inserisci la nuova stazione nella posizione corretta
        newStazione->successiva = current->successiva;
        if (current->successiva != NULL) {
            current->successiva->precedente = newStazione;
        }
        current->successiva = newStazione;
        newStazione->precedente = current;
        return 1;
    }
    // Funzione per rimuovere una stazione data la sua distanza
    //1 = rimozione successo, 0 = rimozione fallito
    int rimuoviStazione(Stazione **head, int distanza) {
        Stazione *current = *head;

        // Cerca la stazione con la distanza data
        while (current != NULL && current->distanza != distanza) {
            current = current->successiva;
        }

        // Se non esiste una stazione con la distanza data, ritorna
        if (current == NULL) {
            return 0;
        }

        // Aggiorna i collegamenti dei nodi adiacenti
        if (current->precedente != NULL) {
            current->precedente->successiva = current->successiva;
        } else {
            *head = current->successiva;
        }
        if (current->successiva != NULL) {
            current->successiva->precedente = current->precedente;
        }

        // Libera la memoria occupata dalla stazione
        free(current);
        return 1;
    }

    void stampaStazioni(Stazione *head) {
        Stazione *temp = head;
        while(temp != NULL) {
            printf("Stazione: %d, Numero Auto: %d\n", temp->distanza, temp->numeroAuto);
            temp = temp->successiva;
        }
    }

    // Funzione per aggiungere una auto a una stazione data la sua distanza
    int aggiungiAuto(Stazione *head, int distanza, int autonomia) {
        Stazione *current = head;

        // Cerca la stazione con la distanza data
        while (current != NULL && current->distanza != distanza) {
            current = current->successiva;
        }

        // Se non esiste una stazione con la distanza data, ritorna
        if (current == NULL) {
            return 0;
        }

        // Se la stazione ha già il numero massimo di auto, ritorna
        if (current->numeroAuto >= 512) {
            return 0;
        }

        // Alloca più memoria per le auto e aggiunge la nuova auto
        current->Auto = realloc(current->Auto, (current->numeroAuto + 1) * sizeof(int));
        current->Auto[current->numeroAuto] = autonomia;
        current->numeroAuto++;
        return 1;
    }


    // Funzione per rimuovere la prima auto con l'autonomia specificata da una stazione data la sua distanza
    int rimuoviAuto(Stazione *head, int distanza, int autonomia) {
        Stazione *current = head;

        // Cerca la stazione con la distanza data
        while (current != NULL && current->distanza != distanza) {
            current = current->successiva;
        }

        // Se non esiste una stazione con la distanza data, ritorna
        if (current == NULL) {
            return 0;
        }
        printf("stazione trovata\n");
        stampaAuto(head);
        // Cerca la prima auto con l'autonomia data
        for (int i = 0; i < current->numeroAuto; i++) {
            if (current->Auto[i] == autonomia) {
                // Sposta l'ultima auto nella posizione dell'auto da rimuovere
                current->Auto[i] = current->Auto[current->numeroAuto - 1];
                // Riduci la memoria allocata per le auto
                current->Auto = realloc(current->Auto, (current->numeroAuto - 1) * sizeof(int));
                current->numeroAuto--;
                return 1;
            }
        }
        return 0;
    }


    // Funzione per stampare tutte le auto di una stazione data la sua distanza in ordine di autonomia
    void stampaAuto(Stazione* head) {
        Stazione* current = head;

        while (current != NULL) {
            printf("Stazione %d: ", current->distanza);
            printf("Numero auto: %d, ", current->numeroAuto);
            for (int i = 0; i < current->numeroAuto; i++) {
                printf("%d ", current->Auto[i]);
            }
            printf("\n");
            current = current->successiva;
        }
    }

    // Crea e inizializza una matrice di adiacenza con un valore molto grande
    int **creaMatriceAdiacenza(int numStazioni) {
        int **matrice = malloc(numStazioni * sizeof(int*));
        for (int i = 0; i < numStazioni; i++) {
            matrice[i] = malloc(numStazioni * sizeof(int));
            for (int j = 0; j < numStazioni; j++) {
                matrice[i][j] = INT_MAX;
            }
        }
        return matrice;
    }
    void riempiMatriceAdiacenza(Stazione* head, int** matrice,int numStazioni) {
        Stazione* current = head;
        printf("-----------------verifica riempimento 0---------------------\n");

            stampaAuto(head);
            // Trova l'autonomia massima nella stazione corrente
            int autonomia_max = 0;
            for (int i = 0; i < current->numeroAuto; i++) {
                printf("-----------------verifica riempimento 0.1---------------------\n");
                //se una stazione ha auto
                if (current->Auto[i] > autonomia_max) {
                    autonomia_max = current->Auto[i];
                }
            }
            printf("-----------------verifica riempimento 0.2---------------------\n");
            // Verifica se la stazione corrente può raggiungere altre stazioni
            //------------------------------------------------------------------------------------------------------------------------------
            Stazione* next = current->successiva;
            printf("-----------------verifica riempimento 0.3---------------------\n");
            for(int i=0;i<numStazioni;i++){
                int autonomia_max=trovaAutonomiaMassima(getStazioneByIndex(head,i));
                for(int j=0;j<numStazioni;j++){
                    //se la stazione è la stessa
                    if(i==j){
                        matrice[i][j]=0;
                    }

                    else if(autonomia_max
                        >=
                    abs(getStazioneByIndex(head,i)->distanza- getStazioneByIndex(head,j)->distanza)){
                        matrice[i][j]=1;
                    }
                    else matrice[i][j]=0;
                }
            }
        for (int i = 0; i < numStazioni; i++) {
            for (int j = 0; j < numStazioni; j++) {
                printf("%d\t", matrice[i][j]);
            }
            printf("\n");
        }

    }
    Stazione* getStazioneByIndex(Stazione* head, int index) {
        Stazione* current = head;
        int count = 0;

        while (current != NULL) {
            if (count == index) {
                return current;
            }
            current = current->successiva;
            count++;
        }

        // Se l'indice non esiste nella lista
        return NULL;
    }
    int trovaAutonomiaMassima(Stazione* head) {

        int autonomiaMax = 0;
        Stazione* current = head;

        for (int i = 0; i < current->numeroAuto; i++) {
            if (current->Auto[i] > autonomiaMax) {
                autonomiaMax = current->Auto[i];
            }
        }

        printf("Autonomia massima: %d\n", autonomiaMax);
        return autonomiaMax;
    }


    //adesso la matrice di adiacenza è giusta e corretta, devo utilizzare il corretto dijkstra per trovare il percorso più corto
    //fixme
    void dijkstra(int **matrice, int *dist, int *prev, int numStazioni, int sorgente, int destinazione) {
        int *visited = calloc(numStazioni, sizeof(int));
        for (int i = 0; i < numStazioni; i++) {
            dist[i] = INT_MAX;
            prev[i] = -1;
        }
        dist[sorgente] = 0;

        for (int i = 0; i < numStazioni; i++) {
            int u = -1;
            for (int j = 0; j < numStazioni; j++) {
                if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                    u = j;
                }
            }

            if (u == -1 || dist[u] == INT_MAX) {
                break;
            }

            visited[u] = 1;

            if (u == destinazione) {
                break;
            }

            for (int j = 0; j < numStazioni; j++) {
                if (matrice[u][j] != INT_MAX && dist[u] + matrice[u][j] < dist[j]) {
                    dist[j] = dist[u] + matrice[u][j];
                    prev[j] = u;
                }
            }
        }
        free(visited);
    }

    //fixme
    void stampa_percorso(int *prev, int sorgente, int destinazione) {
        int *path = malloc(sizeof(int)); // array dinamico per contenere il percorso
        int count = 0; // contatore per la lunghezza del percorso
        printf("------------------------prova 3----------------------------------\n");
        // costruiamo il percorso dalla destinazione alla sorgente
        int u = destinazione;
        while (u != sorgente && prev[u] != -1) {
            u = prev[u];
            path = realloc(path, (count + 1) * sizeof(int));
            path[count] = u;
            count++;
            printf("%d\n", u);
        }
        printf("------------------------prova 4----------------------------------\n");
        path = realloc(path, (count + 1) * sizeof(int)); // riallocazione per la sorgente
        path[count] = sorgente; // aggiunge la sorgente alla fine del percorso
        printf("------------------------prova 5----------------------------------\n");
        // stampiamo il percorso in ordine inverso, dalla sorgente alla destinazione
        printf("Il percorso più breve è: ");
        for (int i = count; i >= 0; i--) {
            printf("%d", path[i]);
            if (i != 0) printf(" -> ");
        }
        printf("\n");

        free(path);
    }

    void stampaListaStazioni(Stazione *head) {
        Stazione *current = head;
        int indice = 0;

        while (current != NULL) {
            printf("Stazione %d: distanza %d, numero auto %d\n", indice, current->distanza, current->numeroAuto);
            current = current->successiva;
            indice++;
        }
    }