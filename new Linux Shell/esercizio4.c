/*
** Nome: Michael
** Cognome: Tranchino
** Gruppo: Tranchino-Vitali
** Data: 12/01/2024
*/

#include <stdio.h> // Libreria base dell'input/output
#include <unistd.h> // Interfaccia sistema Linux
#include <stdlib.h> // Libreria standard di C
#include <sys/types.h> // Per il tipo Pid_t che gestisce la tipologia dei pid
#include <sys/wait.h> // Per la funzione wait
#include <string.h> // Per operazioni sulle stringhe


#define MAX_COMMAND_LENGTH 50 // Lunghezza massima del comando in caratteri
#define MAX_ARGUMENTS 10 // Massimo numero di argomenti

int main() {
    printf("Benvenuto nella mia shell Favolosa\n");
    printf("Type exit per uscire dalla shell :(\n");
    bool EXIT = false;
    while (EXIT == false ) 
    {
        printf(">>$");

        // Dichiarazione di un array di caratteri chiamato "input" con una dimensione massima MAX_COMMAND_LENGTH
        char input[MAX_COMMAND_LENGTH];

        // Utilizzo della funzione fgets per leggere una riga di input dalla tastiera (stdin)
        // e memorizzarla nell'array "input" con una dimensione massima di MAX_COMMAND_LENGTH
        fgets(input, sizeof(input), stdin);

        // La funzione strcspn restituisce la lunghezza della sottostringa in "input" 
        // che non contiene il carattere di newline ('\n'). 
        // Questo valore viene utilizzato come indice per modificare il carattere '\n' con '\0',
        // terminando così la stringa alla fine della riga.
        input[strcspn(input, "\n")] = '\0';


        // Se l'utente preme solo Enter, mostra di nuovo il prompt
        if (strlen(input) == 0) 
        {
            continue;
        }

        // Analizza il comando e gli argomenti
        // Utilizza la funzione strtok per suddividere la stringa di input in token
        // utilizzando gli spazi come "barriere". Il primo token (comando) viene restituito.
        char *command = strtok(input, " ");

        // Dichiarazione di un array di puntatori a stringa chiamato "argomenti" 
        // per contenere gli argomenti del comando.
        char *argomenti[MAX_ARGUMENTS];

        // Assegna il primo token (comando) all'indice 0 dell'array "argomenti".
        argomenti[0] = command;  // Il primo argomento è sempre il comando
        // Insrisco un comando custom in modo tale da poter uscire dalla shell
        if (strcmp(argomenti[0], "exit") == 0) 
        {
            printf("ciao ciao\n");
            EXIT = true;
            continue;  // Salta il resto del ciclo
        }

        // Inizializzazione di una variabile "argc" a 1, che tiene traccia del numero di argomenti.
        int argc = 1;

        // Dichiarazione di un puntatore a stringa chiamato "token" per l'utilizzo con strtok.
        char *token;

        // Ciclo while per estrarre gli argomenti dal resto della stringa di input
        // Utilizza la funzione strtok per estrarre i successivi token (argomenti) dalla stringa di input.
        // Continua finché ci sono token rimanenti e il numero di argomenti (argc) è inferiore a MAX_ARGUMENTS.
        while ((token = strtok(NULL, " ")) != NULL && argc < MAX_ARGUMENTS) 
        {
            // Assegna il token corrente all'indice "argc" dell'array "argomenti" 
            // e incrementa il contatore degli argomenti (argc).
            argomenti[argc++] = token;
        }

        argomenti[argc] = NULL;  // L'ultimo elemento dell'array deve essere NULL, così da sapere quando finiscono gli argomenti

        pid_t pid = fork();

        if (pid == 0) 
        {
            // Processo figlio
            char path[MAX_COMMAND_LENGTH + 5];  // 5 è la lunghezza di "/bin/"
            snprintf(path, sizeof(path), "/bin/%s", command);

            // Esegui il programma dalla cartella /bin/ con gli argomenti
            execvp(path, argomenti);

            // Se execvp ha avuto successo, questa riga non verrà mai raggiunta
            perror("Errore: ");
            exit(EXIT_FAILURE);
        } else if (pid < 0) 
        {
            // Errore nella creazione del processo figlio
            perror("Errore durante la creazione del processo figlio");
            exit(EXIT_FAILURE);
        } else 
        {
            // Processo padre
            int status;
            waitpid(pid, &status, 0);

            // Controlla se il processo figlio è terminato normalmente (non è stato interrotto da un segnale)
            // e se il codice di uscita del processo figlio è 127, il che indica spesso che il comando non è stato trovato.
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127) 
            {
            // Stampa un messaggio di errore indicando che il comando specificato non esiste.
            printf("ERRORE! Il comando %s non esiste...\n", command);
            }

        }
    }

    return 0;
}