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

	int a,b;				// Polinomi del tipo:	x^a+b       (x+1)^a+b
	int valn;				// Esponente in serie
	int segno;				// Segno di Infinity

	float r_val;				// Stringa per argomento zeta (parte reale)
	float i_val;				// Stringa per argomento zeta (parte immaginaria)
	int nloop;				// Numero cicli calcolo zeta

	char numa[MAXLEN+1];			// Stringa per intero a
	char numb[MAXLEN+1];			// Stringa per intero b
	char numv[MAXLEN+1];			// Stringa per intero valn
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

	WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "f[x_]:=x+1");
	AnswerWSTP();				// Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
	  WSPutString(lp, "f[3]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\n3+1 = %s\n",int_list[0]);

	// Calcola la serie da 0 a infinito di elemento 1/n^2
	// N[Sum[Power[Power[n, 2], -1], List[n, 1, Set[n, Infinity]]]] (si ottiene da Mathematica con FullForm[Hold[expr.]])

	WSPutFunction(lp,"N",1);
	  WSPutFunction(lp,"Sum",2);
	    WSPutFunction(lp,"Power",2);
	      WSPutFunction(lp,"Power",2);
	        WSPutSymbol(lp,"n");
	        WSPutInteger(lp,2);
	      WSPutInteger(lp,-1);
	    WSPutFunction(lp,"List",3);
	      WSPutSymbol(lp,"n");
	      WSPutInteger(lp,1);
	      WSPutSymbol(lp,"Infinity");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nSomma serie di elemento 1/n^2 = ");
	printf("%s\n\n",real_list[0]);

	WSPutFunction(lp,"Apply",2);
  	  WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp,"LCM");
  	  WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp,"Range[10000]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nLCM(1,2,...,99,10000) = %s\n\n",int_list[0]);

	strncat(int_list[0]," // Log",MAXLEN-strlen(int_list[0]));

	WSPutFunction(lp,"N",1);
  	    WSPutFunction(lp,"ToExpression",1);
  	      WSPutString(lp,int_list[0]);
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nLog(LCM(1,2,...,99,10000)) = %s\n\n",real_list[0]);


	WSPutFunction(lp,"ToExpression",1);
	  WSPutString(lp, "psi[x_]:=Total@MangoldtLambda[Range[Floor[x]]]");
	AnswerWSTP();				// Attesa e parsing risposta

	WSPutFunction(lp,"N",1);
          WSPutFunction(lp,"ToExpression",1);
            WSPutString(lp, "psi[10000]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nChebichev2(10000)=%s\n\n",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "ll[n_]:=N[Log[Apply[LCM,Range[n]]]/n]");
	AnswerWSTP();				// Attesa e parsing risposta

	WSPutFunction(lp,"ToExpression",1);
	  WSPutString(lp, "ll[2]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("\nChebichev2(2)/2=%s\n",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
	  WSPutString(lp, "ll[10]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("Chebichev2(10)/10=%s\n",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
	  WSPutString(lp, "ll[100]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("Chebichev2(100)/100=%s\n",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
	  WSPutString(lp, "ll[1000]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("Chebichev2(1000)/1000=%s\n",real_list[0]);

	WSPutFunction(lp,"ToExpression",1);
	  WSPutString(lp, "ll[10000]");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("Chebichev2(10000)/10000=%s\n\n",real_list[0]);

	// Calcola il limite del termine 1/x^valn per x->segno*infinito
	
	valn=3;			// Esponente intero di x
	snprintf(numv,MAXLEN,"%d",valn);

	segno=1;		// 1 per limite a +infinito o -1 per limite a -infinito

	WSPutFunction(lp,"CompoundExpression",2);
	  WSPutFunction(lp,"Set",2);
	    WSPutSymbol(lp,"n");
	    WSPutInteger(lp,valn);
	  WSPutFunction(lp,"Limit",2);
	    WSPutFunction(lp,"Power",2);
	      WSPutFunction(lp,"Power",2);
	        WSPutSymbol(lp,"x");
	        WSPutSymbol(lp,"n");
	      WSPutInteger(lp,-1);
	    WSPutFunction(lp,"Rule",2);
	      WSPutSymbol(lp,"x");
	        WSPutFunction(lp,"Times",2);
	          WSPutInteger(lp,segno);
	          WSPutSymbol(lp,"Infinity");
	AnswerWSTP();				// Attesa e parsing risposta

	printf("Limit(1/x^%s, x->",numv);
	if (segno==1)
		printf("Infinity) = ");
	else
		printf("-Infinity) = ");

	if (indstr>0)
		if (int_list[0][0]=='-')
			printf("-%s\n\n",str_list[0]);
		else
			printf("%s\n\n",str_list[0]);
	else {
		if (indint>0)
			printf("%s\n\n",int_list[0]);
		if (indreal>0)
			printf("%s\n\n",real_list[0]);
	}

	// Definisce la funzione z[s_]= funzione di Riemann (equivale a Zeta[] built-in
	
	WSPutFunction(lp,"SetDelayed",2);
	  WSPutFunction(lp,"z",1);
	    WSPutFunction(lp,"Pattern",2);
	      WSPutSymbol(lp,"s");
	      WSPutFunction(lp,"Blank",0);
	  WSPutFunction(lp,"Sum",2);
	    WSPutFunction(lp,"Power",2);
	      WSPutFunction(lp,"Power",2);
	        WSPutSymbol(lp,"n");
	        WSPutSymbol(lp,"s");
	      WSPutInteger(lp,-1);
	    WSPutFunction(lp,"List",3);
	      WSPutSymbol(lp,"n");
	      WSPutInteger(lp,1);
	      WSPutSymbol(lp,"Infinity");
	AnswerWSTP();				// Attesa e parsing risposta

	// Calcola z[2]

  	WSPutFunction(lp,"N",2);
	  WSPutFunction(lp,"ToExpression",1);
	    WSPutString(lp, "z[2]");
	  WSPutInteger(lp,10);			// 10 digit di precisione
	AnswerWSTP();				// Attesa e parsing risposta

	printf("z[2]=%s\n\n",real_list[0]);


	// SetDelayed[g[Pattern[s, Blank[]], Pattern[n, Blank[]]], 
 	// N[Times[Times[1, 
    	// Power[Plus[1, Times[-1, Power[2, Plus[1, Times[-1, s]]]]], -1]], 
   	// Sum[Times[Times[1, Power[Power[2, Plus[k, 1]], -1]], 
     	// Sum[Times[Power[-1, i], Binomial[k, i], 
       	// Power[Plus[i, 1], Times[-1, s]]], List[i, 0, k]]], 
    	// List[k, 0, n]]]]]
	  
        WSPutFunction(lp,"SetDelayed",2);
          WSPutFunction(lp,"newzeta",2);
            WSPutFunction(lp,"Pattern",2);
              WSPutSymbol(lp,"s");
              WSPutFunction(lp,"Blank",0);
            WSPutFunction(lp,"Pattern",2);
              WSPutSymbol(lp,"n");
              WSPutFunction(lp,"Blank",0);
          WSPutFunction(lp,"N",2);
	    WSPutFunction(lp,"Times",2);
              WSPutFunction(lp,"Power",2);
	        WSPutFunction(lp,"Plus",2);
	          WSPutInteger(lp,1);
	          WSPutFunction(lp,"Times",2);
                    WSPutInteger(lp,-1);
                    WSPutFunction(lp,"Power",2);
                      WSPutInteger(lp,2);
	              WSPutFunction(lp,"Plus",2);
	                WSPutInteger(lp,1);
	                WSPutFunction(lp,"Times",2);
                          WSPutInteger(lp,-1);
                          WSPutSymbol(lp,"s");
                WSPutInteger(lp,-1);
	      WSPutFunction(lp,"Sum",2);
	        WSPutFunction(lp,"Times",2);
                  WSPutFunction(lp,"Power",2);
                    WSPutFunction(lp,"Power",2);
                      WSPutInteger(lp,2);
                      WSPutFunction(lp,"Plus",2);
                        WSPutSymbol(lp,"k");
                        WSPutInteger(lp,1);
                    WSPutInteger(lp,-1);
	          WSPutFunction(lp,"Sum",2);
	            WSPutFunction(lp,"Times",3);
                      WSPutFunction(lp,"Power",2);
                        WSPutInteger(lp,-1);
                        WSPutSymbol(lp,"i");
                      WSPutFunction(lp,"Binomial",2);
                        WSPutSymbol(lp,"k");
                        WSPutSymbol(lp,"i");
                      WSPutFunction(lp,"Power",2);
                        WSPutFunction(lp,"Plus",2);
                          WSPutSymbol(lp,"i");
                          WSPutInteger(lp,1);
	                WSPutFunction(lp,"Times",2);
                          WSPutInteger(lp,-1);
                          WSPutSymbol(lp,"s");
	            WSPutFunction(lp,"List",3);
                      WSPutSymbol(lp,"i");
                      WSPutInteger(lp,0);
                      WSPutSymbol(lp,"k");
	        WSPutFunction(lp,"List",3);
                  WSPutSymbol(lp,"k");
                  WSPutInteger(lp,0);
                  WSPutSymbol(lp,"n");
	    WSPutInteger(lp,100);
	AnswerWSTP();				// Attesa e parsing risposta

	// Calcola z[]
	
	nloop=100;

	r_val=-1;
	i_val=0.5;

	snprintf(numa,MAXLEN,"%f",r_val);
	snprintf(numb,MAXLEN,"%f",i_val);
	strncat(numa,"+",MAXLEN-strlen(numa));
	strncat(numa,numb,MAXLEN-strlen(numa));
	strncat(numa,"I",MAXLEN-strlen(numa));

//	strcpy(numa,"ZetaZero[20]");
	
	for (int nt=0; nt<nloop; nt++) {

		strncpy(p1,"newzeta[",MAXLEN-strlen(p1));
		strncat(p1,numa,MAXLEN-strlen(p1));
		strncat(p1," , ",MAXLEN-strlen(p1));

		snprintf(numb,MAXLEN,"%d",nt);
		strncat(p1,numb,MAXLEN-strlen(p1));
		strncat(p1,"]",MAXLEN-strlen(p1));

  		WSPutFunction(lp,"N",1);
	  	  WSPutFunction(lp,"ToExpression",1);
	    	    WSPutString(lp, p1);
		AnswerWSTP();				// Attesa e parsing risposta

		printf("%s = %s + %s I\n", p1, real_list[0], real_list[1]);

	}

	printf("\n");

	strncpy(p1,"Zeta[",MAXLEN-strlen(p1));
	strncat(p1,numa,MAXLEN-strlen(p1));
	strncat(p1,"]",MAXLEN-strlen(p1));

  	WSPutFunction(lp,"N",1);
  	  WSPutFunction(lp,"ToExpression",1);
    	    WSPutString(lp, p1);
	AnswerWSTP();				// Attesa e parsing risposta

	printf("%s = %s + %s I\n\n", p1, real_list[0], real_list[1]);

	CloseWSTP();			// Chiude il collegamento col kernel di Mathematica

	return(0);			// Tutto Ok!
}
