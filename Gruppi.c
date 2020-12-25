#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>

#define esc 27

#define	MAXDIM	16
#define	MAXSOL	10000
#define	NOT	0
#define	YES	1

#define NLOOP	10000

#define	MAGMA		0
#define C_MAGMA		1
#define SEMIGRUPPO	2
#define C_SEMIGRUPPO	3
#define	MONOIDE		4
#define	C_MONOIDE	5
#define	GRUPPO		6
#define	C_GRUPPO	7
#define	QUASIGRUPPO	8
#define	C_QUASIGRUPPO	9
#define	LOOP		10
#define	C_LOOP		11
#define	IPLOOP		12
#define	C_IPLOOP	13
#define	MAGMA_U		14
#define	C_MAGMA_U	15
#define	SEMIGRUPPO_I	16
#define	C_SEMIGRUPPO_I	17

#define MAXTYPE	C_SEMIGRUPPO_I

#define NOPOT	0
#define NILPOT	1
#define IDEMPOT	2


void Stampa(void);
void StampaPerm(void);
void Scan(int nrig,int ncol);
void ScanPerm(int nperm);
int Accettato();
int CkAss(void);
int CkNeutro();
int CkInvers();
int CkInvers_nn();
int CkIPLoop();
int CkComm();
void Help();
void StampaPar();
void StampaTipi(void);

int dim;
int neutro;
int fast_neutro;
int qg;
int pass;
int invers;
int invers_nn;
int iploop;
int commut;
int pot;
int ckiso;
int verbose;
int tabmol[MAXDIM][MAXDIM];
int ntab[MAXSOL][MAXDIM][MAXDIM];
int nf[MAXSOL];
int inversi[MAXSOL][MAXDIM];
int invr[MAXDIM];
int tabmola[MAXDIM][MAXDIM];
int tabmolb[MAXDIM][MAXDIM];
int tabperm[MAXDIM];
unsigned int nsol;
unsigned int npp;
unsigned int ns;
unsigned int iter;
int neu;
jmp_buf buf;

int tipo;

// Tipo 0		magma
// Tipo 1		magma commutativo
// Tipo 2		semigruppo
// Tipo 3		semigruppo commutativo
// Tipo 4		monoide
// Tipo 5		monoide commutativo
// Tipo 6		gruppo
// Tipo 7		gruppo commutativo
// Tipo 8		quasigruppo
// Tipo 9		quasigruppo commutativo
// Tipo 10		loop
// Tipo 11		loop commutativo
// Tipo 12		loop IP
// Tipo 13		loop IP commutativo
// Tipo 14		magma on unita'
// Tipo 15		magma con unita' commutativo
// Tipo 16		semigruppo con elementi inversi
// Tipo 17		semigruppo con elementi inversi commutativo

char *TipiStr[] = { "Magma", "Magma commutativo", "Semigruppo", "Semigruppo commutativo", "Monoide", "Monoide commutativo", "Gruppo", "Gruppo commutativo", "Quasigruppo", "Quasigruppo commutativo", "Loop", "Loop commutativo", "IP Loop", "IP Loop commutativo",
"Magma con unita'", "Magma con unita' commutativo", "Semigruppo con elementi inversi", "Semigruppo con elementi inversi commutativo" };

char *TipoPot[] = {"nessuna","nilpotenza","idempotenza"};

int main(int argc,char *argv[])
	{
	int nr,nc;
	int ind;
	int appo;

	dim = 4;
	tipo = GRUPPO;
	ckiso = YES;
	fast_neutro=YES;
	pot=NOPOT;
	verbose = NOT;

	for (ind=1;ind<argc;ind++)
		{
		if (!strncmp(argv[ind],"-nockiso",8))
			ckiso=NOT;
		else if (!strncmp(argv[ind],"-tipo=",6))
			{
			if (sscanf(argv[ind]+6,"%d",&appo)==1 && appo>=0 && appo<=MAXTYPE)
				tipo=appo;
			}
		else if (!strncmp(argv[ind],"-ord=",5))
			{
			if (sscanf(argv[ind]+5,"%d",&appo)==1 && appo>=1 && appo<=MAXDIM)
				dim=appo;
			}
		else if (!strncmp(argv[ind],"-nofast",7))
			fast_neutro=NOT;
		else if (!strncmp(argv[ind],"-pot=",5))
			{
			if (sscanf(argv[ind]+5,"%d",&appo)==1 && appo>=1 && appo<=IDEMPOT)
				pot=appo;
			}
		else if (!strncmp(argv[ind],"-t",2))
			{
			StampaTipi();
			return(0);
			}
		else if (!strncmp(argv[ind],"-v",2))
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

	pass=NOT;
	qg=NOT;
	neutro=NOT;
	invers=NOT;
	invers_nn=NOT;
	iploop=NOT;
	commut=NOT;

	switch (tipo & 0xFFFFFFFE)
		{
			case	SEMIGRUPPO:
					pass=YES;
					break;
			case	MONOIDE:
					pass=YES;
					neutro=YES;
					break;
			case	GRUPPO:
					pass=YES;
					neutro=YES;
					invers=YES;
					qg=YES;
					break;
			case	QUASIGRUPPO:
					qg=YES;
					break;
			case	LOOP:
					qg=YES;
					neutro=YES;
					break;
			case	IPLOOP:
					qg=YES;
					neutro=YES;
					iploop=YES;
					break;
			case	MAGMA_U:
					neutro=YES;
					break;
			case	SEMIGRUPPO_I:
					pass=YES;
					invers_nn=YES;
					break;
		}

	if (tipo%2)
		commut = YES;

	iter=0;
	nsol=0;

	for (nc=0;nc<dim;nc++)
		for (nr=0;nr<dim;nr++)
			tabmol[nr][nc]=0;

	printf("\nRicerca configurazione corretta\n");
	printf("\nAttesa ");

	if (verbose==YES)
		printf("\n");

	Scan(0,0);	/* Parte la ricerca ricorsiva */

	printf("\n\n===FINE RICERCA==\n\n");
	printf("\nNr. soluzioni=%d\n",nsol);

	for (ns=0;ns<nsol;ns++)
		{
		printf("\nSol nr. %d",ns+1);

		for (nr=0;nr<dim;nr++)
			for (nc=0;nc<dim;nc++)
				tabmol[nr][nc]=ntab[ns][nr][nc];
		Stampa();

		if (neutro==YES && fast_neutro==NOT)
			printf("Elemento neutro = %d\n",nf[ns]+1);

		if (invers_nn==YES)
		{
			printf("Elementi inversi: ");
			for (nr=0;nr<dim;nr++)
				printf("%d ",inversi[ns][nr]+1);
			printf("\n");
		}

		printf("\nPremere un tasto ");
		fflush(stdout);
		getchar();
		}

	printf("\nBye!\n\n");
	}

void Stampa(void)
	{
	int nr,nc;

	for (nr=0;nr<dim;nr++)
		{
		printf("\n");

		for (nc=0;nc<dim;nc++)
			printf("%d ",tabmol[nr][nc]);
		}

	printf("\n");
	}

void StampaPerm(void)
	{
	int nr;

	printf("\n");

	for (nr=0;nr<dim;nr++)
		printf("%d ",tabperm[nr]);

	printf("\n\n");
	}

void Scan(int nrig,int ncol)
	{
	int nr,nc;
	int ind1;
	int i,j;
	int nel;
	int oksol;

	for (nel=0;nel<dim;nel++)
		{
                tabmol[nrig][ncol]=0;

                ind1=dim;

                if (qg)
                        for (ind1=0;ind1<dim;ind1++)
                                {
                                if (tabmol[nrig][ind1]==nel+1)
                                        break;
                                if (tabmol[ind1][ncol]==nel+1)
                                        break;
                                }

                if (ind1!=dim)
                        continue;

                if (neutro && fast_neutro)
                        {

                        if (nrig==0)
                                if (nel!=ncol)
                                continue;

                        if (ncol==0)
                                if (nel!=nrig)
                                continue;

                        }

		if (pot==NILPOT)
			if (nrig==ncol && nel!=0)
				continue;

		if (pot==IDEMPOT)
			if (nrig==ncol && nel!=nrig)
				continue;

		tabmol[nrig][ncol]=nel+1;

		nc=ncol;
		nr=nrig;

		nc++;
		nc%=dim;

		if (nc==0 && ++nr==dim)		/* trovato */
			{
			if (verbose==YES)
				Stampa();

			if (Accettato())
				{
				oksol=YES;

				if (ckiso==YES)
					{
					printf("\n\nTrovata possibile soluzione nr=%d\n",nsol+1);

					for (i=0;i<dim;i++)
						for (j=0;j<dim;j++)
							tabmola[i][j]=tabmol[i][j];

					for (ns=0;ns<nsol;ns++)
						{
						for (i=0;i<dim;i++)
							for (j=0;j<dim;j++)
								tabmolb[i][j]=ntab[ns][i][j];

						if (setjmp(buf)==1)
						{
							oksol=NOT;
							break;
						}
						else
							{
							npp=0;
							printf("Check se possibile soluzione nr=%d e' isomorfa a soluzione nr=%d\n",nsol+1,ns+1);
							ScanPerm(0);
							}
						}
					}

				if (oksol==YES)
					{
					 if (nsol==MAXSOL)
                                                        {
                                                        printf("\nNot enough memory!\n");
                                                        exit(1);
                                                        }

					for (i=0;i<dim;i++)
						for (j=0;j<dim;j++)
							ntab[nsol][i][j]=tabmol[i][j];

					nf[nsol]=neu;

					if (invers_nn==YES)
						for (i=0;i<dim;i++)
							inversi[nsol][i]=invr[i];

					printf("\nSoluzione nr=%d accettata\n",nsol+1);
					nsol++;
					}

				printf("\nAttesa ");

				if (verbose==YES)
					printf("\n");
				}
			else
				if (verbose==NOT)
					{
					printf("%c%d",esc,7);
					if (iter%(4*NLOOP)==0)
						printf("-");
					if (iter%(4*NLOOP)==NLOOP)
						printf("/");
					if (iter%(4*NLOOP)==2*NLOOP)
						printf("|");
					if (iter%(4*NLOOP)==3*NLOOP)
						printf("\\");
					printf("%c%d",esc,8);
					iter++;
					}
			continue;
			}

		Scan(nr,nc);			/* ricorsivita' */
		}

	tabmol[nrig][ncol]=0;
	}

int Accettato()
	{
	int esito;

	esito=CkAss() && CkNeutro() && CkInvers() && CkInvers_nn() && CkIPLoop() && CkComm();

	return(esito);
	}

int CkNeutro()
	{
	int ns1,ns2;

	neu=-1;

	if (neutro==NOT)
		return(1);

        for (ns1=0;ns1<dim;ns1++)
		{
        	for (ns2=0;ns2<dim;ns2++)
			if (tabmol[ns1][ns2]!=ns2+1 || tabmol[ns2][ns1]!=ns2+1)
				break;
		if (ns2==dim)
			{
			if (neu!=-1)
				return(0);
			neu=ns1;
			}
		}


	return((neu==-1) ? 0:1);
	}

int CkAss(void)
	{
	int n1,n2,n3;

	if (pass==NOT)
		return(1);

	for (n1=0;n1<dim;n1++)
		for (n2=0;n2<dim;n2++)
			for (n3=0;n3<dim;n3++)
			{
				if (tabmol[tabmol[n1][n2]-1][n3]!=
				tabmol[n1][tabmol[n2][n3]-1])
					{
					#if 0
					printf("\n%d %d %d\n",n1,n2,n3);
					#endif
					return(0);
					}
			}
	return(1);
	}

int CkInvers(void)
	{
	int n1,n2;
	int count;

	if (invers==NOT)
		return(1);

	for (n1=0;n1<dim;n1++)
		{
		count=0;
		for (n2=0;n2<dim;n2++)
			if ((tabmol[n1][n2] == tabmol[n2][n1]) && (tabmol[n1][n2] == neu+1))
				count++;
		if (count!=1)
			return(0);
		}

	return(1);
	}

int CkInvers_nn(void)
	{
	int n1,n2;
	int found;

	if (invers_nn==NOT)
		return(1);

	for (n1=0;n1<dim;n1++)
	{
		found=NOT;

		for (n2=0;n2<dim;n2++)
			if ((tabmol[n1][tabmol[n2][n1]-1]==n1+1 && tabmol[n2][tabmol[n1][n2]-1]==n2+1))	// x=xyx y=yxy
			{
				found=YES;
				invr[n1]=n2;
				break;
			}

		if (found==NOT)
			return(0);
	}

	return(1);
	}

int CkIPLoop(void)
	{
	int n1,n2,n3;
	int found;

	if (iploop==NOT)
		return(1);

	for (n1=0;n1<dim;n1++)
		{
		found=NOT;
		for (n2=0;n2<dim;n2++)
			{
			for (n3=0;n3<dim;n3++)
				if (tabmol[n2][tabmol[n1][n3]-1]!=tabmol[tabmol[n3][n1]-1][n2] || tabmol[n2][tabmol[n1][n3]-1]!=n3+1)
					break;
			if (n3!=dim)
				continue;

			if (found==YES)
				return(0);
			else
				found=YES;
			}
		if (found!=YES)
			return(0);
		}
	return(1);
	}

int CkComm(void)
	{
	int n1,n2;

	if (commut==NOT)
		return(1);

        for (n1=0;n1<dim-1;n1++)
	        for (n2=n1+1;n2<dim;n2++)
			if (tabmol[n1][n2]!=tabmol[n2][n1])
				return(0);

	return(1);
	}

void ScanPerm(int nperm)
	{
	int nel;
	int np;
	int ind1;
	int nr;
	int nc;
	int noiso;

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
			npp++;
			if (verbose==YES)
				{
				printf("Check permutazione:  ");
				StampaPerm();
				//getchar();
				}

			noiso=NOT;

			for (nr=0;nr<dim;nr++)
			{
				for (nc=0;nc<dim;nc++)
					if (tabperm[tabmola[nr][nc]-1]!=
					    tabmolb[tabperm[nr]][tabperm[nc]]-1)
					{
						noiso=YES;
						break;
					}
				if (noiso==YES)
					break;
			}
			if (noiso==NOT)
				{
				//StampaPerm();
				printf("Soluzione scartata : Isomorfismo con soluzione nr=%d\n",ns+1);
				longjmp(buf,1);
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
	printf("Ordine = %d\n",dim);
	printf("Tipo = %d  (%s)\n",tipo,TipiStr[tipo]);
	printf("Controllo isomorfismi = %s\n",(ckiso) ? "si" : "no");
	printf("Disabilita scelta neutro=1 = %s\n",(fast_neutro) ? "no" : "si");
	printf("Potenza = %s\n",TipoPot[pot]);
	printf("Verbose = %s\n",(verbose) ? "si" : "no");

	printf("\nPremere un tasto ");
	fflush(stdout);
	getchar();
	}

void Help(void)
	{
	printf("\n");
	printf("Opzioni disponibili:\n\n");
	printf("-ord=n     Ordine della struttura (da 1 a 16)\n");
	printf("-tipo=t    Tipo dello struttura (da 0 a 17)\n");
	printf("-nockiso   disabilita il controllo sugli isomorfismi\n");
	printf("-nofast	   disabilita la scelta neutro=1\n");
	printf("-pot=n     tipo potenza (= 0 nessuna,= 1 nilpotenza,= 2 idempotenza)\n");
	printf("-v	   verbose\n");
	printf("\n");
	printf("-t         stampa i vari tipi di struttura\n");
	printf("-h         questo help\n");
	printf("-help      questo help\n");
	printf("\n");
	}

void StampaTipi(void)
	{
	int tp;

	printf("\n");
	printf("Tipi disponibili:\n\n");

	for (tp=0;tp<=MAXTYPE;tp++)
		{
			printf("%d	%s\n",tp,TipiStr[tp]);
		}
	printf("\n");
	}
