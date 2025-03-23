#include <stdio.h>

#define MAXN 16				// Numero massimo ordine del quadrato

#define VUOTO -1			// Indicatore locazione del quadrato
							// non assegnata

typedef struct {
	int greco;				// Simbolo quadrato greco (0..n-1)
	int latino;				// Simbolo quadrato latino (0..n-1)
	int usato;				// Indicatore se il simbolo e' stato
							// usato nel quadrato
} SIMBOLO;					// Tipo casella del quadrato

typedef struct {
	SIMBOLO Q[MAXN][MAXN];	// Quadrato
} QUADRATO;

typedef struct {
	int moves[MAXN*MAXN];	// Lista indici simboli ammessi
	int nm;					// Numero entry nella lista
} MOVESALLOWED;				// Lista simboli ammessi nella casella

void InitSearch(void);
void Search(int row, int column);
int Good(int row, int column, int p);
void Stampa(QUADRATO q);
MOVESALLOWED GenMovesAllowed(int row, int column);
void TakeMove(int *row, int *column, int ind);
void TakeBack(int *row, int *column, int ind);

const SIMBOLO Vuoto = { -1, -1, 0 };	// Costante che corrisponde
								// ad una casella vuota del quadrato

QUADRATO Quadrato;				// Istanza quadrato
SIMBOLO Pool[MAXN * MAXN];		// Istanza insieme di tutti i simboli
								// ammessi

int numsol;						// Numero soluzioni trovate

int n;							// Dimensione del quadrato



//
// Init ricerca
//
void InitSearch(void) {
	int i, j, ind;

	ind = 0;
	numsol = 0;					// Azzera il numero di soluzioni
								// trovate

//	Dichiara il quadrato vuoto e inizializza l'insieme dei simboli
//	ammessi

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			Quadrato.Q[i][j] = Vuoto;
			Pool[ind].greco = i;
			Pool[ind].latino = j;
			Pool[ind].usato = 0;
			ind++;
		}
}

//
// Effettua il deposito del simbolo nel quadrato
//
// INPUT:	row		riga deposito in quadrato
//			column	colonna deposito in quadrato
//			ind		indice simbolo da depositare
// OUTPUT:
//
// nota: in uscita row e column sono aggiornati alla
//       posizione seguente nel quadrato

void TakeMove(int *row, int *column, int ind) {

	Quadrato.Q[*row][*column] = Pool[ind];	// Deposita il simbolo
											// nel quadrato
	Pool[ind].usato = 1;	// Dichiara il simbolo utilizzato

	// Passa alla posizione successiva nel quadrato
	(*column)++;
	if (*column >= n) {		// Test cambio riga
		*column = 0;
		(*row)++;
	}
}

//
// Annulla il deposito del simbolo effettuato
//
// INPUT:	row		riga in quadrato da dichiarare di nuovo vuoto
//			column	colonna in quadrato da dichiarere di nuovo vuoto
//			ind		indice simbolo da ritenere di nuovo utilizzabile
// OUTPUT:
//
// nota: in uscita row e column sono aggiornati alla
//       posizione del quadrato resa di nuovo libera

void TakeBack(int *row, int *column, int ind) {

	// Torna alla posizione precedente nel quadrato
	(*column)--;
	if (*column < 0) {		// Test cambio riga
		*column = n - 1;
		(*row)--;
	}

	Quadrato.Q[*row][*column] = Vuoto;	// Dichiara la casella
										// di nuovo vuota
	Pool[ind].usato = 0;				// Dichiara il simbolo di
										// nuovo inutilizzato
}

//
// Routine RICORSIVA di esplorazione
// di tutti i possibili quadrati graeco-latini
//
//	INPUT:	r,c		coordinate casella del quadrato (riga,colonna)
//					del quale ricercare tutti i simboli ammessi

void Search(int r, int c) {
	MOVESALLOWED Moves;			// Lista simboli ammessi
	int ind;					// Indice simbolo
	int nm;						// Indice in lista simboli ammessi

	Moves=GenMovesAllowed(r,c);	// Genera la lista dei simboli ammessi
								// Nota: la lista viene passata
								// PER VALORE (ricorsivita'!)

	for (nm = 0; nm < Moves.nm; nm++) {	// Ora ogni simbolo ammesso
		ind = Moves.moves[nm];
		TakeMove(&r,&c,ind);			// viene depositato
										// nel quadrato (passando alla
										// locazione seguente)

		if (r == n) {					// test se ultimo simbolo
										// depositato
									Stampa(Quadrato);
		printf("Soluzione nr. %d\n", numsol + 1);	// e l'ordinale
		printf("\n");
			numsol++;					// Aggiorna nr. soluzioni
		}

		Search(r, c);					// Ricerca la nuova casella

		TakeBack(&r,&c,ind);			// Annulla la mossa e passa
										// al simbolo successivo
										// da depositare
	}
}

//
// Genera la lista dei simboli ammessi in una casella del quadrato
//
// INPUT:	row,column	coordinate (riga,colonna)
//						della casella da controllare
// OUTPUT:	lista delle caselle ammesse

MOVESALLOWED GenMovesAllowed(int row, int column) {
	int ind;

	MOVESALLOWED Moves;		// La lista di mosse da generare deve
							// essere locale e viene passata in uscita
							// PER VALORE (per la ricorsivita'!)

	Moves.nm = 0;			// Azzera inialmente il numero di caselle
							// presenti nella lista

	for (ind = 0; ind < n * n; ind++)	// Loop su tutti i simboli
		if (Pool[ind].usato == 0)		// non ancira utilizzati
			if (Good(row,column,ind))	// test se simbolo ammesso
				Moves.moves[Moves.nm++] = ind;	// Se si' carica
												// indice del simbolo
												// in lista
	return(Moves);						// Out PER VALORE della lista
}

//
// Test se un simbolo e' depositabile in una casella
//
// INPUT:	row,column	coordinate (riga,colonna) della casella
//						da controllare
//			p			indice del simbolo da depositare
//
// OUTPUT:	0			simbolo non ammesso
//			1			simbolo ammesso

int Good(int row, int column,int p) {
	int r, c;

	for (r = 0; r < row; r++)
		if ((Quadrato.Q[r][column].greco == Pool[p].greco) ||
			(Quadrato.Q[r][column].latino == Pool[p].latino))
			return(0);	// Un simbolo greco o latino e' gia' presente
						// nella colonna della casella al di sopra
						// di essa : il simbolo non e' ammesso

	for (c = 0; c < column; c++)
		if ((Quadrato.Q[row][c].greco == Pool[p].greco) ||
			(Quadrato.Q[row][c].latino == Pool[p].latino))
			return(0);	// Un simbolo greco o latino e' gia' presente
						// nella riga della casella alla sua sinistra:
						// il simbolo non e' ammesso

	return(1);		// Superati i test: il simbolo e' ammesso
}

//
// Stampa del quadrato graeco-latino dove una singola casella viene
// indicata con lettere e numeri
//
// INPUT:	q		il quadrato da stampare (puo' essere incompleto)
//
void Stampa(QUADRATO q) {
	int r, c;
	for (r = 0; r < n; r++) {		// Loop righe
		for (c = 0; c < n; c++) {	// Loop colonne
			if (q.Q[r][c].greco == Vuoto.greco ||
				q.Q[r][c].latino == Vuoto.latino)
				printf("*** ");		// Se casella vuota stampa ***
			else
				printf("%c%d ", q.Q[r][c].greco + 'a',
						q.Q[r][c].latino);
		}
		printf("\n");
	}
}

// Main

int main(int argc, char* argv[]) {
	int ns;

	n = 3;							// Dimensione del quadrato

// Controllo sul valore di n
	if (n <= 0)
		n = 1;						// Limta al minimo
	if (n >= MAXN)
		n = MAXN;					// Limita al massimo

	InitSearch();					// Init ricerca
	Search(0, 0);					// La ricerca parte dalla casella
									// relativa alla prima riga e alla
									// prima colonna

//	Stampa numero totale soluzioni trovate
	printf("\nNr. Quadrati graeco-latini = %d\n\n", numsol);

	return(0);
}
