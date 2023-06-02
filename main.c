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

    }

    return 0;
}
