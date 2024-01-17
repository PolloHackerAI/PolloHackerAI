/*
** Nome: Michael
** Cognome: Tranchino
** Gruppo: Tranchino-Vitali
** Data: 12/01/2024
*/

#include <stdio.h> // Libreria base dell'input output
#include <unistd.h> // Interfaccia sistema linux
#include <stdlib.h> // Per la funzione atoi 
#include <sys/types.h> // Per il tipo Pid_t che gestisce la tipologia dei pid
#include <sys/wait.h> // Per la funzione wait

/*
Scrivere un programma C che, preso un numero intero n come parametro dalla linea di comando, generi n
processi concorrenti ognuno dei quali dovrà stampare il proprio pid ed uscire.
*/

int main(int argc, char *argv[])
{
    if (argc != 2)
    {   // verifica se il numero di argomenti dalla riga di comando non è uguale a 2. Se questa condizione è vera.
        // significa che l'utente non ha fornito il numero corretto di argomenti. In questo caso
        // il programma stampa un messaggio di errore sul flusso di errore standard (stderr) utilizzando fprintf, ed esce restituendo un codice di uscita pari a 1.
        fprintf(stderr, "attenzione al numero dei processi:\nsintassi = %s <numeroProcessi>\n", argv[0]);
        return 1;
    }

    int numeroProcessi = atoi(argv[1]); // converte il carattere che contiene il numero di processi da spownare in int
    
    for (int i = 0; i < numeroProcessi; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            printf("e' appena nato %d dal padre %d\n", getpid(), getppid());
            return 0;
        }
        else if (pid < 0)
        {
            perror("fork"); // throwa un errore nella shell
            return 1;
        }
        else
        {
            // processo padre
            wait(NULL); // aspetta che il/i figlio/i finisca/no
        }
    }

    return 0;
}
