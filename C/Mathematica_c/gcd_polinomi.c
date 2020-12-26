#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wstp.h>				// Includes from Mathematica

#define MAXLIST	128				// Massimo valori in risposta dal kernel
#define MAXLEN	1024				// Massima lunghezza stringa valore intero convertito

WSENV env=NULL;					// Hook all'ambiente WSTP
WSLINK lp=NULL;					// Hook alle API WSTP

int indint;					// Indice in int_list
char int_list[MAXLIST+1][MAXLEN+1];		// Lista interi convertiti in stringa in output da Mathematica

int indreal;					// Indice in real_list
char real_list[MAXLIST+1][MAXLEN+1];		// Lista reali convertiti in stringa in output da Mathematica

int indstr;					// Indice in str_list
char str_list[MAXLIST+1][MAXLEN+1];		// Lista stringhe in output da Mathematica

//
// Funzione di errore da Mathematica o da programma
//
// Se errore la routine forza l'uscita dal programma ritornando il valore 1
//
void error(void) {

        if (WSError(lp))
		printf("Errore rilevato da WSTP: %s.\n",WSErrorMessage(lp));
	else
		printf("Errore rilevato dal programma.\n");
	exit(1);
}

//
// Inizializza WSTP (Wolfram System Transfer Protocol)
//
// OUTPUT:	env e lp come variabili globali
//
void OpenWSTP(void) {
	int err;	// Codice errore

	env=WSInitialize((WSEnvironmentParameter)0);	// Ottiene hook all'ambiente WSTP

	if (env==(WSENV)0) {				// Test se errore
		printf("Error in WSInitialize\n");	// Stampa errore
		exit(1);				// Forza uscita con valore 1
	}

	lp=WSOpenString(env,"-linkname 'math -mathlink' -linkmode launch", &err);	// Apre il collegamento con il kernel
											// di Mathematica. Imposta
											// il nome del collegamento e la modalita'
											// (shared-memory)

	if (lp==(WSLINK)0 || err!=WSEOK)		// Test errore
		error();				// Stampa errore e forza uscita con il valore 1
}

//
// Chiude il collegamento col kernel di Mathematica
//
void CloseWSTP(void) {

	WSClose(lp);					// Chiude collegamento API
	WSDeinitialize(env);				// Chiude collegamento ambiente
}

//
// Funzione di parsing dell'out di mathematica
// Tutti i valori interi in uscita vengono caricati
// come stringhe nella lista str_list
// Tutti i valori reali in uscita vengono caricati
// come stringhe nella lista real_list
//
// La routine e' ricorsiva
//
// INPUT:	0	no init
// 		1	init
//
void ReadExpression(int init) {

	const char *s;		// Address stringa in out
	int len;		// Numero argomenti in funzione rilevata

	if (init) {		// Test se resettare le liste in out
		indstr=0;
		indint=0;
		indreal=0;
	}

	switch (WSGetNext(lp)) {			// Passa al prossimo token

		case WSTKSYM:				// Simbolo
			WSGetSymbol(lp,&s);		// Carica l'address della stringa del simbolo in s
							// Non implementato: uso della stringa del simbolo in out
			WSReleaseSymbol(lp,s);		// Libera la memoria per la stringa in Mathematica
			break;

		case WSTKSTR:				// Stringa
			WSGetString(lp,&s);		// Carica l'address della stringa in s
			strncpy(str_list[indstr++],s,MAXLEN);	// Copia la stringa nella lista str_list
			if (indstr>MAXLIST)		// Limitazione al valore MAXLIST
				indstr=MAXLIST;
			WSReleaseString(lp,s);		// Libera la memoria per la stringa in Mathematica
			break;

		case WSTKINT:				// Intero
               		WSGetNumberAsString(lp,&s);	// Carica l'address della stringa dell'intero in s
			strncpy(int_list[indint++],s,MAXLEN);	// Copia la stringa dell'intero nella lista int_list
			if (indint>MAXLIST)		// Limitazione al valore MAXLIST
				indint=MAXLIST;
               		WSReleaseString(lp,s);		// Libera la memoria per la stringa in Mathematica
			break;

		case WSTKREAL:				// Reale
               		WSGetNumberAsString(lp,&s);	// Carica l'address della stringa del reale in s
			strncpy(real_list[indreal++],s,MAXLEN);	// Copia la stringa dell'intero nella lista real_list
			if (indreal>MAXLIST)		// Limitazione al valore MAXLIST
				indreal=MAXLIST;
               		WSReleaseString(lp,s);		// Libera la memoria per la stringa in Mathematica
			break;

		case WSTKFUNC:				// Funzione
			if (WSGetArgCount(lp,&len)==0)	// Carica il numero degli argomenti della funzione
				error();		// Se zero esce con errore
			else {
				ReadExpression(0);	// Ricorsivamente legge il nome della funzione

				for (int i=1;i<=len;i++)	// e i suoi len
					ReadExpression(0);	// argomenti
			}
			break;

		case WSTKERROR:				// Se errore
		default:				// o token non riconosciuto
			error();			// Esce con errore
	}
}

//
// Gestisce attesa risposta comando e parsing risposta
//
void AnswerWSTP(void) {

        WSEndPacket(lp);                                // Invia al kernel il comando:

        while (WSNextPacket(lp) != RETURNPKT) {         // Test se il prossimo pacchetto e' risposta a comando
                WSNewPacket(lp);                        // Se no salta al pacchetto successivo
                if (WSError(lp))                        // Se errore
                        error();                        // stampa messaggio ed forza uscita
        }

        ReadExpression(1);                              // Parsing risposta
}

//
// Questa funzione calcola il gcd puntuale di 2 polinomi
//
int main(int argc, char *argv[]) {

	int a,b;				// Polinomi del tipo:	x^a+b       (x+1)^a+b

	char numa[MAXLEN+1];			// Stringa per intero a
	char numb[MAXLEN+1];			// Stringa per intero b
	char resultant[MAXLEN+1];		// Stringa per intero resultant
	char fatt[MAXLEN+1];			// Stringa per intero fatt
	char n[MAXLEN+1];			// Stringa per intero n
	char result[MAXLEN+1];			// Stringa per intero result

	char p1[MAXLEN+1];			// Stringa polinomio p1
	char p2[MAXLEN+1];			// Stringa polinomio p2
	char p3[MAXLEN+1];			// Stringa equazione diofantina p3

	//
	// Per sicurezza termina tutte le stringhe
	//

	numa[MAXLEN]='\0';
	numb[MAXLEN]='\0';
	resultant[MAXLEN]='\0';
	fatt[MAXLEN]='\0';
	n[MAXLEN]='\0';
	result[MAXLEN]='\0';
	p1[MAXLEN]='\0';
	p2[MAXLEN]='\0';
	p3[MAXLEN]='\0';

	for (int i=0;i<MAXLIST+1;i++) {
		int_list[i][MAXLEN]='\0';
		real_list[i][MAXLEN]='\0';
		str_list[i][MAXLEN]='\0';
	}

	OpenWSTP();				// Apre il collegamento al Kernel di Mathematica

// (a=3 b=8) (a=17 b=9) (a=19 b=6) (a=29 b=14)

	a=17;
	b=9;					// Set parametri polinomio

	snprintf(numa,MAXLEN,"%d",a);
	snprintf(numb,MAXLEN,"%d",b);		// Conversione in stringhe

	strncpy(p1,"x^",MAXLEN);
	strncat(p1,numa,MAXLEN-strlen(p1));
	strncat(p1,"+",MAXLEN-strlen(p1));
	strncat(p1,numb,MAXLEN-strlen(p1));	// p1 = stringa primo polinomio

	strncpy(p2,"(x+1)^",MAXLEN);
	strncat(p2,numa,MAXLEN-strlen(p2));
	strncat(p2,"+",MAXLEN-strlen(p2));
	strncat(p2,numb,MAXLEN-strlen(p2));	// p2 = stringa secondo polinomio

	WSPutFunction(lp,"Resultant", 3);	
	  WSPutFunction(lp,"ToExpression", 1);
	    WSPutString(lp,p1);
	  WSPutFunction(lp,"ToExpression", 1);
	    WSPutString(lp,p2);
	    WSPutSymbol(lp,"x");

	AnswerWSTP();				// Attesa e parsing risposta

	if (indint==0)
		error();			// Se nessun valore ricevuto stampa messaggio errore e forza uscita

	strncpy(resultant,int_list[0],MAXLEN);	// Copia in resultant il valore del risultante convertito in stringa

	WSPutFunction(lp,"FactorInteger", 1);
  	  WSPutFunction(lp,"ToExpression", 1);
	    WSPutString(lp,resultant);

	AnswerWSTP();				// Attesa e parsing risposta

	if (indint==0 || indint%2==1)		// Se nessun valore ricevuto o ricevuto un numero di valori dispari
		error();			// stampa nessaggio d'errore e forza l'uscita

	for (int k=0; k<indint; k+=2) {		// Loop sulle coppie (primo,potenza) della scomposizione

		if (k==0)
			fatt[0]='\0';		// All'inizio svuota la stringa da stampare
		else
			strncat(fatt,"x",MAXLEN-strlen(fatt));	// Altrimenti accoda un altro fattore

		strncat(fatt,int_list[k],MAXLEN-strlen(fatt));	// Stampa il fattore primo

		if (strcmp(int_list[k+1],"1")) {		// e se la potenza non e' 1

			strncat(fatt,"^",MAXLEN-strlen(fatt));			// accoda "^"
			strncat(fatt,int_list[k+1],MAXLEN-strlen(fatt));	// e la potenza

		}
	}

	if (indint>2)							// Test numero fattori primi
		printf("\nResultant=%s=%s\n",resultant,fatt);
	else								// Se uno solo stampa che il risultante e' un numero primo!
		printf("\nResultant=%s   e' primo!\n",resultant);

	strncpy(p3,p1,MAXLEN);
	strncat(p3,"==",MAXLEN-strlen(p3));
	strncat(p3,resultant,MAXLEN-strlen(p3));
	strncat(p3,"*y && ",MAXLEN-strlen(p3));
	strncat(p3,p2,MAXLEN-strlen(p3));
	strncat(p3,"==",MAXLEN-strlen(p3));
	strncat(p3,resultant,MAXLEN-strlen(p3));
	strncat(p3,"*z",MAXLEN-strlen(p3));		// Carica in p3 equazione diofantea in x,y,z:
							// (x^a+b==resultant*y && (x+1)^a+b==resultant*z)

	WSPutFunction(lp,"Reduce", 3);
	  WSPutFunction(lp,"ToExpression", 1);
	    WSPutString(lp,p3);
	  WSPutFunction(lp,"ToExpression", 1);
	    WSPutString(lp,"{x,y,z}");
	  WSPutSymbol(lp,"Integers");

	AnswerWSTP();				// Attesa e parsing risposta

	if (indint<1)					// Test se valore ricevuto
		error();				// se nessun valore stampa messaggio errore e forza l'uscita

	strncpy(n,int_list[1],MAXLEN);			// Carica in il valore della soluzione x convertita in stringa

	strncpy(result,"gcd(",MAXLEN);
	strncat(result,n,MAXLEN-strlen(result));
	strncat(result,"^",MAXLEN-strlen(result));
	strncat(result,numa,MAXLEN-strlen(result));
	strncat(result,"+",MAXLEN-strlen(result));
	strncat(result,numb,MAXLEN-strlen(result));
	strncat(result,",(",MAXLEN-strlen(result));
	strncat(result,n,MAXLEN-strlen(result));
	strncat(result,"+1)^",MAXLEN-strlen(result));
	strncat(result,numa,MAXLEN-strlen(result));
	strncat(result,"+",MAXLEN-strlen(result));
	strncat(result,numb,MAXLEN-strlen(result));
	strncat(result,")=",MAXLEN-strlen(result));
	strncat(result,resultant,MAXLEN-strlen(result));	// carica in result la stringa:
								// "gcd(n^a+b,(n+1)^a+b)=resultant"

	printf("\n%s\n\n",result);				// Stampa il gcd

	CloseWSTP();			// Chiude il collegamento col kernel di Mathematica

	return(0);			// Tutto Ok!
}
