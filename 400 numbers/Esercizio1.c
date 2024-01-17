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


/*
Scrivere un programma C che stampi a schermo i numeri da 1 a 400. La stampa dei numeri dovrà essere
distribuita tra 4 processi concorrenti, secondo lo schema:
P1 -> 1,5,9,13, ...
P2 -> 2,6,10,14, ...
P3 -> 3,7,11,15,19, ...
P4 -> 4,8,12,16,20, ...
Non è necessario che la stampa sia ordinata. E' importante che ogni processo stampi il proprio PID prima di
ogni numero.
*/

int main() {
    pid_t pid1,pid2,pid3,pid4;
    int WaitStatus;
    pid1 = fork();
    if(pid1 > 0)
    {
        pid2 = fork();
        if(pid2 > 0)
        {
            pid3 = fork();
            if(pid3 >0 )
            {
                pid4 = fork();
                if(pid4 > 0)
                {
                    for(int i = 0; i <=4; i++)
                    {
                    wait(&WaitStatus);
                    }
                } // figlio 4
                else if (pid4 == 0)
                {   // stampo i numeri di p4
                    for(int i = 4; i <=400; i += 4)
                    {
                        printf("il pid e' %d numero: %d\n", getpid(),i);
                    }
                }
            } // figlio 3
            else if(pid3 == 0)
            {   // stampa i numeri di p3
                for(int i = 3; i <=400; i += 4)
                {
                    printf("il pid e' %d numero: %d \n", getpid(),i);
                }
            }
        } // figlio 2
        else if (pid2 == 0)
        {   // stampa i numeri di p2 
            for(int i = 2; i <=400; i += 4)
            {
                printf("il pid e' %d numero: %d \n", getpid(),i);
            }
        }
    } // figlio 1
    else if (pid1 == 0)
    {   // stampa i numeri di p1
       for(int i = 1; i <=400; i += 4)
       {
        printf("il pid e' %d numero: %d\n", getpid(),i);
       }
    }
     
    return 0;
}