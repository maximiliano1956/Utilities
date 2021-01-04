#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	MAXDIM	40
#define	MAXISO	1024
#define	NOT	0
#define	YES	1

void Stampa(void);
void ScanPerm(int nperm);
void Help();
void StampaPar();

unsigned int tabperm[MAXDIM];
unsigned int permiso[MAXISO][MAXDIM];
unsigned int niso;

int verbose;

/*
unsigned int dim=6;

int tabmol[][6] = {
	{0,1,0,0,0,1},
	{1,0,1,0,0,0},
	{0,1,0,1,0,0},
	{0,0,1,0,1,0},
	{0,0,0,1,0,1},
	{1,0,0,0,1,0}
};
*/
/*
unsigned int dim=8;

int tabmol[][8] = {
	{0,1,1,0,0,0,0,0},
	{1,0,0,1,0,0,0,0},
	{1,0,0,1,1,1,0,0},
	{0,1,1,0,1,1,0,0},
	{0,0,1,1,0,1,1,0},
	{0,0,1,1,1,0,0,1},
	{0,0,0,0,1,0,0,1},
	{0,0,0,0,0,1,1,0}
};
*/

/*
  0 : 1 13 29;
  1 : 0 2 18;
  2 : 1 3 23;
  3 : 2 4 10;
  4 : 3 5 27;
  5 : 4 6 14;
  6 : 5 7 19;
  7 : 6 8 24;
  8 : 7 9 29;
  9 : 8 10 16;
 10 : 3 9 11;
 11 : 10 12 20;
 12 : 11 13 25;
 13 : 0 12 14;
 14 : 5 13 15;
 15 : 14 16 22;
 16 : 9 15 17;
 17 : 16 18 26;
 18 : 1 17 19;
 19 : 6 18 20;
 20 : 11 19 21;
 21 : 20 22 28;
 22 : 15 21 23;
 23 : 2 22 24;
 24 : 7 23 25;
 25 : 12 24 26;
 26 : 17 25 27;
 27 : 4 26 28;
 28 : 21 27 29;
 29 : 0 8 28;
*/
/*
unsigned int dim=30;

int tabmol[][30] = {
	{0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0},
	{0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
	{0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
	{1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,0},
	{0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0},
	{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0},
	{0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1},
	{1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}
};
*/
unsigned int dim=4;

int tabmol[][6] = {
	{0,1,0,2},
	{1,0,2,0},
	{0,2,0,1},
	{2,0,1,0}
};

int main(int argc,char *argv[])
	{
	unsigned int nelp,ns,ind;
	verbose = NOT;

	for (ind=1;ind<argc;ind++)
		{
		if (!strncmp(argv[ind],"-v",2))
			verbose=YES;
		else if (!strncmp(argv[ind],"-h",2))
			{
			Help();
			return(0);
			}
		else if (!strncmp(argv[ind],"-help",5))
			{
			Help();
			return(0);
			}
		else	
			{
			printf("\nOpzione non riconosciuta!\n");
			Help();
			return(0);
			}
		}

	StampaPar();

	niso=0;

	ScanPerm(0);

	printf("\nNr. automorfismi trovati = %d\n",niso);
	printf("\n");

	for (ns=0;ns<niso;ns++)
	{
		printf("Automorfismo nr. %d\tPermutazione = ",ns+1);
		for (nelp=0;nelp<dim;nelp++)
			printf("%d ",permiso[ns][nelp]);
		printf("\n");
	}


	printf("\nExit program!\n");
	return(0);
	}

void Stampa(void)
	{
	int nr,nc;

	for (nr=0;nr<dim;nr++)
		{
		for (nc=0;nc<dim;nc++)
			printf("%d ",tabmol[nr][nc]);
		printf("\n");
		}
	}

void ScanPerm(int nperm)
	{
	int nel;
	int nelp;
	int np;
	int ind1;
	int nr;
	int nc;
	int esci;

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
			if (verbose==YES)
				{
				printf("Check permutazione:  ");

				for (nr=0;nr<dim;nr++)
					printf("%d ",tabperm[nr]);
				printf("\n\n");

				//getchar();
				}

			esci=0;

			for (nr=0;nr<dim;nr++)
				if (esci==0)
					for (nc=0;nc<dim;nc++)
						if (tabmol[nr][nc]!=tabmol[tabperm[nr]][tabperm[nc]])
						{
							esci=1;
							break;
						}

			if (esci==0)
			{
				if (niso>=MAXISO)
				{
					printf("Not enough memory!\n");
					exit(1);
				}

				for (nelp=0;nelp<dim;nelp++)
					permiso[niso][nelp]=tabperm[nelp];
				printf("Trovato automorfismo nr=%d\n",++niso);
			}
			continue;
			}

		ScanPerm(np);			/* ricorsivita' */
		}
	}

void StampaPar(void)
	{
	printf("\n");
	printf("Valore parametri:\n\n");
	printf("Verbose = %d\n",verbose);
	printf("Dim = %d\n",dim);
	printf("\nTabella di Cailey = \n");
	Stampa();

	printf("\nPremere un tasto\n");
	fflush(stdout);
	getchar();
	}

void Help(void)
	{
	printf("\n");
	printf("Opzioni disponibili:\n\n");
	printf("-v	   verbose\n");
	printf("\n");
	printf("-h         questo help\n");
	printf("-help      questo help\n");
	printf("\n");
	}
