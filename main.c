#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

typedef enum { false, true } bool;

unsigned int grado(uint64_t dato);
void TestSingPol(uint64_t polmod, bool *fl_irr, bool *fl_prim, bool log, bool solop);
void TestPol(unsigned int grad,bool log, bool solop);

// Legenda:			polinomio				polinomio a coefficienti binari al massimo di grado 63


// Restituisce una stringa pari al formato binario del dato in ingresso

char *pBin(uint64_t x) {

	static char buff[64 + 1];

	memset(buff, '\0', sizeof(buff));			// blank string

	for (int nbit = grado(x); nbit >= 0; nbit--) {	// fill in array from right to left

		buff[nbit] = (x & 1) ? '1' : '0';		// determine bit

		x >>= UINT64_C(1);								// shift right 1 bit

	} while (x > 0);

	return(buff);
}

// Aggiunge al polinomio l'elemento di grado nbit

void setbit(uint64_t *dato, unsigned int nbit) {

	if (nbit > 63)
		nbit = 63;

	*dato |= (UINT64_C(1) << nbit);
}

// Toglie dal polinomio l'elemento di grado nbit

void resbit(uint64_t *dato, unsigned int nbit) {

	if (nbit > 63)
		nbit = 63;

	*dato &= ~(UINT64_C(1) << nbit);
}

// Test se il polinomio ha l'elemento di grado nbit

bool tstbit(uint64_t dato, unsigned int nbit) {

	if (nbit > 63)
		nbit = 63;

	return((dato &= (UINT64_C(1) << nbit)) ? true : false);
}

// Somma di 2 polinomi (ex-or)

uint64_t somma(uint64_t dato1, uint64_t dato2) {

	return(dato1 ^ dato2);
}

// Prodotto di 2 polinomi

uint64_t prodotto(uint64_t dato1, uint64_t dato2) {

	uint64_t prod;
	unsigned int nbit;

	prod = 0;

	for (nbit = 0; nbit < 64; nbit++) {

		if ( tstbit(dato2, nbit) ) {

		prod = somma(prod, (dato1 << nbit));
		}

	}

	return(prod);
}

// grado di un polinomio

unsigned int grado(uint64_t dato) {

	int grd;
	int nbit;

	grd = 0;

	for (nbit = 0; nbit < 64; nbit++) {

		if (tstbit(dato, nbit))
			grd = nbit;
	}

	return(grd);
}

uint64_t modulo(uint64_t dato1, uint64_t dato2) {

	int gap;

	while (true) {

		gap = grado(dato1) - grado(dato2);

		if (gap < 0)
			break;

		dato1 = somma(dato1, prodotto(dato2, UINT64_C(1) << gap));
	}

	return(dato1);
}

void stampa(char *intest, uint64_t a) {

	printf("%s%s\n", intest, pBin(a));
}


void TestSingPol(uint64_t polmod, bool *fl_irr, bool *fl_prim, bool log, bool solop) {

	uint64_t seed;
	unsigned int ordine;
	uint64_t maxel;					// nr. massimo elementi del gruppo
	char intest[129];
	char buff[129];
	uint64_t el;
	uint64_t el_old;
	uint64_t nel;
	uint64_t prs;
	uint64_t uls;

	*fl_irr = true;
	*fl_prim = true;

	ordine = grado(polmod);

	if (log) {
		stampa("\n\nPolinomio in test = ", polmod);
		printf("====================================\n\n");
	}

	maxel = (UINT64_C(1) << ordine) - 1;

	if (solop) {
		prs = 2;
		uls = 2;
	}
	else {
		prs = 1;
		uls = maxel;
	}

	for (seed = prs; seed <= uls; seed++) {

		if (log)
			stampa("\nSeed = ",seed);

		el = 0;
		nel = 0;

		do {

			if (el == 0)
				el = 1;
			else {
				el_old = el;
				el = modulo(prodotto(el, seed), polmod);
				if (el <= 1) {
					el = el_old;
					break;
				}
			}

			nel++;

			if (log) {
				strcpy(intest, "Nr. elemento ");
				sprintf(buff, "%" PRIu64 "", nel);
				strcat(intest, buff);
				strcat(intest, " = ");
				stampa(intest, el);
			}


		} while (nel < maxel);

		el = modulo(prodotto(el, seed), polmod);

		if (el == 1) {
			if (nel == maxel) {
				if (log)
					stampa("Elemento generatore = ", seed);
			}
			else
				if (seed > 1) {
					if (log)
						stampa("Elemento non generatore = ", seed);

					if (seed == 2)
						*fl_prim = false;
				}
		}
		else
			{
				if (log) {
					if (el == 0)
						stampa("Resto 0 per Elemento = ", seed);
					else
						stampa("Ciclo anomalo per Elemento = ", seed);
				}
				*fl_irr = false;
				*fl_prim = false;
				break;
			}
	}

	if (log)
		printf("\n\n");
}


void TestPol(unsigned int grad, bool log, bool solop) {

	bool fl_irr;
	bool fl_prim;
	uint64_t npol;
	uint64_t indpol;
	uint64_t appo;
	uint64_t nprim;
	uint64_t nirr;
	uint64_t nnirr;
	uint64_t polmod;
	uint64_t maxel;
	uint64_t seed;
	uint64_t el;
	uint64_t el_old;
	uint64_t nel;

	printf("\n");
	printf("=================================================\n");
	printf("Inizio valutazione polinomi modulo di ordine %d\n", grad);
	printf("Elementi da 1 a %" PRIu64 "\n", (UINT64_C(1) << grad) - 1);
	printf("=================================================\n\n");

	nprim = 0;
	nirr = 0;
	nnirr = 0;

	npol = UINT64_C(1) << (grad - 1);
	appo = (npol << 1);

	for (indpol = 0; indpol < npol; indpol++) {

		polmod = appo | (indpol << 1) | 1;

		TestSingPol(polmod, &fl_irr, &fl_prim, log, solop);

		if (fl_prim)
			printf("Il polinomio = %s e' primitivo\n", pBin(polmod));
		else
			if (solop)
				printf("Il polinomio = %s non e' primitivo\n", pBin(polmod));
			else {
				if (fl_irr)
					printf("Il polinomio = %s e' irriducibile\n", pBin(polmod));
				else
					printf("Il polinomio = %s non irriducibile\n", pBin(polmod));
			}

		if (fl_prim) {
			nprim++;
			nirr++;
		}
		else
			if (fl_irr)
				nirr++;
			else
				nnirr++;

		if (log) {
			printf("\nPremere un tasto\n");
			fflush(stdout);
			getchar();
		}
	}

	printf("\nRisultato:\n");

	if (solop)
		printf("%" PRIu64 " polinomi primitivi\n", nprim);
	else
		printf("%" PRIu64 " polinomi primitivi\n%" PRIu64 " polinomi irriducibili\n%" PRIu64 " polinomi non irriducibili\n", nprim, nirr, nnirr);

	printf("\nPremere un tasto\n");
	fflush(stdout);
	getchar();

	for (indpol = 0; indpol < npol; indpol++) {

		polmod = appo | (indpol << 1) | 1;

		TestSingPol(polmod, &fl_irr, &fl_prim, log, solop);

		if (fl_prim)
			printf("=====================================\nIl polinomio = %s e' primitivo\n", pBin(polmod));
		else
			if (solop)
				printf("=====================================\nIl polinomio = %s non e' primitivo\n", pBin(polmod));
			else
				if (fl_irr)
					printf("=====================================\nIl polinomio = %s e' irriducibile\n", pBin(polmod));

		if (fl_prim || fl_irr) {

			maxel = (UINT64_C(1) << grado(polmod)) - 1;

			for (seed = 1; seed <= maxel; seed++) {

				if (log)
					stampa("\nSeed = ", seed);

				el = 0;
				nel = 0;

				do {

					if (el == 0)
						el = 1;
					else {
						el_old = el;
						el = modulo(prodotto(el, seed), polmod);
						if (el <= 1) {
							el = el_old;
							break;
						}
					}

					if (seed > 1 || grad == 1)
						printf("%" PRIu64 " ", el);

					nel++;

				} while (nel < maxel);

				printf("\n");

				if (seed == maxel) {

					if (fl_prim)
						printf("=====================================\nIl polinomio = %s e' primitivo\n", pBin(polmod));
					else
						if (solop)
							printf("=====================================\nIl polinomio = %s non e' primitivo\n", pBin(polmod));
						else
							if (fl_irr)
								printf("=====================================\nIl polinomio = %s e' irriducibile\n", pBin(polmod));
				}
			}

		printf("\nPremere un tasto\n");
		fflush(stdout);
		getchar();
		}
	}

	printf("\nFine\n");
	printf("\nPremere un tasto\n");
	fflush(stdout);
	getchar();
}

void Help(void) {

	printf("\n");
	printf("Opzioni:\n");
	printf("\t-grado=n	degree of the polynomials\n");
	printf("\t-solop		check only primitivity (faster) and not irreducibility\n");
	printf("\t-log		verbose mode\n");
	printf("\t-help		this help\n");
	printf("\n");
}

uint64_t NextLfsr(uint64_t polmod, uint64_t seed, uint64_t lfsr) {

	if ((grado(seed) >= grado(polmod)) || (grado(lfsr) >= grado(polmod)))
		return(0);

	if (lfsr == 0)
		return(seed);

	return(modulo(lfsr << UINT64_C(1), polmod));
}

bool Lfsr(uint64_t polmod, uint64_t seed, unsigned int nbit) {

	uint64_t lfsr;
	uint64_t nel;
	uint64_t maxel;
	uint64_t scar;
	uint64_t prel;
	uint64_t ind;
	uint64_t appo;
	uint64_t printed;
	uint64_t val;
	bool coda[128];

	if (nbit < 1 || nbit > 64)
		return(false);

	stampa("\n\nPolinomio lsfr = ", polmod);
	printf("Seme = %" PRIu64 "\n",seed);
	if (nbit < 14)
		printf("Flusso di %" PRIu64 " digit decimali senza spaziatura\n",
				(uint64_t)(log10((double)((UINT64_C(1) << nbit) - 1))) + 1);
	else
		printf("Flusso di valori decimali con spaziatura\n");
	printf("Numeri generati da 1 a (2^%d - 1) = %" PRIu64 "\n", nbit, (UINT64_C(1) << nbit) - 1);
	printf("====================================\n\n");

	prel = 0;
	scar = 0;

	maxel = (UINT64_C(1) << grado(polmod)) - 1;

	lfsr = 0;

	printed = 0;
	val = 0;

	for (nel = 0; nel < maxel; nel++) {

		if ((lfsr = NextLfsr(polmod, seed, lfsr)) == 0)
			return(false);

		appo = lfsr;

		for (ind = 0; ind < grado(polmod); ind++) {

			coda[scar++] = tstbit(appo, 0);
			scar %= UINT64_C(128);
			appo >>= UINT64_C(1);
		}

		while ((prel != scar)) {

			val |= (coda[prel++] << printed++);
			prel %= UINT64_C(128);

			if (printed == nbit) {

				switch (nbit) {

				case	1:		printf("%" PRIu64 "", val);
								break;
				case	2:		printf("%" PRIu64 "", val);
								break;
				case	3:		printf("%" PRIu64 "", val);
								break;
				case	4:		printf("%" PRIu64 "", val);
								break;
				case	5:		printf("%" PRIu64 "", val);
								break;
				case	6:		printf("%" PRIu64 "", val);
								break;
				case	7:		printf("%" PRIu64 "", val);
								break;
				case	8:		printf("%" PRIu64 "", val);
								break;
				case	9:		printf("%" PRIu64 "", val);
								break;
				case	10:		printf("%" PRIu64 "", val);
								break;
				case	11:		printf("%" PRIu64 "", val);
								break;
				case	12:		printf("%" PRIu64 "", val);
								break;
				case	13:		printf("%" PRIu64 "", val);
								break;
				default:
								printf("%" PRIu64 " ", val);
								break;
				}
				printed = 0;
				val = 0;
			}
		}
	}

	printf("\n");

	return(true);
}


int main(int argc ,char *argv[]) {

	unsigned int grad;		// Grado del polinomio modulo
	bool log;
	bool solop;
	int ind;

	grad = 5;
	log = false;
	solop = false;

	for (ind = 1; ind < argc; ind++) {

		if (!strncmp(argv[ind], "-grado=", 7)) {
			sscanf(argv[ind] + 7, "%d", &grad);
			continue;
		}
		if (!strncmp(argv[ind], "-log", 4)) {
			log = true;
			continue;
		}
		if (!strncmp(argv[ind], "-solop", 6)) {
			solop = true;
			continue;
		}

		Help();

		exit(0);
	}

	if (argc == 1)
		printf("\nAssuming grado = %d as default\n", grad);

	if (grad <= 0 || grad > 63)
	{
		printf("\nIl grado deve essere maggiore di 0 e minore di 64\n");
		printf("\nPremere un tasto\n");
		fflush(stdout);
		getchar();
		exit(1);
	}

	TestPol(grad, log, solop);

	uint64_t polmod;

	polmod = 0b11111011011;

	if (Lfsr(polmod, 0b1101101, /*grado(polmod)*/ 14) == false)
		printf("Lsfr: Dati in input errati\n");

	printf("\nPremere un tasto\n");
	fflush(stdout);
	getchar();
	printf("\nExit program!\n");

	return(0);
}
