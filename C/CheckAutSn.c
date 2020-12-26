#include <stdio.h>

#define	NMAX	8
#define	PMAX	10000
#define	CMAX	1000

#define MAXDIM  16
#define MAXISO  1024

unsigned int Perm(unsigned int n);
void Stampa(unsigned int nperm, unsigned int n);
void StampaCailey(unsigned int numperm,unsigned int ord);
void Sort(unsigned int nperm, unsigned int n);
unsigned int GenCailey(unsigned int nperm, unsigned int n);
unsigned int VerifyCailey(unsigned int ord);

void ScanPerm(unsigned int nperm);

unsigned int PermTable[PMAX][NMAX];
unsigned int CaileyTable[CMAX][CMAX];

unsigned int tabperm[MAXDIM];
unsigned int permiso[MAXISO][MAXDIM];
unsigned int niso;
unsigned int dim;

int main(int argc, char *argv[]) {

	unsigned int ord;
	unsigned int nperm[NMAX];
	unsigned int numperm;

	ord=3;

	numperm = Perm(ord);
	if (numperm == 0)
		return(1);
	Sort(numperm, ord);
	GenCailey(numperm, ord);
	if (VerifyCailey(numperm) != 0) {
		printf("\nCaileyTable errata!\n");
		return(1);
		}

	StampaCailey(numperm,ord);

	printf("Premi un tasto ");
	getchar();
	printf("\n");

	niso=0;
	dim=numperm;

	ScanPerm(0);

	printf("\nNr. automorfismi trovati = %d\n",niso);
	printf("\n");

	for (unsigned int ns=0;ns<niso;ns++)
	{
		printf("Automorfismo nr. %d\tPermutazione = ",ns+1);
		for (unsigned int nelp=0;nelp<dim;nelp++)
			printf("%d ",permiso[ns][nelp]+1);
		printf("\n");
	}

	printf("\n");

	return (0);
}

void Stampa(unsigned int nperm, unsigned int n) {

	printf("\nPerm(%d)=\n\n",n);

	for (unsigned int i = 0; i < nperm; i++) {
		for (unsigned int j = 0; j < n; j++) {
			printf("%d,", PermTable[i][j]);
		}
		printf("\n");
	}

	printf("\n");

}

void StampaCailey(unsigned int numperm,unsigned int ord) {

	printf("\nTabella di Cailey di S%d (%d elementi) =\n\n", ord,numperm);

	for (unsigned int i = 0; i < numperm; i++) {
		for (unsigned int j = 0; j < numperm; j++) {
			printf("%d", CaileyTable[i][j]);
			if (j<numperm-1)
				printf(",");
		}
		printf("\n");
	}

	printf("\n");

}

// Genera in PermTable tutte le permutazioni di n elementi (1,2, .. , n)
// n viene limitato al valore massimo NMAX
// Fornisce in output il numero di elementi generati (n!) (-1 se buffer PermTable di dimensioni non adatte)

unsigned int Perm(unsigned int n) {

	unsigned int indn;
	unsigned int indf;
	unsigned int appo;

	if (n <= 0)
		n = 1;
	if (n > NMAX)
		n = NMAX;

	for (unsigned int nn = 1; nn <= n; nn++) {

		if (nn == 1) {

			indn = 0;
			PermTable[indn++][0] = 1;
			continue;
		}

		indf = indn;

		for (unsigned int i = 0; i < indf; i++) {

			for (unsigned int sh = 0; sh < nn; sh++) {

				if (sh == 0) {

					for (unsigned int j = 0; j < nn; j++)
						PermTable[indn][j + 1] = PermTable[i][j];

					PermTable[indn][sh] = nn;
				}
				else {

					for (unsigned int j = 0; j < n; j++)
						PermTable[indn][j] = PermTable[indn - 1][j];

					appo = PermTable[indn][sh];
					PermTable[indn][sh] = PermTable[indn][sh - 1];
					PermTable[indn][sh - 1] = appo;
				}

				indn++;

				if (indn >= PMAX)
					return(0);
			}
		}

		for (unsigned int i = indf; i < indn; i++)
			for (int j = 0; j < nn; j++)
				PermTable[i - indf][j] = PermTable[i][j];

		indn -= indf;
	}

	return(indn);
}

// Ordina per valori crescenti la tabella PermTable
// nperm = numero elementi nella tabella da ordinare (= n!)
// n = numero elementi permutati

void Sort(unsigned int nperm, unsigned int n) {

	unsigned int swap;
	unsigned int appo;
	unsigned int fswap;

	if (nperm <= 1)
		return;

	do {
		swap = 0;

		for (unsigned int i = 0; i < nperm - 1; i++) {

			fswap = 0;

			for (unsigned int j = 0; j < n; j++) {

				if (fswap == 0) {
					if (PermTable[i][j] == PermTable[i + 1][j])
						continue;
					if (PermTable[i][j] < PermTable[i + 1][j])
						break;
				}

				appo = PermTable[i][j];
				PermTable[i][j] = PermTable[i + 1][j];
				PermTable[i + 1][j] = appo;

				fswap = 1;
				swap = 1;
			}
		}
	}
	while (swap);

}

// Genera la tabella Cayley (CaileyTable) del gruppo Sn
// nperm = numero elementi nella tabella da ordinare (= n!)
// n = numero elementi permutati
// Il numero degli elementi della tabella CaileyTable e' limitato al valore CMAX
// Restituisce  0 se tutto ok.
//				1 se CaileyTable di dimensioni insufficienti
//				2 se tabella PermTable non coerente

unsigned int GenCailey(unsigned int nperm, unsigned int n) {

	unsigned int perm[NMAX];
	unsigned int found;

	if (nperm > CMAX || n > NMAX)
		return(1);

	for (unsigned int i = 0; i < nperm; i++)
		for (unsigned int j = 0; j < nperm; j++) {

			for (unsigned int k = 0; k < n; k++)
				perm[k] = PermTable[j][PermTable[i][k] - 1];

			for (unsigned int k = 0; k < nperm; k++) {

				found = 1;

				for (unsigned int l = 0; l < n; l++) {

					if (PermTable[k][l] != perm[l]) {
						found = 0;
						break;
					}
				}

				if (found) {
					CaileyTable[i][j] = k;
					break;
				}
			}

			if (found == 0)
				return(2);
		}


	return(0);
}

// Verifica che la tabella Cayley sia effettivamente la tabella operativa di un gruppo
// In input ord e' l'ordine del gruppo
// Restituisce	0 se tutto ok
//				1 se non operazione non iniettiva (no quadrato latino)
//				2 se l'elemento inverso destro e sinistro non coincidono
//				3 se non e' soddisfatta la proprieta' associativa
//				4 se non esiste un elemento neutro coerente

unsigned int VerifyCailey(unsigned int ord) {

	if (ord < 2)
		return(0);

	// Latin Square

	for (unsigned int i = 0; i < ord; i++)
		for (unsigned int j = 0; j < ord - 1; j++)
			for (unsigned int k = j + 1; k < ord; k++)
				if (CaileyTable[i][j] == CaileyTable[i][k])
					return(1);

	for (unsigned int j = 0; j < ord; j++)
		for (unsigned int i = 0; i < ord - 1; i++)
			for (unsigned int k = i + 1; k < ord; k++)
				if (CaileyTable[i][j] == CaileyTable[k][j])
					return(1);

	// Controllo su elemento neutro (si suppone sia il primo simbolo (cioe' zero))

	for (unsigned int j = 0; j < ord; j++)				// Per l'elemento generico j controllo che e * j = j
		if (CaileyTable[0][j] != j)
			return(4);

	for (unsigned int i = 0; i < ord; i++)				// Per l'elemento generico i controllo che i * e = i
		if (CaileyTable[i][0] != i)
			return(4);

	// Elemento inverso

	for (unsigned int i = 0; i < ord; i++)
		for (unsigned int j = 0; j < ord; j++)
			if (CaileyTable[i][j] == 0) {

				if (CaileyTable[j][i] != 0)
					return(2);

				break;
			}

	// Proprieta' associativa

	for (unsigned int i = 0; i < ord; i++)
		for (unsigned int j = 0; j < ord; j++)
			for (unsigned int k = 0; k < ord; k++)
				if (CaileyTable[CaileyTable[i][j]][k] != CaileyTable[i][CaileyTable[j][k]])
					return(3);
	return(0);
}

void ScanPerm(unsigned int nperm)
	{
	unsigned int nel;
	unsigned int nelp;
	unsigned int np;
	unsigned int ind1;
	unsigned int nr;
	unsigned int nc;
	unsigned int esci;

	for (nel=0;nel<dim;nel++)
		{
		for (ind1=0;ind1<nperm;ind1++)
			if (tabperm[ind1]==nel)
				break;

		if (ind1<nperm)
			continue;

		tabperm[nperm]=nel;

		np=nperm+1;

		if (np==dim)		/* trovato */
			{
			esci=0;

			for (nr=0;nr<dim;nr++)
				if (esci==0)
					for (nc=0;nc<dim;nc++)
						if (CaileyTable[tabperm[nr]][tabperm[nc]]!=tabperm[CaileyTable[nr][nc]])
						{
							esci=1;
							break;
						}

			if (esci==0)
			{
				for (nelp=0;nelp<dim;nelp++)
					permiso[niso][nelp]=tabperm[nelp];
				printf("Trovato isomorfismo nr=%d\n",++niso);
			}
			continue;
			}

		ScanPerm(np);			/* ricorsivita' */
		}
	}
