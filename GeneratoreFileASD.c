// inclusione delle librerie
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

//----------------------
// DEFINIZIONI STRUTTURE
//----------------------

// definizione della struttura array
typedef struct array
{
    char nome_cpu[7];    // stringa con il nome della cpu
}array; 

//-----------------------
// DICHIARAZIONI FUNZIONI
//-----------------------

// funzione di controllo valori interi
int ValidazioneCpu();

// funzione che aqusisce gli estremi per il calcolo randomico
double ValoreRandom (double min, double max);

// funzione che mi crea i nomi delle cpu
void GeneraNomi(int numero_cpu, char nome_cpu[7], array *lista_cpu);

// funzione che mi genera i dati e li inserisce nel file
void GeneraDati(int numero_cpu, FILE *dati, array *lista_cpu);


//----------------------------
// DEFINIZIONE FUNZIONE 'main'
//----------------------------
int main (void)
{
    // dichiarazione variabili locali
    int numero_cpu;     // numero di cpu 
    
    char nome_cpu[7];   // valore alfanumerico che caratterizza una determinata cpu

    array *lista_cpu;   // struttura che contine tutti i nomi delle cpu

    FILE *dati = NULL;  // puntatore al file

    srand(time(NULL));  // cambio il seme del rand ogni volta che eseguo il programma

    // acquisizione del numero di cpu da voler utilizzare
    numero_cpu = ValidazioneCpu();

    // allocazione della memoria in base al numero di cpu
    lista_cpu = (array *)malloc(numero_cpu*sizeof(array));

    // calcolo dei nomi per le varie cpu
    GeneraNomi(numero_cpu, nome_cpu, lista_cpu);

    // apertura del file per la scrittura e verifica della corretta apertura
    dati = fopen("dati.txt", "w");
    if (dati == NULL)
        printf("Errore nell'apertura del file 'dati'\nControllare l'esistenza e\\o la posizione del file\n");
    
    // se il file viene aperto correttamente si procede con il calcolo casuale dei valori e la scrittura di questi nel file
    else 
        GeneraDati (numero_cpu, dati, lista_cpu); 
    
    // chiusura del file
    fclose(dati);

    printf("END");

    return 0;
}// END 'main'

//---------------------
// DEFINIZIONI FUNZIONI
//---------------------

// funzione 'ValidazioneCpu'
int ValidazioneCpu()
{
    // dichiarazione variabili locali
    int num,
        temp;       
    
    printf("Inserire numero di cpu da analizzare: \n");
    // aquisizione e verifica valori
    do 
    {
        temp = scanf("%d", &num);
        if((num <= 0) || (temp == 0))
            printf("\nIl valore inserito non e' accettabile\nPerfavore inserire un numero naturale diverso da 0\n");
        while (getchar() != '\n');
    }while((num <= 0) || (temp == 0));

    // restituisco il primo valore valido
    return num;
}// END 'ValidazioneCpu'


// funzione 'ValoreRandom'
double ValoreRandom (double min, double max)
{
    // dichiarazione variabili locali
    double ritorno, // variabile di ritorno
           i = rand();

    // espressione per ottenere un numero randomico in un intervallo
    ritorno = (i/RAND_MAX)*(max-min) + min;

    return ritorno;       
}// END 'ValoreRandom'


// funzione 'GeneraNomi'
void GeneraNomi(int numero_cpu, char nome_cpu[7], array *lista_cpu)
{
    for( int i = 0; i < numero_cpu ; i++ )
    {        
        for( int j = 0; j < 6; j++)
        {
            // per fare ciò bisogna consultare la tabella ascii
            // per i primi 3 valori vengono inserite delle lettere maiuscole casuali
            if( j < 3)
                nome_cpu[j] = 65 + rand() % (90-65); 
            // per gli ultimi 3 valori vengono inseriti dei numeri tra 0 e 9 casuali
            else if(j == 6)
                nome_cpu[j] = 0;
            else
                nome_cpu[j] = 48 + rand() % (57-48);
        }

        // copio il valore ottenuto nella struttura
        strcpy(lista_cpu[i].nome_cpu, nome_cpu);
    }
}// END 'GeneraNomi'


// funzione 'GeneraDati'
void GeneraDati(int numero_cpu, FILE *dati, array *lista_cpu)
{
    // dichiarazione variabili locali
    int     ora;         // numero intero tra 0 e 24 -- andrà ad indicare l'orario delle rilevazioni
    double  potenza,     // valore medio di consumo di potenza in 'W'
            temperatura, // valore medio della temperatura in '°C'
            processi,    // valore medio di processi attivi 
            memoria;     // quantità media di RAM richiesta
    // calcolo dei dati per ogni ora e inserimento nel file
    for ( ora = 1; ora <= 24; ora++)
    {
        // calcolo dati casuali all'interno di un range dato
        for ( int i = 0; i < numero_cpu; i++)
        {
	        potenza     = ValoreRandom (5.00, 15.00);
			temperatura = ValoreRandom (30.00, 90.00);
			processi    = ValoreRandom (1.00, 40.00);
			memoria     = ValoreRandom (10.00, 2000.00);

            // inserimento dei dati nel file
            fprintf(dati, "%d\t%s\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",
                    ora, lista_cpu[i].nome_cpu, potenza, temperatura, processi, memoria);
        }
    }
}// END 'GeneraDati'