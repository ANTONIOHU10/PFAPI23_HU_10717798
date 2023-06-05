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
    typedef struct {
        int distanza; // Distanza dalla partenza dell'autostrada
        int numeroAuto; // Numero di veicoli nella stazione
        int *autoveicoli; // Array delle autonomie dei veicoli
    } Stazione;

    typedef struct {
        int partenza; // Nodo di partenza dell'arco
        int destinazione; // Nodo di arrivo dell'arco
        int peso; // Peso dell'arco (distanza tra le stazioni)
    } Arco;

    typedef struct {
        int numeroStazioni; // Numero di stazioni di servizio
        Stazione *stazioni; // Array delle stazioni di servizio
        int numeroArchi; // Numero di archi nel grafo
        Arco *archi; // Array degli archi
    } Autostrada;

    Autostrada autostrada;


    int aggiungiStazione(int distanza, int numeroAuto, int *auoveicoli);
    int rimuoviStazione(int distanza);

    void aggiungiArco(int partenza, int destinazione, int peso);
    int rimuoviArco(int partenza, int destinazione);

    int aggiungiAuto(int distanza, int autonomia);
    int rimuoviAuto(int distanza, int autonomia);

    int trovaAutonomiaMassima(int distanza);

    void stampaArchi();
    void stampaStazioni();
    int esisteArco(int partenza, int destinazione);

    int trovaIndiceStazione(int distanza);
    void inizializzaDistanze(int *distanze, int* predecessori, int numeroStazioni);
    void dijkstra(int partenza, int destinazione);


    int main() {

        //dichiarazione per lettura
        char riga[5000];
        //separatore di riga
        const char separatore[] = " ";
        char *token;

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
                int risultato = aggiungiStazione(distanza,numeroAuto,listaAutonomia);
                if(risultato == 1){
                    printf("Stazione aggiunta\n");

                }
                else{
                    printf("Stazione non aggiunta\n");
                    free(listaAutonomia);
                }

            }
            else if(strcmp(token,"demolisci-stazione") ==0 ){
                printf("+Caso rimuovi stazione\n");
                //ottengo la distanza stazione
                token = strtok(NULL, separatore);
                int distanza = atoi(token);
                printf("Distanza: %d\n",distanza);
                int risultato= rimuoviStazione(distanza);
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
                int risultato =aggiungiAuto(distanza,autonomia);
                if(risultato == 1){
                    printf("Auto aggiunta\n");
                }
                else{
                    printf("Auto non aggiunta\n");
                }

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


                int risultato = rimuoviAuto(distanza,autonomia);
                if(risultato == 1){
                    printf("Auto rimossa\n");
                }
                else{
                    printf("Auto non rimossa\n");
                }

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


                if (trovaIndiceStazione(stazione_partenza) == -1 || trovaIndiceStazione(stazione_destinazione) == -1) {
                printf("non esiste la partenza / destinazione\n");
                } else {
                    dijkstra(trovaIndiceStazione(stazione_partenza), trovaIndiceStazione(stazione_destinazione));
                    //todo da verificare se il percorso scelto è quello migliore (se prende la stazione di servizio più vicina all'inizio, cioè valore più piccolo)
                }

                printf("---------------------Fine stampa percorso----------------\n");
            }
            else if(strcmp(token,"fine") ==0 ){
                printf("+Caso fine\n");
                break;
            }
            else if(strcmp(token,"test") ==0){
                stampaStazioni();
                stampaArchi();
            }
            else{
                printf("Caso non riconosciuto\n");
            }
        }

        return 0;
    }

// Funzione per aggiungere una stazione di servizio al grafo
    int aggiungiStazione(int distanza, int numeroAuto, int *autoveicoli) {
        // Verifica se la stazione esiste già
        for (int i = 0; i < autostrada.numeroStazioni; i++) {
            if (autostrada.stazioni[i].distanza == distanza) {
                return 0;
            }
        }
        //aggiunge la stazione
        autostrada.numeroStazioni++;
        autostrada.stazioni = realloc(autostrada.stazioni, autostrada.numeroStazioni * sizeof(Stazione));
        autostrada.stazioni[autostrada.numeroStazioni - 1].distanza = distanza;
        autostrada.stazioni[autostrada.numeroStazioni - 1].numeroAuto = numeroAuto;
        autostrada.stazioni[autostrada.numeroStazioni - 1].autoveicoli = malloc(numeroAuto * sizeof(int));
        for (int i = 0; i < numeroAuto; i++) {
            autostrada.stazioni[autostrada.numeroStazioni - 1].autoveicoli[i] = autoveicoli[i];
        }

        // Aggiunta degli archi tra le stazioni raggiungibili
        for (int i = 0; i < autostrada.numeroStazioni - 1; i++) {
            int autonomia_massima_i = trovaAutonomiaMassima(autostrada.stazioni[i].distanza);
            int autonomia_massima_nuova = trovaAutonomiaMassima(distanza);

            // Verifica se la stazione i può raggiungere la nuova stazione
            if (autonomia_massima_i >= abs(distanza - autostrada.stazioni[i].distanza)) {
                aggiungiArco(i, autostrada.numeroStazioni - 1, abs(distanza - autostrada.stazioni[i].distanza));
                printf("da stazione %d a stazione %d arco aggiunto\n", i, autostrada.numeroStazioni - 1);
            }

            // Verifica se la nuova stazione può raggiungere la stazione i
            if (autonomia_massima_nuova >= abs(autostrada.stazioni[i].distanza - distanza)) {
                aggiungiArco(autostrada.numeroStazioni - 1, i, abs(autostrada.stazioni[i].distanza - distanza));
                printf("da stazione %d a stazione %d arco aggiunto\n", autostrada.numeroStazioni - 1, i);
            }
        }
        return 1;
    }

// Funzione per rimuovere una stazione di servizio dal grafo
    int rimuoviStazione(int distanza) {
        int indice = -1;

        // Trova l'indice della stazione da rimuovere
        for (int i = 0; i < autostrada.numeroStazioni; i++) {
            if (autostrada.stazioni[i].distanza == distanza) {
                indice = i;
                break;
            }
        }

        // Se l'indice è valido, rimuovi la stazione
        if (indice != -1) {
            // Sposta l'ultima stazione nell'indice da rimuovere
            autostrada.stazioni[indice] = autostrada.stazioni[autostrada.numeroStazioni - 1];
            autostrada.numeroStazioni--;
            autostrada.stazioni = realloc(autostrada.stazioni, autostrada.numeroStazioni * sizeof(Stazione));

            // Rimuovi gli archi collegati
            for (int i = 0; i < autostrada.numeroArchi; i++) {
                if (autostrada.archi[i].partenza == distanza || autostrada.archi[i].destinazione == distanza) {
                    // Sposta l'ultimo arco nell'indice da rimuovere
                    autostrada.archi[i] = autostrada.archi[autostrada.numeroArchi - 1];
                    autostrada.numeroArchi--;
                    autostrada.archi = realloc(autostrada.archi, autostrada.numeroArchi * sizeof(Arco));
                    i--; // Riduci l'indice per esaminare l'arco spostato
                }
            }
            // Liberazione della memoria delle autonomie dei veicoli
            free(autostrada.stazioni[indice].autoveicoli);

            return 1;
        } else {
            return 0;
        }
    }

// Funzione per aggiungere un arco al grafo
    void aggiungiArco(int partenza, int destinazione, int peso) {
        autostrada.numeroArchi++;
        autostrada.archi = realloc(autostrada.archi, autostrada.numeroArchi * sizeof(Arco));
        autostrada.archi[autostrada.numeroArchi - 1].partenza = partenza;
        autostrada.archi[autostrada.numeroArchi - 1].destinazione = destinazione;
        autostrada.archi[autostrada.numeroArchi - 1].peso = peso;
    }

// Funzione per rimuovere un arco dal grafo
    int rimuoviArco(int da, int a) {
        int indice = -1;

        // Trova l'indice dell'arco da rimuovere
        for (int i = 0; i < autostrada.numeroArchi; i++) {
            if (autostrada.archi[i].partenza == da && autostrada.archi[i].destinazione == a) {
                indice = i;
                break;
            }
        }

        // Se l'indice è valido, rimuovi l'arco
        if (indice != -1) {
            // Sposta l'ultimo arco nell'indice da rimuovere
            autostrada.archi[indice] = autostrada.archi[autostrada.numeroArchi - 1];
            autostrada.numeroArchi--;
            autostrada.archi = realloc(autostrada.archi, autostrada.numeroArchi * sizeof(Arco));
            return 1;
        } else {
            return 0;
        }
    }
// Funzione per aggiungere un'auto a una stazione di servizio
    int aggiungiAuto(int distanza, int autonomia) {
        int indice = -1;

        // Trova l'indice della stazione in cui aggiungere l'auto
        for (int i = 0; i < autostrada.numeroStazioni; i++) {
            if (autostrada.stazioni[i].distanza == distanza) {
                indice = i;
                break;
            }
        }

        // Se l'indice è valido, aggiungi l'auto
        if (indice != -1) {
            // Verifica se l'autonomia esiste già
            for (int i = 0; i < autostrada.stazioni[indice].numeroAuto; i++) {
                if (autostrada.stazioni[indice].autoveicoli[i] == autonomia) {
                    printf("Auto già presente\n");
                    return 0;
                }
            }

            // Aggiunta dell'auto
            autostrada.stazioni[indice].numeroAuto++;
            autostrada.stazioni[indice].autoveicoli = realloc(autostrada.stazioni[indice].autoveicoli, autostrada.stazioni[indice].numeroAuto * sizeof(int));
            autostrada.stazioni[indice].autoveicoli[autostrada.stazioni[indice].numeroAuto - 1] = autonomia;

            // Aggiunta degli archi per le stazioni raggiungibili
            for (int i = 0; i < autostrada.numeroStazioni; i++) {
                if (i != indice) {

                    int autonomia_massima_nuova = trovaAutonomiaMassima(distanza);

                    // Verifica se la stazione con l'auto appena aggiunta può raggiungere la stazione i
                    // e se non esiste già un arco tra le due stazioni
                    if (autonomia_massima_nuova >= abs(autostrada.stazioni[i].distanza - distanza) && !esisteArco(indice, i)) {
                        aggiungiArco(indice, i, abs(autostrada.stazioni[i].distanza - distanza));
                        printf("Da stazione %d a stazione %d arco aggiunto\n", indice, i);
                    }
                }
            }
            return 1;
        } else {
            return 0;
        }
    }

// Funzione per rimuovere un'auto da una stazione di servizio
    int rimuoviAuto(int distanza, int autonomia) {
        int indice_stazione = -1;
        int indice_auto = -1;

        // Trova l'indice della stazione in cui rimuovere l'auto
        for (int i = 0; i < autostrada.numeroStazioni; i++) {
            if (autostrada.stazioni[i].distanza == distanza) {
                indice_stazione = i;
                break;
            }
        }

        // Se l'indice della stazione è valido, trova l'indice dell'auto da rimuovere
        if (indice_stazione != -1) {
            for (int i = 0; i < autostrada.stazioni[indice_stazione].numeroAuto; i++) {
                if (autostrada.stazioni[indice_stazione].autoveicoli[i] == autonomia) {
                    indice_auto = i;
                    //rimuove il primo incontrato con il valore corrispondente
                    break;
                }
            }

            // Se l'indice dell'auto è valido, rimuovi l'auto
            if (indice_auto != -1) {
                // Sposta l'ultima auto nell'indice da rimuovere
                autostrada.stazioni[indice_stazione].autoveicoli[indice_auto] = autostrada.stazioni[indice_stazione].autoveicoli[autostrada.stazioni[indice_stazione].numeroAuto - 1];
                autostrada.stazioni[indice_stazione].numeroAuto--;
                autostrada.stazioni[indice_stazione].autoveicoli = realloc(autostrada.stazioni[indice_stazione].autoveicoli, autostrada.stazioni[indice_stazione].numeroAuto * sizeof(int));

                // Rimuovi gli archi che non sono più raggiungibili
                int autonomia_massima= trovaAutonomiaMassima(distanza);
                for(int i = 0; i < autostrada.numeroArchi; i ++){
                    if(autostrada.archi[i].partenza == indice_stazione){
                        if(autonomia_massima<abs(autostrada.archi[i].partenza-autostrada.archi[i].destinazione) || autostrada.stazioni[indice_stazione].numeroAuto==0){
                            printf("trovato arco da rimuovere\n");
                            rimuoviArco(autostrada.archi[i].partenza,autostrada.archi[i].destinazione);
                        }
                    }
                }

                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }
// Funzione per trovare l'autonomia massima in una stazione di servizio
    int trovaAutonomiaMassima(int distanza) {
        int indice_stazione = -1;
        int autonomia_massima = 0;

        // Trova l'indice della stazione
        for (int i = 0; i < autostrada.numeroStazioni; i++) {
            if (autostrada.stazioni[i].distanza == distanza) {
                indice_stazione = i;
                break;
            }
        }

        // Se l'indice è valido, trova l'autonomia massima
        if (indice_stazione != -1) {
            for (int i = 0; i < autostrada.stazioni[indice_stazione].numeroAuto; i++) {
                if (autostrada.stazioni[indice_stazione].autoveicoli[i] > autonomia_massima) {
                    autonomia_massima = autostrada.stazioni[indice_stazione].autoveicoli[i];
                }
            }
        }

        return autonomia_massima;
    }
// Funzione per stampare le informazioni sulle stazioni di servizio
    void stampaStazioni() {
        printf("Stazioni di servizio:\n");
        for (int i = 0; i < autostrada.numeroStazioni; i++) {
            printf("Stazione %d - Distanza: %d km\n", i, autostrada.stazioni[i].distanza);
            printf("   Autonomie dei veicoli: ");
            for (int j = 0; j < autostrada.stazioni[i].numeroAuto; j++) {
                printf("%d ", autostrada.stazioni[i].autoveicoli[j]);
            }
            printf("\n");
            printf("   Numero di veicoli: %d\n", autostrada.stazioni[i].numeroAuto);
            printf("   Stazioni collegate:\n");
            for (int j = 0; j < autostrada.numeroArchi; j++) {
                if (autostrada.archi[j].partenza == i) {
                    printf("      Stazione %d - Distanza: %d km, Peso: %d km\n", autostrada.archi[j].destinazione, autostrada.stazioni[autostrada.archi[j].destinazione].distanza, autostrada.archi[j].peso);
                }
            }
            printf("\n");
        }
    }
// Funzione per stampare le informazioni sugli archi
    void stampaArchi() {
        printf("Archi:\n");
        for (int i = 0; i < autostrada.numeroArchi; i++) {
            int partenza = autostrada.stazioni[autostrada.archi[i].partenza].distanza;
            int destinazione = autostrada.stazioni[autostrada.archi[i].destinazione].distanza;
            printf("Partenza: %d - Destinazione: %d - Peso: %d\n", partenza, destinazione, autostrada.archi[i].peso);
            //printf("Partenza: %d - Destinazione: %d - Peso: %d\n", autostrada.archi[i].partenza, autostrada.archi[i].destinazione, autostrada.archi[i].peso);
        }
    }

// Funzione per verificare l'esistenza di un arco nel grafo
    int esisteArco(int partenza, int destinazione) {
        for (int i = 0; i < autostrada.numeroArchi; i++) {
            if (autostrada.archi[i].partenza == partenza && autostrada.archi[i].destinazione == destinazione) {
                return 1; // L'arco esiste
            }
        }
        return 0; // L'arco non esiste
    }

    int trovaIndiceStazione(int distanza) {
        for (int i = 0; i < autostrada.numeroStazioni; i++) {
            if (autostrada.stazioni[i].distanza == distanza) {
                return i;
            }
        }
        return -1; // 如果未找到节点，返回-1
    }

    void inizializzaDistanze(int* distanze, int* predecessori, int n) {
        for (int i = 0; i < n; i++) {
            distanze[i] = INT_MAX;  // 初始化为无穷大距离
            predecessori[i] = -1;   // 初始化为无前驱节点
        }
    }

    void dijkstra(int stazione_partenza, int stazione_destinazione) {
        int n = autostrada.numeroStazioni;
        int* distanze = malloc(n * sizeof(int));
        int* predecessori = malloc(n * sizeof(int));
        int* visitati = malloc(n * sizeof(int));

        inizializzaDistanze(distanze, predecessori, n);

        distanze[stazione_partenza] = 0;

        //inizializza memoria dei nodi visitati
        memset(visitati, 0, n * sizeof(int));

        for (int count = 0; count < n; count++) {
            int u = -1;
            for (int i = 0; i < n; i++) {
                if (!visitati[i] && (u == -1 || distanze[i] < distanze[u])) {
                    u = i;
                }
            }

            visitati[u] = 1;

            for (int i = 0; i < autostrada.numeroArchi; i++) {
                if (autostrada.archi[i].partenza == u) {
                    int v = autostrada.archi[i].destinazione;
                    int peso = autostrada.archi[i].peso;

                    if (!visitati[v] && distanze[u] != INT_MAX && distanze[u] + peso < distanze[v]) {
                        distanze[v] = distanze[u] + peso;
                        predecessori[v] = u;
                    }
                }
            }
        }

        if (distanze[stazione_destinazione] != INT_MAX) {
            printf("distanza minima：%d\n", distanze[stazione_destinazione]);
            printf("percorso più corto：");

            // 存储路径的临时数组
            int* tempPath = malloc(n * sizeof(int));
            int pathLength = 0;

            int currentNode = stazione_destinazione;
            while (currentNode != -1) {
                tempPath[pathLength++] = autostrada.stazioni[currentNode].distanza;
                currentNode = predecessori[currentNode];
            }

            // stampare in ordine inverso
            for (int i = pathLength - 1; i >= 0; i--) {
                printf("%d ", tempPath[i]);
            }
            printf("\n");
            free(tempPath);
        } else {
            printf("***Non raggiungibile***\n");
        }

        free(distanze);
        free(predecessori);
        free(visitati);
    }
