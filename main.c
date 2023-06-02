    #include <stdio.h>
    #include <string.h>
    #include <stdlib.h>

    /**
     * 0. in ogni stazione è costretto di cambiare auto
     * 1. il numero di salto minore possibile
     * 2. stazione non ripetuta
     * 3. automobili possono avere la stessa autonomia
     * 4. quando si stampa il percorso, si deve includere anche la partenza e l'arrivo
     * 5. due sensi di percorrenza
     * 6. non tornare indietro
     * 7. ogni stazione può contenere al massimo 512 veicoli
     *
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
    void stampaAuto(Stazione *head, int distanza);
    int main() {

        //dichiarazione per lettura
        char riga[5000];
        //separatore di riga
        const char separatore[] = " ";
        char *token;

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
                }
                else{
                    printf("Stazione non aggiunta\n");
                    //fixme
                    free(listaAutonomia);
                }
                //stampa stazioni per verifica
                stampaStazioni(head);
                stampaAuto(head,distanza);
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


                stampaAuto(head,distanza);
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
        //fixme
        free(current);
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
        stampaAuto(head, distanza);
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
    void stampaAuto(Stazione *head, int distanza) {
        Stazione *current = head;

        // Cerca la stazione con la distanza data
        while (current != NULL && current->distanza != distanza) {
            current = current->successiva;
        }

        // Se non esiste una stazione con la distanza data, ritorna
        if (current == NULL) {
            return;
        }
    /*
        // Ordina le auto in ordine di autonomia
        for (int i = 0; i < current->numeroAuto - 1; i++) {
            for (int j = i + 1; j < current->numeroAuto; j++) {
                if (current->Auto[i] > current->Auto[j]) {
                    int temp = current->Auto[i];
                    current->Auto[i] = current->Auto[j];
                    current->Auto[j] = temp;
                }
            }
        }
    */

        // Stampa le auto
        for (int i = 0; i < current->numeroAuto; i++) {
            printf("%d ", current->Auto[i]);
        }
        printf("\n");

    }