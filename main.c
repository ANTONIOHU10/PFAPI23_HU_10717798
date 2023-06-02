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
int main() {

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
            int numeroAuto = atoi(token);
            printf("Numero auto: %d\n",numeroAuto);
            //ottengo la autonomia di ogni auto
            for(int i=0;i<numeroAuto;i++){
                //ottengo la targa
                token = strtok(NULL, separatore);
                int autonomia = atoi(token);
                printf("Autonomia: %d\n",autonomia);
            }

        }

        else if(strcmp(token,"demolisci-stazione") ==0 ){
            printf("+Caso rimuovi stazione\n");
            //ottengo la distanza stazione
            token = strtok(NULL, separatore);
            int distanza = atoi(token);
            printf("Distanza: %d\n",distanza);
        }
        else if(strcmp(token,"aggiungi-auto") ==0 ){
            printf("+Caso aggiungi auto\n");
            //ottengo distanza stazione(stazione)
            token = strtok(NULL, separatore);
            int targa = atoi(token);
            printf("distanza-stazione: %d\n",targa);
            //ottengo la autonomia
            token = strtok(NULL, separatore);
            int autonomia = atoi(token);
            printf("Autonomia: %d\n",autonomia);
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
