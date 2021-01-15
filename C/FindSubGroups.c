#include <stdio.h>
#include <stdlib.h>

#define NMAX	6
// NOTA: 720 e' 6!
#define MAXDIM	720

int dim;
int ind[MAXDIM];
int numg;
int numgn;

unsigned int Perm(unsigned int n);
void Stampa(unsigned int nperm, unsigned int n);
void StampaCailey(unsigned int numperm,unsigned int ord);
void Sort(unsigned int nperm, unsigned int n);
unsigned int GenCailey(unsigned int nperm, unsigned int n);
unsigned int VerifyCailey(unsigned int ord);

unsigned int PermTable[MAXDIM][NMAX];
unsigned int CaileyTable[MAXDIM][MAXDIM];


int checkEl(int n,int el[],int l) {

	if (l==1)
		return(1);

	for (int ind=0;ind<n;ind++)
		if (l==el[ind])
			return(1);
	return(0);
}

int checkSub(int n,int el[]) {

	// Test chiusura

	for (int ind1=0;ind1<n;ind1++)
		for (int ind2=0;ind2<n;ind2++)
			if (checkEl(n,el,CaileyTable[el[ind1]-1][el[ind2]-1])==0)
				return(0);

	// Test inverso

	for (int ind1=0;ind1<n;ind1++)
		for (int ind2=0;ind2<dim;ind2++)
			if (CaileyTable[el[ind1]-1][ind2]==1)
				if (checkEl(n,el,ind2+1)==0)
					return(0);

	return(1);
}

int checkNormal(int nn) {

	int inv;
	int conj;

	inv=0;

	for (int i=2;i<=dim;i++) {
		if (checkEl(nn,ind,i)==0) {
			for (int j=1;j<=dim;j++)
				if (CaileyTable[i-1][j-1]==1) {
					inv=j;
					break;
				}

			if (inv==0) {
				printf("Element of the group without inverse!\n");
				exit(1);
			}

			for (int k=0;k<nn;k++) {
				conj=CaileyTable[inv-1][CaileyTable[ind[k]-1][i-1]-1];
				if (checkEl(nn,ind,conj)==0)
					return(0);
			}
		}
	}

	return(1);
}

void Scan2(int nn,int n,int ii) {

	int result;
	int conj;

	if (n==nn) {
		result=checkSub(nn,ind);

		if (result) {
			printf("{1");
			for (int i=0;i<nn;i++)
				printf(",%d",ind[i]);

			result=checkNormal(nn);

			if (result==1) {
				printf("} is a normal subgroup\n");
				numgn++;
			}
			else {
				printf("} is a subgroup\n");
				numg++;
			}
		}
		return;
	}

	for (int i=ii;i<=dim;i++) {
		ind[n]=i;

		Scan2(nn,n+1,i+1);
	}
}

void Scan(int order) {

	numg=0;
	numgn=0;

	printf("Finding order %d subgroups of an order %d group\n",order,dim);

	if (order==1) {
		printf("{1} is a normal sungroup\n");
		numgn=1;
	}
	else if (order==dim) {
		printf("{1");
		for (int i=2;i<=dim;i++)
			printf(",%d",i);
		printf("} is a normal sungroup\n");
		numgn=1;
	}
	else if ((dim%order)==0)
		Scan2(order-1,0,2);

	if (numg>0) {
		if (numg==1)
			printf("Only 1 subgroup\n");
		else
			printf("Found %d subgroups\n",numg);
	}
	else
		printf("No subgroups\n");

	if (numgn>0) {
		if (numgn==1)
			printf("Only 1 normal subgroup\n");
		else
			printf("Found %d normal subgroups\n",numgn);
	}
	else
		printf("No normal subgroups\n");

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
// Fornisce in output il numero di elementi generati (n!) (0 se buffer PermTable di dimensioni non adatte)

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

				if (indn >= MAXDIM)
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
// Il numero degli elementi della tabella CaileyTable e' limitato al valore MAXDIM
// Restituisce  0 se tutto ok.
//				1 se CaileyTable di dimensioni insufficienti
//				2 se tabella PermTable non coerente

unsigned int GenCailey(unsigned int nperm, unsigned int n) {

	unsigned int perm[MAXDIM];
	unsigned int found;

	if (nperm > MAXDIM || n > NMAX)
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
					CaileyTable[i][j] = k+1;
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
		if (CaileyTable[0][j] != j+1)
			return(4);

	for (unsigned int i = 0; i < ord; i++)				// Per l'elemento generico i controllo che i * e = i
		if (CaileyTable[i][0] != i+1)
			return(4);

	// Elemento inverso

	for (unsigned int i = 0; i < ord; i++)
		for (unsigned int j = 0; j < ord; j++)
			if (CaileyTable[i][j] == 1) {

				if (CaileyTable[j][i] != 1)
					return(2);

				break;
			}

	// Proprieta' associativa

	for (unsigned int i = 0; i < ord; i++)
		for (unsigned int j = 0; j < ord; j++)
			for (unsigned int k = 0; k < ord; k++)
				if (CaileyTable[CaileyTable[i][j]-1][k] != CaileyTable[i][CaileyTable[j][k]-1])
					return(3);
	return(0);
}

int main(char argc,char *argv[]) {

	unsigned int ord;
	int outcod;
	int stot;
	int ng;
	int ngn;

	ord=4;

        dim = Perm(ord);
        if (dim==0) {
                printf("\nNumero permutazioni errato!\n");
                return(1);
	}

        Sort(dim, ord);

        if ((outcod=GenCailey(dim, ord))!=0) {
                printf("\nGenCailey error! (%d)\n",outcod);
                return(1);
                }
        if ((outcod=VerifyCailey(dim))!=0) {
                printf("\nCaileyTable errata! (%d)\n",outcod);
                return(1);
                }

        StampaCailey(dim,ord);

        printf("Premi un tasto ");
        getchar();
        printf("\n");

	ng=0;
	ngn=0;

	for (int nn=1;nn<=dim;nn++) {
		Scan(nn);

		ng+=numg;
		ngn+=numgn;
	}

	printf("%d subgroups in total (%d normal and %d non normal)\n",ng+ngn,ngn,ng);
        printf("\n");

	return(0);
}
