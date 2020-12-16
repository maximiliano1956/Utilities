#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	MAXDIM	16
#define	MAXISO	1024
#define	NOT	0
#define	YES	1

#define NTAB	5

void Stampa(void);
void ScanPerm(int nperm);
void Help();
void StampaPar();

unsigned int tabperm[MAXDIM];
unsigned int permiso[MAXISO][MAXDIM];
unsigned int niso;

int verbose;

#if NTAB==1
unsigned int dim=4;

int tabmol[][4] = {
	{1,2,3,4},
	{2,1,4,3},
	{3,4,1,2},
	{4,3,2,1}
};
#elif NTAB==2
unsigned int dim=4;

int tabmol[][4] = {
	{1,2,3,4},
	{2,3,4,1},
	{3,4,1,2},
	{4,1,2,3}
};
#elif NTAB==3
unsigned int dim=2;

int tabmol[][2] = {
	{1,2},
	{2,1},
};
#elif NTAB==4
unsigned int dim=3;

int tabmol[][3] = {
	{1,2,3},
	{2,3,1},
	{3,1,2},
};
#elif NTAB==5
unsigned int dim=5;

int tabmol[][5] = {
	{1,2,3,4,5},
	{2,3,4,5,1},
	{3,4,5,1,2},
	{4,5,1,2,3},
	{5,1,2,3,4}
};
#else
unsigned int dim=1;

int tabmol[][1] = {
	{1},
};
#endif


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
			printf("%d ",permiso[ns][nelp]+1);
		printf("\n");
	}


	printf("\nExit program!\n");
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
						if (tabmol[tabperm[nr]][tabperm[nc]]-1!=tabperm[tabmol[nr][nc]-1])
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
