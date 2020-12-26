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
	WSPutString(lp, "Clear[arg, log];");
	AnswerWSTP();				// Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
	WSPutString(lp, "arg[z_, s_ : -Pi] := Arg[z Exp[-I (s + Pi)]] + s + Pi;");
	AnswerWSTP();				// Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "log[z_, s_ : -Pi] := Log[Abs[z]] + I arg[z, s]");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "f[z_] := E^((p - 1) log[z, 0])/(z + 1)");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "Cont1[t_, eps_] := t + I eps");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "Cont2[t_, r_, th_] := r (Cos[t + th] + I Sin[t + th])");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "SommaDiRiemannL[range_, epsi_, n_] := (deltat = range/2^n; If[range > 0, t = 0, t = -range + deltat]; zb = Cont1[t, epsi]; Somma = 0;  For[k = 0, k < 2^n, k++, za = zb; t += deltat; zb = Cont1[t, epsi]; Somma += f[za]*(zb - za)]; Return[N[Somma]])");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "SommaDiRiemannC[range_, raggio_, fase_, n_] := ( deltat = range/2^n; t = 0; zb = Cont2[t, raggio, fase]; Somma = 0;  For[k = 0, k < 2^n, k++, za = zb; t += deltat; zb = Cont2[t, raggio, fase]; Somma += f[za]*(zb - za)]; Return[N[Somma]])");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "p = 0.99; l = 1000; epsi = 0.1; n = 16;");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "r = Sqrt[l^2 + epsi^2]; pha = ArcTan[epsi/l]; a = SommaDiRiemannL[l, epsi, n]; b = SommaDiRiemannC[2 Pi - 2 pha, r, pha, n]; c = SommaDiRiemannL[-l, -epsi, n]; d = SommaDiRiemannC[-Pi, epsi, 3 Pi/2, n];");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nValori approssimati da somma di Riemann:\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "a");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\na= %s",real_list[0]);
	if (real_list[1][0]=='-')
		printf("%s",real_list[1]);
	else
		printf("+%s",real_list[1]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "b");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nb= %s",real_list[0]);
	if (real_list[1][0]=='-')
		printf("%s",real_list[1]);
	else
		printf("+%s",real_list[1]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "c");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nc= %s",real_list[0]);
	if (real_list[1][0]=='-')
		printf("%s",real_list[1]);
	else
		printf("+%s",real_list[1]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "d");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nd= %s",real_list[0]);
	if (real_list[1][0]=='-')
		printf("%s",real_list[1]);
	else
		printf("+%s",real_list[1]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "a+b+c+d");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\na+b+c+d= %s",real_list[0]);
	if (real_list[1][0]=='-')
		printf("%s",real_list[1]);
	else
		printf("+%s",real_list[1]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "ar = NIntegrate[Re[f[Cont1[t, epsi]]], {t, 0, l}, AccuracyGoal -> 10]; ai = NIntegrate[Im[f[Cont1[t, epsi]]], {t, 0, l}, AccuracyGoal -> 10]; br = NIntegrate[ Re[f[Cont2[t, r, pha]]*D[Cont2[t, r, pha], t]], {t, 0, 2 Pi - 2 pha}, AccuracyGoal -> 10]; bi = NIntegrate[ Im[f[Cont2[t, r, pha]]*D[Cont2[t, r, pha], t]], {t, 0, 2 Pi - 2 pha}, AccuracyGoal -> 10]; cr = NIntegrate[Re[f[Cont1[t, -epsi]]], {t, l, 0}, AccuracyGoal -> 10]; ci = NIntegrate[Im[f[Cont1[t, -epsi]]], {t, l, 0}, AccuracyGoal -> 10]; dr = NIntegrate[ Re[f[Cont2[t, epsi, Pi/2]]*D[Cont2[t, epsi, Pi/2], t]], {t, Pi, 0}, AccuracyGoal -> 10]; di = NIntegrate[ Im[f[Cont2[t, epsi, Pi/2]]*D[Cont2[t, epsi, Pi/2], t]], {t, Pi, 0}, AccuracyGoal -> 10];");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nValori teorici:\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "ar");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\na= %s",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "ai");
        AnswerWSTP();                           // Attesa e parsing risposta

	if (real_list[0][0]=='-')
		printf("%s",real_list[0]);
	else
		printf("+%s",real_list[0]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "br");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nb= %s",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "bi");
        AnswerWSTP();                           // Attesa e parsing risposta

	if (real_list[0][0]=='-')
		printf("%s",real_list[0]);
	else
		printf("+%s",real_list[0]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "cr");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nc= %s",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "ci");
        AnswerWSTP();                           // Attesa e parsing risposta

	if (real_list[0][0]=='-')
		printf("%s",real_list[0]);
	else
		printf("+%s",real_list[0]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "dr");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\nd= %s",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "di");
        AnswerWSTP();                           // Attesa e parsing risposta

	if (real_list[0][0]=='-')
		printf("%s",real_list[0]);
	else
		printf("+%s",real_list[0]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "ar+br+cr+dr");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\na+b+c+d= %s",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "ai+bi+ci+di");
        AnswerWSTP();                           // Attesa e parsing risposta

	if (real_list[0][0]=='-')
		printf("%s",real_list[0]);
	else
		printf("+%s",real_list[0]);
	printf("i\n");

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "g1[z_, p_] := z^(p - 1)/(z + 1)");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "f1[z_] := g1[z,p]");
        AnswerWSTP();                           // Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
        WSPutString(lp, "N[2 Pi I Residue[f1[z], {z, -1}]]");
        AnswerWSTP();                           // Attesa e parsing risposta

	printf("\n(2 Pi) * Residuo in (-1) = %s",real_list[0]);
	if (real_list[1][0]=='-')
		printf("%s",real_list[1]);
	else
		printf("+%s",real_list[1]);
	printf("i\n");

	CloseWSTP();			// Chiude il collegamento col kernel di Mathematica

	return(0);			// Tutto Ok!
}
