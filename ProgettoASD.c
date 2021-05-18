// inclusione delle librerie
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//----------------------
// DEFINIZIONI STRUTTURE
//----------------------

// struttura albero binario (andrà utilizzata per la ricerca)
typedef struct albero
{
    int             ora;           // numero intero tra 0 e 24 -- andrà ad indicare l'orario delle rilevazioni
    double          potenza,       // valore medio di consumo di potenza in 'W' 
                    temperatura,   // valore medio della temperatura in '°C'
                    processi,      // valore medio di processi attivi 
                    memoria;       // quantità media di RAM richiesta
    char            nome_cpu[7];   // valore alfanumerico che caratterizza una determinata cpu
    struct  albero  *sx,           // puntatore al nodo sinistro dell'albero
                    *dx;           // puntatore al nodo destro dell'albero
}albero;

// struttura Array
typedef struct array
{
    int             ora;           // numero intero tra 0 e 24 -- andrà ad indicare l'orario delle rilevazioni
    double          potenza,       // valore medio di consumo di potenza in 'W' 
                    temperatura,   // valore medio della temperatura in '°C'
                    processi,      // valore medio di processi attivi 
                    memoria;       // quantità media di RAM richiesta
    char            nome_cpu[7];   // valore alfanumerico che caratterizza una determinata cpu
}Array;


//-----------------------
// DICHIARAZIONI FUNZIONI
//-----------------------

// funzione per l'apertura del file in lettura
int ApriFile(FILE **file);

// funzione che inserisce i valori
int Inserimento(albero **radice, albero *nodo);

// funzione che controlla se i valori tra i nodi sono uguali
int NodiUguali(albero *n1, albero *n2);

// funzione che controlla se un nodo è minore dell'altro
int NodoMinore(albero *n1, albero *n2);

// funzione che visita l'albero in modo simmetrico e stampa i nomi delle cpu
void StampaNomiAlbero(albero *nodo);

// funzione che cerca nell'albero e stampa i valori della cpu scelta
int CercaCpuAlbero(albero *radice, char nome_cpu[6], int *passi);

// funzione che permette all'utente di scegliere se effettuare altre ricerche 
int AltraRicerca();

// funzione che scorre l'array e stampa i nomi delle cpu
void StampaNomiArray(Array *array, int righe);

// funzione che effettua la ricerca nell'array e stampa i dati della cpu scelta
int CercaCpuArray(Array* array, int righe, char *nome_cpu, int *passi);

// funzione che verifica se il nome della cpu inserito è accettabile
void VerificaNome(char nome_cpu[7]);


//----------------------------
// DEFINIZIONE FUNZIONE 'main'
//----------------------------
int main (void)
{
    // dichiarazione variabili locali
    int     passi,              // contatore per i passi svolti dall'algoritmo
            opzione,            // variabile utilizzata per scegliere la struttura da utilizzare
            controllo,          // utilizzato per il comportamento in basa al tentavivo di apertura del file
            scelta,             // variabile per scegliere se si desidera effettuare un'altra ricerca
            esito = 0;          // variabile per verificare se l'acquisizione ha dato problemi
    char    nome_cpu[7];        // nome della cpu scelta dall'utente
    FILE    *dati = NULL;      
    
    printf ("\nQuale tipo di struttura si vuole utilizzare?\n1) Albero\n2) Array?\n");
    printf ("Digitare '1' o '2'\n");
    
    // aquisizione e validazione della scelta
    do
    {
        esito = scanf("%d", &opzione);
        if((esito == 0) || ((opzione != 1) && (opzione != 2)))
            printf("Scelta non consentita\nPerfavore scegliere tra '1'  e  '2'\n");
        while (getchar() != '\n'); 
    } while ((esito == 0) || ((opzione != 1) && (opzione != 2)));
    
    switch (opzione)
    {
    case 1: // situazione in cui è stato scelto l'albero

        printf("E' stato scelto l'albero\n");

        // dichiarazione variabili utilizzate solo in questo case
        albero *radice = NULL, // nodo radice
               *nodo   = NULL; // nodo per l'inserimento

        controllo = ApriFile(&dati);   // apro il file in lettura

        if(controllo == 1)   // caso in cui sia stato aperto corretamente il file
        {   
            do  // aquisizione dei dati dal file
            {    
                // creazione di un nuovo nodo
                nodo = (albero*)malloc(sizeof(albero));
                nodo -> dx = nodo -> sx = NULL;

                // lettura dei valori del nodo successivo
                esito = fscanf(dati, "%d  %s  %lf  %lf  %lf  %lf",
                               &(nodo -> ora),          (nodo -> nome_cpu), &(nodo -> potenza),
                               &(nodo -> temperatura), &(nodo -> processi), &(nodo -> memoria));
                if(esito == 6 )
                    if(!Inserimento(&radice, nodo))
                        printf("\nInserimento non riuscito");
            } while(esito != EOF);

            // finita l'aquisizione dei dati si procede con la chiusura del file
            fclose(dati);

            // visualizzazione dei nomi delle cpu
            printf("\nLista dei nomi delle cpu che sono state analizzate\n");
            
            StampaNomiAlbero(radice);       // stampa i nomi delle cpu contenuti nel file

            do
            {
                passi = 0;
                
                VerificaNome(nome_cpu);     // acquisizione del nome della cpu scelto

                // ricerca dei dati relativi alla cpu scelta
                if (!CercaCpuAlbero(radice, nome_cpu, &passi))
                    printf("\nNon e' stata trovata alcuna CPU, (attenzione inserire lettere maiuscole)\n");
                printf("\nI passi compiuti dall'algoritmo per trovare e stampare i dati della CPU richiesta: %d\n", passi);

                scelta = AltraRicerca();
                
            } while (scelta == 1);
        }
        else    // caso in cui non sia stato possibile aprire il file
            printf("\nNon e' stato possibile aprire corretamente il file...\nPerfavore controllare la sua esistenza e/o posizione\n");
        
        break; // END 'case 1'
    

    case 2: // situazione in cui è stato scelto l'array
        
        printf("E' stato scelto l'array\n");        

        // dichiarazione varibili utilizzate solamente in questo case
        int     ora,           // numero intero tra 0 e 24 -- andrà ad indicare l'orario delle rilevazioni
                righe = 0,         // contatore delle righe del file
                c = 0;         // contatore utilizzato per l'inserimento
        double  potenza,       // valore medio di consumo di potenza in 'W' 
                temperatura,   // valore medio della temperatura in '°C'
                processi,      // valore medio di processi attivi 
                memoria;       // quantità media di RAM richiesta

        Array *array;       // creazione di un nuovo array
        controllo = ApriFile(&dati);    // apertura del file

        if (controllo == 1)
        {
            // conteggio del numero di righe del file aperto
            do
            {
                esito = fscanf(dati, "%d  %s  %lf  %lf  %lf  %lf",
                               &ora,          nome_cpu,   &potenza,
                               &temperatura,  &processi,  &memoria);
                righe += 1;
            } while (esito != EOF);
        
            array = (Array*)malloc(righe*sizeof(Array));
        
            // chiudo e riapro il file per poterlo leggere di nuovo dall'inizio
            fclose(dati);
            ApriFile(&dati);
        
            // lettura e inserimento dei dati
            do
            {
                esito = fscanf(dati, "%d  %s  %lf  %lf  %lf  %lf",
                               &ora,          nome_cpu,   &potenza,
                               &temperatura,  &processi,  &memoria);

                array[c].ora         = ora;
                array[c].potenza     = potenza; 
                array[c].temperatura = temperatura;
                array[c].processi    = processi;
                array[c].memoria     = memoria;
                strcpy(array[c].nome_cpu, nome_cpu);
                c += 1;
            } while (esito != EOF);
        
            fclose(dati);   // chiusura del file
            printf("\nLista dei nomi delle cpu che sono state analizzate\n");
            
            StampaNomiArray(array, righe);      // stampa i nomi delle cpu contenuti nel file

            do  // acquisizione dei dati dal file
            {
                passi = 0;

                VerificaNome(nome_cpu);     // acquisizione del nome della cpu scelto
                
                // ricerca dei dati relativi alla cpu scelta
                if (!CercaCpuArray(array, righe, nome_cpu, &passi))
                    printf("\nNon e' stata trovata alcuna CPU, (attenzione inserire lettere maiuscole)\n");
                printf("\nI passi compiuti dall'algoritmo per trovare e stampare i dati della CPU richiesta: %d\n", passi);

                scelta = AltraRicerca();
                
            } while (scelta == 1);
        }
        else    // caso in cui non sia stato possibile aprire il file
            printf("\nNon e' stato possibile aprire corretamente il file...\nPerfavore controllare la sua esistenza e/o posizione\n");
        
        break;  // END 'case 2'
    }
    printf("\nFINE PROGRAMMA\n");
    return 0;
}// END 'main'


//---------------------
// DEFINIZIONI FUNZIONI
//---------------------

// funzione 'ApriFile'
int ApriFile(FILE **file)
{
    // apro il file in lettura
    *file = fopen("dati.txt", "r");
    // verifico la corretta apertura del file
    if(*file == NULL)
        return 0;
    else
        return 1;    
}// END 'ApriFile'


// funzione 'Inserimento'
int Inserimento(albero **radice, albero *nodo)
{
    // dichiarazione variabili locali
    int     esito;          // variabile di ritorno
    albero  *attuale,       // nodo attuale 
            *padre;         // nodo padre

    // scorrimento dell'albero per il posizionamento della foglia
    for(attuale = padre = *radice;
        ((attuale != NULL) && (!NodiUguali(attuale, nodo)));
        padre = attuale, attuale = NodoMinore(nodo, attuale) ? attuale -> sx : attuale -> dx);

    // verifica che non ci siano ripetizioni 
    if(attuale != NULL)
        esito = 0;
    else
    {          
        esito = 1;

        if(attuale == *radice)  // nel caso l'albero sia vuoto inserimento dell'elemento come radice
            *radice = nodo;
        else                    // altrimenti l'inserimento avviene a sinistra se l'elemento è minore altrimenti a destra
        { 
            if(NodoMinore(nodo, padre))
                padre -> sx = nodo;
            else
                padre -> dx = nodo;
        }
    }    
    return esito;
}// END 'Inserimento


// funzione 'NodiUguali'
int NodiUguali(albero *n1, albero *n2)
{
    // dichiarazione variabili locali
    int esito = 0;  // valore di ritorno

    // controllo se i valori tra i nodi sono uguali
    if((stricmp(n1 -> nome_cpu, n2 -> nome_cpu) == 0) && (n1 -> ora == n2 -> ora))
        esito = 1;
    return esito;
}// END 'NodiUguali'


// funzione 'NodoMinore'
int NodoMinore(albero *n1, albero *n2)
{
    //dichiarazione variabili locali
    int esito = 0;  // variabile di ritorno

    if(strcmp(n1 -> nome_cpu, n2 -> nome_cpu) < 0)
        esito = 1;
    return esito;
}// END 'NodoMinore'


// funzione 'StampaNomiAlbero'
void StampaNomiAlbero(albero *nodo)
{
    if(nodo != NULL)
    {
        // analizzo l'albero partendo dal sotto albero sinistro per poi procedere verso destra
        StampaNomiAlbero(nodo -> sx);
        if(nodo -> ora == 1)
            printf("\t%s\n", nodo -> nome_cpu);
        StampaNomiAlbero(nodo -> dx);
    }
}// END 'StampaNomiAlbero'


// funzione 'CercaCpuAlbero'
int CercaCpuAlbero(albero *radice, char nome_cpu[6], int *passi)
{
    // dichiarazione variabili locali
    albero  *attuale;      // nodo attuale
    int     esito = 0;     // variabile di ritorno

    // scorrimento dell'albero per trovare il nodo passato alla funzione
    for (attuale = radice;
        (attuale != NULL) && (strcmp(attuale -> nome_cpu, nome_cpu) != 0);
        attuale = (strcmp(nome_cpu, attuale -> nome_cpu) < 0) ? attuale -> sx: attuale -> dx)
    *passi += 1;

    // caso in cui abbia trovato una cpu con lo stesso nome
    if(attuale != NULL)
    {
        esito = 1;
        // stampa del nodo
        printf("%d\t\t%s\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",
                attuale -> ora,            attuale -> nome_cpu,   attuale -> potenza,
                attuale -> temperatura,    attuale -> processi,   attuale -> memoria);
        *passi += 1;

        // per trovare il nodo relativo all'ora successiva si procede con la ricerca nel sotto-albero desto
        CercaCpuAlbero(attuale -> dx, nome_cpu, passi);   
    }
    return esito;
}// END 'CercaCpuAlbero


// funzione 'AltraRicerca'
int AltraRicerca()
{
    // dichiarazione variabili locali
    int num,
        temp;       
    printf("Si disidera cercare un'altra CPU?\nPremere '1' per rispondere'si'\nPremere '2' per rispondere 'no'\n");
    do 
    {
        temp = scanf("%d", &num);
        if(((num <= 0) || (num > 2)) || (temp == 0))
            printf("\nRisposta non accettabile...\nPerfavore selezionare '1' o '2'\n");
        while (getchar() != '\n');
    }while(((num != 1) && (num != 2)) || (temp == 0));

    // restituisco il primo valore valido
    return num;
}// END 'AltraRicerca'


// funzione 'StampaNomiArray'
void StampaNomiArray(Array *array, int righe)
{
    for(int i = 0; i < righe; i++)
    {
        if(array[i].ora == 1)
            printf("\t%s\n", array[i].nome_cpu);
    }
}// END 'StampaNomiArray'


// funzione 'CercaCpuArray
int CercaCpuArray(Array* array, int righe, char *nome_cpu, int *passi)
{
    // dichiarazione variabili locali
    int esito = 0;      // variabile di ritorno
    
    for (int i = 0; i < righe; i++)
    {
        if (strcmp(nome_cpu, array[i].nome_cpu) == 0)
        {
            esito = 1;
            printf("%d\t\t%s\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n",
                   array[i].ora,           array[i].nome_cpu,  array[i].potenza,
                   array[i].temperatura,   array[i].processi,  array[i].memoria);
        }
    
        *passi += 1;
    }
    return esito;
}// END 'CercaCpuArray


// funzione 'VerificaNome'
void VerificaNome(char nome_cpu[7])
{
    // dichiarazione variabili locali
    int esito,
        lunghezza_nome;     // variabile utilizzata per controllare la lunghezza del nome della cpu inserito dall'utente
    
    // scelta della cpu da parte dell'utente
    printf("\nInserire il nome di una cpu sopra-visualizzata di cui si vogliono visualizzare le prestazioni\n");
    do
    {
        esito = (scanf("\n%s", nome_cpu));
        lunghezza_nome = strlen(nome_cpu);

        if((esito != 1) || (lunghezza_nome != 6))
            printf("\nErrore nell'aquisizione del nome della cpu scelta...\nControllare la lunghezza del nome inserito\n");
        while (getchar() != '\n');
    } while ((esito != 1) || (lunghezza_nome != 6));
}// END 'VerificaNome'