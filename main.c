#include <stdio.h>

#define	NMAX	8
#define	PMAX	10000
#define	CMAX	1000

int Perm(int n);
void Stampa(int nperm, int n);
void StampaCailey(int ord);
void Sort(int nperm, int n);
int GenCailey(int nperm, int n);
int VerifyCailey(int ord);
int GenSynthemes(void);
int GenPentads(void);
int GenInner(void);
int GenOuter(void);
void StampaAutS6(void);

int PermTable[PMAX][NMAX];
int CaileyTable[CMAX][CMAX];
int Syntheme[15][6];
int Pentad[6][5][6];
int Inner[720][720];
int Outer[720][720];
int AutS6[1440][720];
int indn;
int indf;
int appo;

int main(int argc, char *argv[]) {

	int nperm[NMAX];

	nperm[5] = Perm(6);
	if (nperm[5] < 0)
		return(1);
	Sort(nperm[5], 6);
	GenCailey(nperm[5], 6);
	if (VerifyCailey(nperm[5]) != 0) {
		printf("\nCaileyTable errata!\n");
		return(1);
		}
	GenSynthemes();
	GenPentads();
	GenInner();
	GenOuter();
	StampaAutS6();

	getchar();

	return (0);
}

void Stampa(int nperm, int n) {

	printf("\nPerm(%d)=\n\n",n);

	for (int i = 0; i < nperm; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d,", PermTable[i][j]);
		}
		printf("\n");
	}

	printf("\n");

}

void StampaCailey(int ord) {

	printf("\nOrd(%d)=\n\n", ord);

	for (int i = 0; i < ord; i++) {
		for (int j = 0; j < ord; j++) {
			printf("%d,", CaileyTable[i][j]);
		}
		printf("\n");
	}

	printf("\n");

}

// Genera in PermTable tutte le permutazioni di n elementi (1,2, .. , n)
// n viene limitato al valore massimo NMAX
// Fornisce in output il numero di elementi generati (n!) (-1 se buffer PermTable di dimensioni non adatte)

int Perm(int n) {

	if (n <= 0)
		n = 1;
	if (n > NMAX)
		n = NMAX;

	for (int nn = 1; nn <= n; nn++) {

		if (nn == 1) {

			indn = 0;
			PermTable[indn++][0] = 1;
			continue;
		}

		indf = indn;

		for (int i = 0; i < indf; i++) {

			for (int sh = 0; sh < nn; sh++) {

				if (sh == 0) {

					for (int j = 0; j < nn; j++)
						PermTable[indn][j + 1] = PermTable[i][j];

					PermTable[indn][sh] = nn;
				}
				else {

					for (int j = 0; j < n; j++)
						PermTable[indn][j] = PermTable[indn - 1][j];

					appo = PermTable[indn][sh];
					PermTable[indn][sh] = PermTable[indn][sh - 1];
					PermTable[indn][sh - 1] = appo;
				}

				indn++;

				if (indn >= PMAX)
					return(-1);
			}
		}

		for (int i = indf; i < indn; i++)
			for (int j = 0; j < nn; j++)
				PermTable[i - indf][j] = PermTable[i][j];

		indn -= indf;
	}

	return(indn);
}

// Ordina per valori crescenti la tabella PermTable
// nperm = numero elementi nella tabella da ordinare (= n!)
// n = numero elementi permutati

void Sort(int nperm, int n) {

	int swap;
	int appo;
	int fswap;

	if (nperm <= 1)
		return;

	do {
		swap = 0;

		for (int i = 0; i < nperm - 1; i++) {

			fswap = 0;

			for (int j = 0; j < n; j++) {

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

int GenCailey(int nperm, int n) {

	int perm[NMAX];
	int found;

	if (nperm > CMAX || n > NMAX)
		return(1);

	for (int i = 0; i < nperm; i++)
		for (int j = 0; j < nperm; j++) {

			for (int k = 0; k < n; k++)
				perm[k] = PermTable[j][PermTable[i][k] - 1];

			for (int k = 0; k < nperm; k++) {

				found = 1;

				for (int l = 0; l < n; l++) {

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

int VerifyCailey(int ord) {

	if (ord < 2)
		return(0);

	// Latin Square

	for (int i = 0; i < ord; i++)
		for (int j = 0; j < ord - 1; j++)
			for (int k = j + 1; k < ord; k++)
				if (CaileyTable[i][j] == CaileyTable[i][k])
					return(1);

	for (int j = 0; j < ord; j++)
		for (int i = 0; i < ord - 1; i++)
			for (int k = i + 1; k < ord; k++)
				if (CaileyTable[i][j] == CaileyTable[k][j])
					return(1);

	// Controllo su elemento neutro (si suppone sia il primo simbolo (cioe' zero))

	for (int j = 0; j < ord; j++)				// Per l'elemento generico j controllo che e * j = j
		if (CaileyTable[0][j] != j)
			return(4);

	for (int i = 0; i < ord; i++)				// Per l'elemento generico i controllo che i * e = i
		if (CaileyTable[i][0] != i)
			return(4);

	// Elemento inverso

	for (int i = 0; i < ord; i++)
		for (int j = 0; j < ord; j++)
			if (CaileyTable[i][j] == 0) {

				if (CaileyTable[j][i] != 0)
					return(2);

				break;
			}

	// Proprieta' associativa

	for (int i = 0; i < ord; i++)
		for (int j = 0; j < ord; j++)
			for (int k = 0; k < ord; k++)
				if (CaileyTable[CaileyTable[i][j]][k] != CaileyTable[i][CaileyTable[j][k]])
					return(3);
	return(0);
}

// Genera i 15 synthemes (gruppo simmetrico S6)
// Restituisce	0 se okay
//				1 se i synthemes trovati non sono 15

int GenSynthemes(void) {

	int perm[6];
	int nsynth;
	int j;
	int appo;
	int found;

	nsynth = 0;
	found = 0;

	for (int i = 0; i < 720; i++) {

		for (j = 0; j < nsynth; j++) {

			for (int k = 0; k < 6; k++)
				perm[k] = k + 1;

			appo = perm[PermTable[i][0] - 1];
			perm[PermTable[i][0] - 1] = perm[PermTable[i][1] - 1];
			perm[PermTable[i][1] - 1] = appo;

			appo = perm[PermTable[i][2] - 1];
			perm[PermTable[i][2] - 1] = perm[PermTable[i][3] - 1];
			perm[PermTable[i][3] - 1] = appo;

			appo = perm[PermTable[i][4] - 1];
			perm[PermTable[i][4] - 1] = perm[PermTable[i][5] - 1];
			perm[PermTable[i][5] - 1] = appo;

			appo = perm[Syntheme[j][0] - 1];
			perm[Syntheme[j][0] - 1] = perm[Syntheme[j][1] - 1];
			perm[Syntheme[j][1] - 1] = appo;

			appo = perm[Syntheme[j][2] - 1];
			perm[Syntheme[j][2] - 1] = perm[Syntheme[j][3] - 1];
			perm[Syntheme[j][3] - 1] = appo;

			appo = perm[Syntheme[j][4] - 1];
			perm[Syntheme[j][4] - 1] = perm[Syntheme[j][5] - 1];
			perm[Syntheme[j][5] - 1] = appo;

			found = 1;

			for (int k = 0; k < 6; k++)
				if (perm[k] != k + 1) {
					found = 0;
					break;
				}

			if (found == 1)
				break;
		}

		if (found == 0) {

			if (nsynth >= 15)
				return(1);

			for (int k = 0; k < 6; k++)
				Syntheme[nsynth][k] = PermTable[i][k];

			nsynth++;
		}
	}

	return((nsynth == 15) ? 0 : 1);
}

// Genera i 6 pentads (gruppo simmetrico S6)
// Restituisce	0 se okay
//				1 se i pentads non sono 6

int GenPentads(void) {

	int perm[6];
	int ind[5];
	int npent;
	int found;
	int Pentd[1][5][6];

	npent = 0;

	for (int i = 0; i < 11; i++) {
		for (int j = i + 1; j < 12; j++) {
			for (int k = j + 1; k < 13; k++) {
				for (int l = k + 1; l < 14; l++) {
					for (int m = l + 1; m < 15; m++) {

						ind[0] = i;
						ind[1] = j;
						ind[2] = k;
						ind[3] = l;
						ind[4] = m;

						for (int i1 = 0; i1 < 4; i1++) {
							for (int i2 = i1 + 1; i2 < 5; i2++) {

								for (int s = 0; s < 6; s++)
									perm[s] = s + 1;

								appo = perm[Syntheme[ind[i1]][0] - 1];
								perm[Syntheme[ind[i1]][0] - 1] = perm[Syntheme[ind[i1]][1] - 1];
								perm[Syntheme[ind[i1]][1] - 1] = appo;

								appo = perm[Syntheme[ind[i1]][2] - 1];
								perm[Syntheme[ind[i1]][2] - 1] = perm[Syntheme[ind[i1]][3] - 1];
								perm[Syntheme[ind[i1]][3] - 1] = appo;

								appo = perm[Syntheme[ind[i1]][4] - 1];
								perm[Syntheme[ind[i1]][4] - 1] = perm[Syntheme[ind[i1]][5] - 1];
								perm[Syntheme[ind[i1]][5] - 1] = appo;

								appo = perm[Syntheme[ind[i2]][0] - 1];
								perm[Syntheme[ind[i2]][0] - 1] = perm[Syntheme[ind[i2]][1] - 1];
								perm[Syntheme[ind[i2]][1] - 1] = appo;

								appo = perm[Syntheme[ind[i2]][2] - 1];
								perm[Syntheme[ind[i2]][2] - 1] = perm[Syntheme[ind[i2]][3] - 1];
								perm[Syntheme[ind[i2]][3] - 1] = appo;

								appo = perm[Syntheme[ind[i2]][4] - 1];
								perm[Syntheme[ind[i2]][4] - 1] = perm[Syntheme[ind[i2]][5] - 1];
								perm[Syntheme[ind[i2]][5] - 1] = appo;

								found = 1;

								for (int s = 0; s < 6; s++)
									if (perm[s] == s + 1) {
										found = 0;
										break;
									}

								if (found == 0)
									break;
							}

							if (found == 0)
								break;
						}

						if (found == 1) {

							found = 0;

							for (int s = 0; s < 5; s++)
								for (int t = 0; t < 6; t++)
									Pentd[0][s][t] = Syntheme[ind[s]][t];

							for (int np = 0; np < npent; np++) {
								for (int inds = 0; inds < 5; inds++) {

									for (int s = 0; s < 6; s++)
										perm[s] = s + 1;

									appo = perm[Pentd[0][inds][0] - 1];
									perm[Pentd[0][inds][0] - 1] = perm[Pentd[0][inds][1] - 1];
									perm[Pentd[0][inds][1] - 1] = appo;

									appo = perm[Pentd[0][inds][2] - 1];
									perm[Pentd[0][inds][2] - 1] = perm[Pentd[0][inds][3] - 1];
									perm[Pentd[0][inds][3] - 1] = appo;

									appo = perm[Pentd[0][inds][4] - 1];
									perm[Pentd[0][inds][4] - 1] = perm[Pentd[0][inds][5] - 1];
									perm[Pentd[0][inds][5] - 1] = appo;

									appo = perm[Pentad[0][inds][0] - 1];
									perm[Pentad[0][inds][0] - 1] = perm[Pentad[0][inds][1] - 1];
									perm[Pentad[0][inds][1] - 1] = appo;

									appo = perm[Pentad[0][inds][2] - 1];
									perm[Pentad[0][inds][2] - 1] = perm[Pentad[0][inds][3] - 1];
									perm[Pentad[0][inds][3] - 1] = appo;

									appo = perm[Pentad[0][inds][4] - 1];
									perm[Pentad[0][inds][4] - 1] = perm[Pentad[0][inds][5] - 1];
									perm[Pentad[0][inds][5] - 1] = appo;

									found = 1;

									for (int s = 0; s < 6; s++)
										if (perm[s] != s + 1) {
											found = 0;
											break;
										}

									if (found == 0)
										break;
								}

								if (found == 1)
									break;
							}

							if (found == 0) {

								if (npent >= 6)
									return(1);

								for (int inds = 0; inds < 5; inds++) {
									for (int s = 0; s < 6; s++) {
										Pentad[npent][inds][s] = Pentd[0][inds][s];
									}
								}

								npent++;
							}
						}
					}
				}
			}
		}
	}

	return((npent == 6) ? 0 : 1);
}

// Genera la tabella Inner dei 720 automorfismi interni (gruppo simmetrico S6)
// Restituisce	0 se okay
//				1 se la tabella Cayley e' incoerente
//				2 se una funzione trovata non e' biettiva
//				3 se le funzioni trovate non sono diversi tra di loro
//				4 se la funzione trovata non e' un morfismo

int GenInner(void) {

	int inv;
	int found;

	for (int i = 0; i < 720; i++) {

		inv = -1;

		for (int k = 0; k < 720; k++)
			if (CaileyTable[i][k] == 0) {
				inv = k;
				break;
			}

		if (inv == -1)
			return(1);

		for (int j = 0; j < 720; j++)
			Inner[i][j] = CaileyTable[CaileyTable[inv][j]][i];
	}

	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 719; j++)
			for (int k = j + 1; k < 720; k++)
				if (Inner[i][j] == Inner[i][k])
					return(2);					// no injective

	for (int i = 0; i < 719; i++)
		for (int j = i + 1; j < 720; j++) {

			found = 1;

			for (int k = 0; k < 720; k++)
				if (Inner[i][k] != Inner[j][k]) {

					found = 0;
					break;
				}
		}


	if (found ==1)
		return(3);					// 2 automorfismi uguali

	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 720; j++)
			for (int k = 0; k < 720; k++)
				if (Inner[i][CaileyTable[j][k]] != CaileyTable[Inner[i][j]][Inner[i][k]])
					return(4);		// No automorfismo

	return(0);
}

// Genera la tabella Outer dei 720 automorfismi esterni (gruppo simmetrico S6)
// Restituisce	0 se okay
//				1 se Pentads incoerenti
//				2 se le funzioni trovate non sono diversi tra di loro
//				3 se una funzione trovata non e' biettiva
//				4 se le funzioni trovate non sono diversi tra di loro
//				5 se la funzione trovata non e' un morfismo


int GenOuter(void) {

	int PermPentad[6][5][6];
	int perm1[6];
	int found;
	int appo;
	int Out[720];

	for (int j = 0; j < 720; j++) {
		for (int k = 0; k < 6; k++) {
			for (int l = 0; l < 5; l++) {
				for (int m = 0; m < 6; m++)
					PermPentad[k][l][m] = PermTable[j][Pentad[k][l][m] - 1];
			}
		}

		for (int n = 0; n < 6; n++) {

			int k, l1, l2;

			for (k = 0; k < 6; k++) {

				for (l1 = 0; l1 < 5; l1++) {

					for (l2 = 0; l2 < 5; l2++) {

						int perm[6];

						for (int s = 0; s < 6; s++)
							perm[s] = s + 1;

						appo = perm[PermPentad[n][l1][0] - 1];
						perm[PermPentad[n][l1][0] - 1] = perm[PermPentad[n][l1][1] - 1];
						perm[PermPentad[n][l1][1] - 1] = appo;

						appo = perm[PermPentad[n][l1][2] - 1];
						perm[PermPentad[n][l1][2] - 1] = perm[PermPentad[n][l1][3] - 1];
						perm[PermPentad[n][l1][3] - 1] = appo;

						appo = perm[PermPentad[n][l1][4] - 1];
						perm[PermPentad[n][l1][4] - 1] = perm[PermPentad[n][l1][5] - 1];
						perm[PermPentad[n][l1][5] - 1] = appo;

						appo = perm[Pentad[k][l2][0] - 1];
						perm[Pentad[k][l2][0] - 1] = perm[Pentad[k][l2][1] - 1];
						perm[Pentad[k][l2][1] - 1] = appo;

						appo = perm[Pentad[k][l2][2] - 1];
						perm[Pentad[k][l2][2] - 1] = perm[Pentad[k][l2][3] - 1];
						perm[Pentad[k][l2][3] - 1] = appo;

						appo = perm[Pentad[k][l2][4] - 1];
						perm[Pentad[k][l2][4] - 1] = perm[Pentad[k][l2][5] - 1];
						perm[Pentad[k][l2][5] - 1] = appo;

						int s;

						for (s = 0; s < 6; s++)
							if (perm[s] != s + 1)
								break;

						if (s == 6)
							break;
					}

					if (l2 == 5 )
						break;
				}

				if (l1 == 5)
					break;
			}

			if (k == 6)
				return(1);
			else
				perm1[n] = k;
		}


		for (int s = 0; s < 720; s++) {

			found = 1;

			for (int t = 0; t < 6; t++)
				if (perm1[t] != PermTable[s][t] - 1) {

					found = 0;
					break;
				}

			if (found == 1) {

				Out[j] = s;
				break;
			}
		}
	}

	for (int i = 0; i < 719; i++)
		for (int j = i + 1; j < 720; j++)
			if (Out[i] == Out[j])
				return(2);

	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 720; j++)
			if (Out[CaileyTable[i][j]] != CaileyTable[Out[i]][Out[j]])
				return(3);

	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 720; j++)
			Outer[i][j] = Out[Inner[i][j]];

	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 719; j++)
			for (int k = j + 1; k < 720; k++)
				if (Outer[i][j] == Outer[i][k])
					return(3);					// no injective

	for (int i = 0; i < 719; i++)
		for (int j = i + 1; j < 720; j++) {

			found = 1;

			for (int k = 0; k < 720; k++)
				if (Outer[i][k] != Outer[j][k]) {

					found = 0;
					break;
				}
		}


	if (found == 1)
		return(4);					// 2 automorfismi uguali

	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 720; j++)
			for (int k = 0; k < 720; k++)
				if (Outer[i][CaileyTable[j][k]] != CaileyTable[Outer[i][j]][Outer[i][k]])
					return(5);		// No automorfismo

	// Controllo finale


	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 720; j++)
			AutS6[i][j] = Inner[i][j];

	for (int i = 0; i < 720; i++)
		for (int j = 0; j < 720; j++)
			AutS6[i + 720][j] = Outer[i][j];


	for (int i = 0; i < 1440; i++)
		for (int j = 0; j < 719; j++)
			for (int k = j + 1; k < 720; k++)
				if (AutS6[i][j] == AutS6[i][k])
					return(5);					// no injective

	for (int i = 0; i < 1439; i++)
		for (int j = i + 1; j < 1440; j++) {

			found = 1;

			for (int k = 0; k < 720; k++)
				if (AutS6[i][k] != AutS6[j][k]) {

					found = 0;
					break;
				}
		}


	if (found == 1)
		return(4);					// 2 automorfismi uguali

	for (int i = 0; i < 1440; i++)
		for (int j = 0; j < 720; j++)
			for (int k = 0; k < 720; k++)
				if (AutS6[i][CaileyTable[j][k]] != CaileyTable[AutS6[i][j]][AutS6[i][k]])
					return(5);		// No automorfismo

	return(0);
}

void StampaAutS6(void) {

	printf("\n");

	for (int i = 0; i < 1440; i++) {

		printf("\n(%d) = ", i);

		for (int j = 0; j < 720; j++)

			printf("%d ", AutS6[i][j]);
	}

	printf("\n");
}
