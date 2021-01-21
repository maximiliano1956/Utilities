#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dlx.h"


/*
 * Ricerca sistemi di Steiner (t,b,n)
*/

typedef struct {		// Struttura dinamica per le combinazioni
	char **combnz;		// Pointer matrice dinamica contenente la lista di combinazioni
	int numc;		// Numero di elementi in lista
} MATRIX;

MATRIX comb_bn;			// Combinazioni (b,n)
MATRIX comb_tn;			// Combinazioni (t,n)
MATRIX comb_tb;			// Combinazioni (t,b)

char *ind;			// Vettore dinamico utilizzato nella generazione delle configurazioni
char *set;			// Vettore dinamico utilizzato nella generazione delle configurazioni
char *soluz;			// Vettore dinamico contenente le soluzioni trovate
char *MatDlx;			// Vettore dinamico utilizzata nell'algoritmo DLX

int **set_tb;			// Matrice dinamica contenente gli indici alla lista delle configurazioni (t,n)
  
unsigned int nsol;		// nr soluzioni trovate
unsigned int nb;		// nr. blocchi necessari alla esatta copertura

unsigned char n;		// nr. totale simboli
unsigned char b;		// nr. simboli in un blocco
unsigned char t;		// nr. simboli che devono comparire una e una solo volta nella lista dei blocchi della soluzione

unsigned int nt;		// Numero combinazioni (t,n)
unsigned int bt;		// Numero combinazioni (t,b)
unsigned int tnb;		// Numero combinazioni (b,n)

char verbose;			// se = 1 attiva delle stampe di debug

unsigned char dlx_err_ram;	// errore rilevato in callback dell'algoritmo DLX

/*									*/
/* Routine che imposta i parametri dello Steiner System			*/
/*									*/
/* Inserire qui i parametri che definiscono lo Steiner system (t,b,n)	*/
/*									*/
void init_par(void) {

	t=2;
	b=3;
	n=7;

/*
	t=5;
	b=8;
	n=24;
*/
}

/*							*/
/* Routine che alloca dinamicamente una matrice di char	*/
/*							*/
/* INPUT:	nrow	Nr. righe			*/	
/*       	ncol	Nr. colonne			*/
/*							*/
/* OUTPUT:	pointer alla matrice			*/
/*		NULL se errore				*/
/*							*/
char **allocMatChar(int nrow,int ncol) {

	char **mat;
	int nr;
	int nc;

	if ((mat=malloc((nrow)*sizeof(char **)))==NULL)
		return(NULL);

	for (nr=0;nr<nrow;nr++)
        	if ((mat[nr]=malloc(ncol))==NULL)
			return(NULL);

	return(mat);
}

/*								*/
/* Routine che dealloca dinamicamente una matrice di char	*/
/*								*/
/* INPUT:	mat	pointer alla matrice			*/	
/*       	nrow	Nr. righe				*/	
/*								*/
void deallocMatChar(char **mat,int nrow) {

	int nr;

	if (mat==NULL)
		return;

	for (nr=0;nr<nrow;nr++)
		free(mat[nr]);

	free(mat);

	mat=NULL;
}

/*							*/
/* Routine che alloca dinamicamente un vettore di char	*/
/*							*/
/* INPUT:	nel	Nr. elementi			*/	
/*							*/
/* OUTPUT:	pointer al vettore			*/
/*		NULL se errore				*/
/*							*/
char *allocVectChar(int nel) {

	char *vect;

	if ((vect=malloc(nel))==NULL)
		return(NULL);

	return(vect);
}

/*								*/
/* Routine che dealloca dinamicamente un vettore di char	*/
/*								*/
/* INPUT:	mat	pointer al vettore			*/	
/*								*/

void deallocVectChar(char *vect) {

	if (vect==NULL)
		return;

	free(vect);

	vect=NULL;
}

/*							*/
/* Routine che alloca dinamicamente una matrice di int	*/
/*							*/
/* INPUT:	nrow	Nr. righe			*/	
/*       	ncol	Nr. colonne			*/
/*							*/
/* OUTPUT:	pointer alla matrice			*/
/*		NULL se errore				*/
/*							*/
int **allocMatInt(int nrow,int ncol) {

	int **mat;
	int nr;
	int nc;

	if ((mat=malloc((nrow)*sizeof(int **)))==NULL)
		return(NULL);

	for (nr=0;nr<nrow;nr++)
        	if ((mat[nr]=malloc(ncol*sizeof(int)))==NULL)
			return(NULL);

	return(mat);
}


/*								*/
/* Routine che dealloca dinamicamente una matrice di int	*/
/*								*/
/* INPUT:	mat	pointer alla matrice			*/	
/*       	nrow	Nr. righe				*/	
/*								*/
void deallocMatInt(int **mat,int nrow) {

	int nr;

	if (mat==NULL)
		return;

	for (nr=0;nr<nrow;nr++)
		free(mat[nr]);

	free(mat);

	mat=NULL;
}


/*								*/
/* Routine che alloca dinamicamente la ram necessaria		*/
/*								*/
/* INPUT:	strerr	pointer al messaggio errore		*/	
/* OUTPUT:	0	OKAY					*/	
/*		1	ERRORE					*/
int allocArr(char *strerr) {

	comb_bn.combnz=allocMatChar(tnb,t);
	if (comb_bn.combnz==NULL) {
		strcpy(strerr,"comb_bn");
		return(1);
	}

	comb_tn.combnz=allocMatChar(nt,t);
	if (comb_tn.combnz==NULL) {
		strcpy(strerr,"comb_tn");
		return(1);
	}

	comb_tb.combnz=allocMatChar(bt,t);
	if (comb_tb.combnz==NULL) {
		strcpy(strerr,"comb_tb");
		return(1);
	}

	set_tb=allocMatInt(tnb,bt);
	if (set_tb==NULL) {
		strcpy(strerr,"set_tb");
		return(1);
	}

	MatDlx=allocVectChar(tnb*nt);
	if (MatDlx==NULL) {
		strcpy(strerr,"MatDlx");
		return(1);
	}

	ind=allocVectChar(n);
	if (ind==NULL) {
		strcpy(strerr,"ind");
		return(1);
	}

	set=allocVectChar(n);
	if (set==NULL) {
		strcpy(strerr,"set");
		return(1);
	}

	soluz=NULL;

	return(0);
}

/*								*/
/*	Routine che rilascia la ram allocata dinamicamente	*/
/*								*/
void deallocArr(void) {

	deallocMatChar(comb_bn.combnz,tnb);
	deallocMatChar(comb_tn.combnz,nt);
	deallocMatChar(comb_tb.combnz,bt);

	deallocMatInt(set_tb,tnb);
	deallocVectChar(MatDlx);
	deallocVectChar(ind);
	deallocVectChar(set);

	if (free!=NULL)
		free(soluz);
}

/*									*/
/* Routine ricorsiva che calcola il coefficiente binomiale (n,k)	*/
/*									*/
/* INPUT:	n							*/	
/*		k							*/
/* OUTPUT:	valore del coefficiente binomiale			*/	
/*									*/
int Comb(int n,int k) {

	if (n == 0)
		return(1);

	if (k==0 || k==n)
		return(1);
	if (k==1 || k==(n-1))
		return(n);

	return(Comb(n-1,k-1)+Comb(n-1,k));
}

/*									*/
/* Routine ricorsiva che genera tutte le combinazioni di	 	*/
/* nsym elementi a gruppi di nn						*/
/*									*/
/* INPUT:	nn	nr. elementi del raggruppamento			*/	
/*		nsym	nr. elementi da scegliere			*/
/*		n	posizione corrente				*/
/*		ii	valore di partenza della posizione corrente	*/
/*		m	pointer struttura deposito delle combinazioni	*/
/*		set	pointer transcodifica posizione-simbolo		*/
/*			se = NULL non si fa' la transcodifica		*/
/*		verbose	= 1 per attivare stampe di debug		*/
/*									*/
void Scan2(unsigned char nn,unsigned char nsym,unsigned char n,unsigned char ii,MATRIX *m,
		char set[],unsigned char verbose) {

	if (n==nn) {
		for (int i=0;i<nn;i++)
			m->combnz[m->numc][i]=(set!=NULL) ? set[ind[i]-1]:ind[i];

		m->numc++;

		return;
	}

	for (int i=ii;i<=nsym;i++) {
		ind[n]=i;

		Scan2(nn,nsym,n+1,i+1,m,set,verbose);
	}
}

/*									*/
/* Routine che innesca la ricerca ricorsiva di tutte le combinazioni 	*/
/* di nsym elementi a gruppi di nn					*/
/*									*/
/* INPUT:	order	nr. elementi del raggruppamento			*/	
/*		nsym	nr. elementi da scegliere			*/
/*		n	posizione corrente				*/
/*		m	pointer struttura deposito delle combinazioni	*/
/*		append	se = 1 non resetta la lista di deposito		*/
/*		set	pointer transcodifica posizione-simbolo		*/
/*			se = NULL non si fa' la transcodifica		*/
/*		verbose	= 1 per attivare stampe di debug		*/
/*									*/
void Scan(unsigned char order,unsigned char nsym,MATRIX *m,
		unsigned char append,char set[],unsigned char verbose) {

	if (append==0)
		m->numc=0;
	Scan2(order,nsym,0,1,m,set,verbose);

	if (verbose==1) {
		printf("\n");
		printf("(");

		for (int i=0;i<nsym;i++) {
			if (set!=NULL)
				printf("%2d",set[i]);
			else
				printf("%2d",i+1);
			if (i<nsym-1)
				printf(",");
		}
		printf(")\n");

		for (int i=0;i<m->numc;i++) {
			for (int j=0;j<order;j++)
				printf("%2d ",m->combnz[i][j]);
			printf("\n");
		}
		printf("Nr. elementi = %d\n",m->numc);
	}
}


/*									*/
/* Routine di callback chiamata quando di trova una soluzione	 	*/
/* durante la ricerca con l'algoritmo DLX				*/
/*									*/
/* INPUT:	rows_size	length di un blocco (cioe' b)		*/	
/*		rows		pointer dati del blocco			*/
/*				i dati sono gli indici nella lista	*/
/*				delle cpmbinazioni (n,t)		*/
/*		data		non usato				*/
/* OUTPUT:	0		OKAY					*/	
/*		1		ERRORE					*/
/*									*/
int cback(int rows_size,int *rows,void *data) {

	int idx;

	if (rows_size!=nb) {
		printf("Incoerent data!\n!");
		return(1);
	}

	soluz=realloc(soluz,(nsol+1)*rows_size*b);
	if (soluz==NULL) {
		dlx_err_ram=1;
		return(1);
	}

	idx=nsol*rows_size*b;
	for (int i=0;i<rows_size;i++)
		for (int j=0;j<b;j++)
			soluz[idx++]=comb_bn.combnz[rows[i]][j];
	nsol++;

	return(0);
}

/*				*/
/* Inizializzazioni varie  	*/
/*				*/
/* OUTPUT:	0	OKAY	*/	
/*		1	ERRORE	*/
/*				*/
int init(void) {

	char str[64];

	init_par();

	t=floor(t);
	b=floor(b);
	n=floor(n);

	if (t<1 || n<t || n<b || b<t) {
		printf("It sistema di Steiner (%d,%d,%d) non puo' esistere!\n",t,b,n);
		return(1);
	}

	verbose=0;

	nsol=0;

	if (b==3)
		if (n%6!=1 && n%6!=3) {
			printf("It sistema di Steiner (%d,%d,%d) non puo' esistere!\n",t,b,n);
			return(1);
		}

	if (Comb(n,t)%Comb(b,t)!=0) {
		printf("It sistema di Steiner (%d,%d,%d) non puo esistere!\n",t,b,n);
		return(1);
	}
  
	printf("\n");
	printf("Steiner System (%d,%d,%d)\n",t,b,n);
	printf("\n");

	nt=Comb(n,t);
	bt=Comb(b,t);
	nb=nt/bt;
	tnb=Comb(n,b);

	printf("Numero combinazioni di %d elementi su %d = %d\n",t,n,nt);
	printf("Numero combinazioni di %d elementi su %d = %d\n",b,n,tnb);
	printf("Numero blocchi di %d elementi su %d necessari per la copertura esatta = %d\n",b,n,nb);
	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	if (allocArr(str)==1) {
		printf("Non riesco ad allocare %s!\n",str);
		return(1);
	}

	return(0);
}

/*						*/
/* Rilascio memoria allocata dinamicamente  	*/
/*						*/
void cleanUp(void) {

	deallocArr();
}

/*				*/
/* Ricerca configurazioni  	*/
/*				*/
/* OUTPUT:	0	OKAY	*/	
/*		1	ERRORE	*/
/*				*/
int search(void) {

	unsigned int trovati;
	unsigned int idx;
	struct dlx_Node *header;

	Scan(t,n,&comb_tn,0,NULL,verbose);
	Scan(b,n,&comb_bn,0,NULL,verbose);

	printf("Generazione dei %d blocchi di %d elementi su %d possibili\n\n",tnb,b,n);

	for (int i=0;i<comb_bn.numc;i++) {
		if (verbose==0)
			if (i%500==0 || comb_bn.numc-i<10)
				printf("%d\n",i);
		for (int j=0;j<b;j++)
			set[j]=comb_bn.combnz[i][j];
		Scan(t,b,&comb_tb,0,set,0);
		for (int j=0;j<comb_tb.numc;j++) {
				for (int k=0;k<comb_tn.numc;k++) {
					trovati=0;
					for (int l=0;l<t;l++)
						if (comb_tb.combnz[j][l]==comb_tn.combnz[k][l])
							trovati++;
						else
							break;
					if (trovati==t) {
						set_tb[i][j]=k;
						break;
					}
				}
				if (trovati!=t) {
					printf("Dati incoerenti!\n");
					return(1);
				}
		}
	}

	if (verbose==1)
		for (int i=0;i<tnb;i++) {
			printf("{");
			for (int j=0;j<b;j++) {
				printf("%2d",comb_bn.combnz[i][j]);
				if (j<b-1)
					printf(",");
			}
			printf("}     ");
			for (int j=0;j<bt;j++) {
				if (j==0)
					printf("{");
				for (int k=0;k<t;k++) {
					if (k==0)
						printf("{");
					printf("%2d",comb_tn.combnz[set_tb[i][j]][k]);
					if (k<t-1)
						printf(",");
					else
						printf("}");
				}
				if (j<bt-1)
					printf(",");
				else
					printf("}\n");
			}
		}
	printf("Nr. elementi = %d\n",tnb);

	idx=0;
	for (int i=0;i<tnb;i++)
		for (int j=0;j<nt;j++)
			MatDlx[idx++]=0;

	idx=0;
	for (int i=0;i<tnb;i++) {
		for (int j=0;j<bt;j++) {
			if (set_tb[i][j]>=nt) {
				printf("Dati incoerenti!\n!");
				return(1);
			}
			for (int k=0;k<t;k++)
				MatDlx[idx+set_tb[i][j]]=1;
		}
		idx+=nt;
	}
#ifdef DEBUG
	if (verbose) {
		idx=0;
		for (int i=0;i<tnb;i++) {
			for (int j=0;j<nt;j++) {
				printf("%1d",MatDlx[idx++]);
			}
			printf("\n");
		}
	}
#endif
	header=dlx_alloc(MatDlx,tnb,nt);

	if (header==NULL) {
		printf("Not enough memory! (dlx_alloc)\n");
		return(1);
	}

	dlx_err_ram=0;

	dlx_solve(header,cback,NULL);
	dlx_free(header);

	if (dlx_err_ram==1) {
		printf("Not enough memory for solutions!\n\n");
		return(1);
	}

	return(0);
}

/*				*/
/* Visualizza soluzioni		*/
/*				*/
void showSol(void) {

	unsigned int idx;

	printf("\n");
	printf("\n");
	printf("Trovate %d soluzioni\n\n",nsol);
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	idx=0;
	for (int ns=0;ns<nsol;ns++) {
		printf("Soluzione nr. %6d      ",ns+1);
		printf("{");
		for (int i=0;i<nb;i++) {
			printf("{");
			for (int j=0;j<b;j++) {
				printf("%d",soluz[idx++]);
				if (j<b-1)
					printf(",");
			}
			printf("}");
			if (i<nb-1)
				printf(",");
		}
		printf("}\n");
	}

	printf("\n");
	printf("\n");
}


/*				*/
/*	Routine principale	*/
/*				*/
/* OUTPUT:	0	OKAY	*/	
/*		1	ERRORE	*/
/*				*/
int main(char argc,char *argv[]) {

	if (init()==1)			// Inizializzazione
		return(1);

	if (search()==1)		// Ricerca soluzioni
		return(1);

	showSol();			// Visualizza soluzioni

	cleanUp();			// Rilascio memoria allocata dinamicamente

	return(0);			// No errors
}
