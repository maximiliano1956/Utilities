#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wstp.h>				// Includes from Mathematica

#define MAXLIST	128				// Massimo valori in risposta dal kernel
#define MAXLEN	100000				// Massima lunghezza stringa valore intero convertito

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
			strncpy(str_list[indstr++],s,MAXLEN);	// Copia il simbolo nella lista str_list
			if (indstr>MAXLIST)		// Limitazione al valore MAXLIST
				indstr=MAXLIST;
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

	WSEndPacket(lp);				// Invia al kernel il comando:

	while (WSNextPacket(lp) != RETURNPKT) {		// Test se il prossimo pacchetto e' risposta a comando
		WSNewPacket(lp);			// Se no salta al pacchetto successivo
		if (WSError(lp))			// Se errore
			error();			// stampa messaggio ed forza uscita
	}

	ReadExpression(1);				// Parsing risposta
}

//
// Questa funzione calcola il gcd puntuale di 2 polinomi
//
int main(int argc, char *argv[]) {

	OpenWSTP();				// Apre il collegamento al Kernel di Mathematica

	WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "p = 0.33; f[x_] := x^(p - 1)/(x + 1)");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nDefining f(x) = x^(0.33-1)/(x+1)\n");

	WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "SommaDiRiemann[a_, b_, n_] := ( deltax = (b - a)/2^n; x = a; Somma = 0; For[k = 0, k < 2^n, k++, Somma += f[x]; x += deltax]; Somma *= deltax; Return[Somma])");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nCalculating improper integral of f[x] from 0 to Infinity by Riemann Sums aproximation...wait!\n");

	WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "SommaDiRiemann[0.0001, 10000, 24]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nSommaDiRiemann[0.0001, 10000, 24]= %s\n\n",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "NIntegrate[f[x], {x, 0, Infinity}]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nValore calcolato con l'integrale improprio = %s\n\n",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "Pi/Sin[p Pi]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nValore teorico (PI/Sin(0.33*PI))= %s\n\n",real_list[0]);

	CloseWSTP();			// Chiude il collegamento col kernel di Mathematica

	return(0);			// Tutto Ok!
}
