/*
** Nome: Michael
** Cognome: Tranchino
** Gruppo: Tranchino-Vitali
** Data: 11/01/2024
*/

#include "stdio.h" // libreria base dell'input output
#include "unistd.h" // interfaccia sistema linux
#include "sys/types.h" // per il tipo Pid_t che gestisce la tipologia dei pid
#include <sys/wait.h> // per la funzione wait
#include <stdbool.h> // Aggiunto l'inclusione per il tipo di dato bool

/*
Scrivere un programma C che generi due processi concorrenti A e B:

• A dovrà creare un file di testo a.txt riempiendolo con i primi 20 numeri di Fibonacci • B dovrà creare un file di testo b.txt riempiendolo con i primi 20 numeri primi
Il processo padre, al termine dell’esecuzione dei figli dovrà fondere i contenuti generati dai figli in un unico
file c.txt. Nota: i numeri nei file dovranno essere memorizzati uno per riga.
*/

// Funzione per generare i primi 20 numeri di Fibonacci e scriverli su un file
void fibonacci(FILE *file, int n) {
    int a = 0, b = 1, c;

    for (int i = 1; i <= n; ++i) {
        fprintf(file, "%d\n", a);
        c = a + b;
        a = b;
        b = c;
    }
}

// Funzione per verificare se un numero è primo
bool isPrime(int n) {
    if (n <= 1) {
        return false; // I numeri minori o uguali a 1 non sono primi
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false; // Se n è divisibile per i, allora non è primo
        }
    }
    return true; // Altrimenti, n è primo
}

// Funzione per calcolare i primi 20 numeri primi e scriverli su un file
void trovaNPrimi(int n, FILE *file) {
    int count = 0;
    int num = 2; // Inizia con il primo numero primo

    while (count < n) {
        if (isPrime(num)) {
            fprintf(file, "%d\n", num);
            ++count;
        }
        ++num;
    }
}

// Funzione per unire due file in un terzo file
void unisciFile(const char *inputFile1, const char *inputFile2, const char *outputFile) {
    FILE *file1, *file2, *resultFile;
    char ch;

    // Apri il primo file in modalità di lettura
    file1 = fopen(inputFile1, "r");
    if (file1 == NULL) {
        perror("Errore nell'apertura del primo file");
        _exit(3);
    }

    // Apri il secondo file in modalità di lettura
    file2 = fopen(inputFile2, "r");
    if (file2 == NULL) {
        perror("Errore nell'apertura del secondo file");
        fclose(file1);
        _exit(2);
    }

    // Apri il file di output in modalità di scrittura
    resultFile = fopen(outputFile, "w");
    if (resultFile == NULL) {
        perror("Errore nell'apertura del file di output");
        fclose(file1);
        fclose(file2);
        _exit(1);
    }

    // Copia il contenuto del primo file nel file di output
    while ((ch = fgetc(file1)) != EOF) {
        fputc(ch, resultFile);
    }

    // Aggiungi una riga vuota tra i contenuti dei due file
    fputc('\n', resultFile);

    // Copia il contenuto del secondo file nel file di output
    while ((ch = fgetc(file2)) != EOF) {
        fputc(ch, resultFile);
    }

    // Chiude tutti i file
    fclose(file1);
    fclose(file2);
    fclose(resultFile);
}

int main() {
    int waitStatus;
    pid_t pid, pid2;

    FILE *fileA = fopen("a.txt", "w+");
    FILE *fileB = fopen("b.txt", "w+");

    // Verifica la corretta apertura dei file
    if (fileA == NULL || fileB == NULL) {
        fprintf(stderr, "Errore nell'apertura dei file.\n");
        return 1;
    }

    pid = fork();
    if (pid > 0) {
        pid2 = fork();
        if (pid2 > 0) {
            for (int i = 0; i < 2; i++) {
                wait(&waitStatus);
            }
        } else {
            // Processo B (figlio)
            trovaNPrimi(20, fileB);
            fclose(fileB);
        }
    } else {
        // Processo A (figlio)
        fibonacci(fileA, 20);
        fclose(fileA);
    }

    if (pid > 0 && pid2 > 0) {
        // Processo padre
        unisciFile("a.txt", "b.txt", "c.txt");
    }

    return 0;
}
