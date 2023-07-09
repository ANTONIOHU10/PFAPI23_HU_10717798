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
     * 8. ogni stazione può anche non contenere veicoli
     * utilizzo struttura dati:
     * todo lista collegata per le stazioni
     * todo array dinamico per auto
     *
     */

    int main() {
        //file da leggere
        FILE *file = fopen("open_1.txt","r");

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

        //leggere una riga di comando
        while (fscanf(file,"%s",operazione) != EOF){

            if(strcmp(operazione,"aggiungi-stazione") == 0) {
                if(fscanf(file,"%d %d",&distanza, &numero_auto) != EOF){
                    for (int i = 0; i < numero_auto; i++){
                        if(fscanf(file,"%d",&autonomie[i]) != EOF){
                            //leggo una autonomia

                        }

                    }
                    //finisce di leggere le autonomie
                    fprintf(file_out,"(aggiungi stazione)  distanza: %d    numero_auto: %d\n",distanza,numero_auto);

                    for(int j = 0; j < numero_auto; j++){
                        fprintf(file_out,"autonomia: %d\n",autonomie[j]);
                    }

                }
            }

            if(strcmp(operazione,"demolisci-stazione") == 0) {
                if(fscanf(file,"%d",&distanza) != EOF){
                    fprintf(file_out,"(demolisci stazione)   distanza: %d\n",distanza);
                }
            }

            if(strcmp(operazione,"aggiungi-auto") == 0) {
                if(fscanf(file,"%d %d",&distanza, &autonomia) != EOF){
                    fprintf(file_out,"(aggiungi auto)   distanza: %d    autonomia: %d\n",distanza,autonomia);
                }
            }

            if(strcmp(operazione, "rottama-auto") == 0) {
                if(fscanf(file,"%d %d",&distanza,&autonomia) != EOF){
                    fprintf(file_out,"(rottama auto)   stazione: %d  autonomia: %d\n",distanza,autonomia);
                }
            }

            if(strcmp(operazione,"pianifica-percorso") == 0) {
                int distanza_destinazione;
                if(fscanf(file,"%d %d",&distanza,&distanza_destinazione) != EOF){
                    fprintf(file_out,"(pianifica percorso)   partenza: %d  destinazione: %d\n",distanza,distanza_destinazione);
                }
            }

        }
        fclose(file);
        fclose(file_out);
        return 0;
    }