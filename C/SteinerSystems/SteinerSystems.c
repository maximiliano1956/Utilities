#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "dlx.h"


/*
 * Ricerca sistemi di Steiner (t,b,n)
*/

#define SNPSHT_NAME	"./Steiner_snapshot.bin"	// Nome immagine snapshot

typedef struct {		// Struttura dinamica per le combinazioni
	char **combnz;		// Pointer matrice dinamica contenente la lista di combinazioni
	unsigned int numc;	// Numero di elementi in lista
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

u_int64_t nt;			// Numero combinazioni (t,n)
u_int64_t bt;			// Numero combinazioni (t,b)
u_int64_t tnb;			// Numero combinazioni (b,n)

u_int64_t size;			// Size della ram allocata dinamicamente

char verbose;			// se = 1 attiva delle stampe di debug

unsigned char dlx_err_ram;	// errore rilevato in callback dell'algoritmo DLX

void showSol(unsigned int ns,unsigned int idx);

/*									*/
/* Routine che imposta i parametri dello Steiner System			*/
/*									*/
/* Inserire qui i parametri che definiscono lo Steiner system (t,b,n)	*/
/*									*/
void init_par(void) {

	t=2;
	b=3;
	n=7;

	t=5;
	b=8;
	n=24;

	t=2;
	b=3;
	n=15;

	t=5;
	b=6;
	n=12;
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
char **allocMatChar(unsigned int nrow,unsigned int ncol) {

	char **mat;
	int nr;
	int nc;
	unsigned int sz;
	unsigned int merr;

	sz=nrow*sizeof(char **);

	if ((mat=malloc(sz))==NULL)
		return(NULL);

	size+=sz;

	merr=0;

	for (nr=0;nr<nrow;nr++)
        	if ((mat[nr]=malloc(ncol))==NULL) {
			merr=1;
			break;
		}
		else
			size+=ncol;

	if (merr==1) {
		for (nr=0;nr<nrow;nr++)
        		if ((mat[nr]!=NULL))
				free(mat[nr]);
		free(mat);
		return(NULL);
	}

	return(mat);
}

/*								*/
/* Routine che dealloca dinamicamente una matrice di char	*/
/*								*/
/* INPUT:	mat	pointer alla matrice			*/	
/*       	nrow	Nr. righe				*/	
/*								*/
void deallocMatChar(char **mat,unsigned int nrow) {

	int nr;

	if (mat==NULL)
		return;

	for (nr=0;nr<nrow;nr++)
		if (mat[nr]!=NULL)
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
char *allocVectChar(u_int64_t nel) {

	char *vect;

	if ((vect=malloc(nel))==NULL) {
		printf("Non riesco ad allocare %ld bytes!\n",nel);
		return(NULL);
	}

	size+=nel;

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
int **allocMatInt(unsigned int nrow,unsigned int ncol) {

	int **mat;
	int nr;
	int nc;
	unsigned int sz;
	unsigned int merr;

	sz=nrow*sizeof(int **);

	if ((mat=malloc((nrow)*sizeof(int **)))==NULL)
		return(NULL);

	size+=sz;

	merr=0;

	sz=ncol*sizeof(int);

	for (nr=0;nr<nrow;nr++) {
        	if ((mat[nr]=malloc(sz))==NULL) {
			merr=1;
			break;
		}
		else
			size+=sz;
	}

	if (merr==1) {
		for (nr=0;nr<nrow;nr++)
        		if ((mat[nr]!=NULL))
				free(mat[nr]);
		free(mat);
		return(NULL);
	}

	return(mat);
}


/*								*/
/* Routine che dealloca dinamicamente una matrice di int	*/
/*								*/
/* INPUT:	mat	pointer alla matrice			*/	
/*       	nrow	Nr. righe				*/	
/*								*/
void deallocMatInt(int **mat,unsigned int nrow) {

	int nr;

	if (mat==NULL)
		return;

	for (nr=0;nr<nrow;nr++)
		if (mat[nr]!=NULL)
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
/*								*/
int allocArr(char *strerr) {

	size=0;

	comb_bn.combnz=allocMatChar(tnb,b);
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

	if (size>1024*1024*1024)
		printf("Ram dinamica allocata = %f Gbytes\n\n",(float)size/(1024*1024*1024));
	else if (size>1024*1024)
		printf("Ram dinamica allocata = %f Mbytes\n\n",(float)size/(1024*1024));
	else if (size>1024)
		printf("Ram dinamica allocata = %f Kbytes\n\n",(float)size/1024);
	else
		printf("Ram dinamica allocata = %f Bytes\n\n",(float)size);

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

	if (soluz!=NULL)
		free(soluz);
}

/*									*/
/* Routine ricorsiva che calcola il coefficiente binomiale (n,k)	*/
/*									*/
/* INPUT:	n							*/	
/*		k							*/
/* OUTPUT:	valore del coefficiente binomiale			*/	
/*									*/
int Comb(unsigned int n,unsigned int k) {

	if (n == 0)
		return(1);		// (0,k)=1

	if (k==0 || k==n)
		return(1);		// (n,0)=(n,n)=1
	if (k==1 || k==(n-1))
		return(n);		// (n,1)=(n,n-1)=n

	return(Comb(n-1,k-1)+Comb(n-1,k));	// ricorsione: (n,k) = (n-1,k-1)+(n-1,k)
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

	if (n==nn) {			// Test se completata la combinazione
		for (unsigned int i=0;i<nn;i++)
			m->combnz[m->numc][i]=
				(set!=NULL) ? set[ind[i]-1]:ind[i];	// Carica la combinazione con eventuale transcodifica

		m->numc++;		// Aggiorna il numero di combinazioni generate

		return;
	}

	for (unsigned int i=ii;i<=nsym;i++) {	// loop dalla posizione corrente all'ultima
		ind[n]=i;			// carica l'indice corrente

		Scan2(nn,nsym,n+1,i+1,m,set,verbose);		// ricorsione
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
		m->numc=0;			// Test se resettare o no l'indice alla lista delle combinazioni trovate 
	Scan2(order,nsym,0,1,m,set,verbose);	// Innesca la ricerca ricorsiva

	if (verbose==1) {			// Se stampe attivate
		printf("\n");
		printf("(");

		for (unsigned int i=0;i<nsym;i++) {	// Stampa l'insieme di elementi da combinare con eventuale transcodifica
			if (set!=NULL)
				printf("%2d",set[i]);
			else
				printf("%2d",i+1);
			if (i<nsym-1)
				printf(",");
		}
		printf(")\n");

		for (unsigned int i=0;i<m->numc;i++) {	// Stampa tutte le combinazioni
			for (unsigned int j=0;j<order;j++)
				printf("%2d ",m->combnz[i][j]);
			printf("\n");
		}
		printf("Nr. elementi = %d\n",m->numc);		// Stampa il numero delle combinazioni presenti in lista
	}
}


/*									*/
/* Routine di callback chiamata quando di trova una soluzione	 	*/
/* durante la ricerca con l'algoritmo DLX				*/
/*									*/
/* INPUT:	rows_size	Nr. blocchi in ogni soluzione  (nb)	*/	
/*		rows		pointer dati del blocco			*/
/*				i dati sono gli indici nella lista	*/
/*				delle cpmbinazioni (n,t)		*/
/*		data		non usato				*/
/* OUTPUT:	0		OKAY					*/	
/*		1		ERRORE					*/
/*									*/
int cback(int rows_size,int *rows,void *data) {

	unsigned int idx;
	unsigned int idx_base;

	if (rows_size!=nb) {
		printf("Dati incoerenti!\n!");
		return(1);
	}

	soluz=realloc(soluz,(nsol+1)*rows_size*b);	// Richiede un nuovo blocco di memoria
	if (soluz==NULL) {
		dlx_err_ram=1;				// Test se ram insufficiente
		return(1);
	}

	idx_base=nsol*rows_size*b;			// indice del nuovo blocco appena allocato
	idx=idx_base;

	for (unsigned int i=0;i<rows_size;i++)
		for (unsigned int j=0;j<b;j++)
			soluz[idx++]=comb_bn.combnz[rows[i]][j];	// carica la soluzione trovata

	showSol(nsol,idx_base);						// Visualizza la soluzione

	nsol++;								// aggiorna il numero di soluzioni trovate

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

	init_par();		// Inizializza t,b,n

	comb_bn.combnz=NULL;	// Inizializza pointer alla ram dinamica
	comb_tn.combnz=NULL;
	comb_tb.combnz=NULL;
	set_tb=NULL;
	MatDlx=NULL;
	ind=NULL;
	set=NULL;
	soluz=NULL;

	t=floor(t);
	b=floor(b);
	n=floor(n);		// Li rende comunque interi

	if (t<1 || n<t || n<b || b<t) {		// Test di coerenza
		printf("It sistema di Steiner (%d,%d,%d) non puo' esistere!\n",t,b,n);
		return(1);
	}

	verbose=0;		// Livello stampe

	nsol=0;			// Init nr. soluzioni trovate

	if (b==3)
		if (n%6!=1 && n%6!=3) {		// Controllo preventivo
			printf("It sistema di Steiner (%d,%d,%d) non puo' esistere!\n",t,b,n);
			return(1);
		}

	if (Comb(n,t)%Comb(b,t)!=0) {		// Controllo preventivo
		printf("It sistema di Steiner (%d,%d,%d) non puo esistere!\n",t,b,n);
		return(1);
	}

	// Intestazione
  
	printf("\n");
	printf("Steiner System (%d,%d,%d)\n",t,b,n);
	printf("\n");

	nt=Comb(n,t);
	bt=Comb(b,t);
	nb=nt/bt;
	tnb=Comb(n,b);

	printf("Numero combinazioni di %2d elementi su %2d = %ld\n",t,n,nt);
	printf("Numero combinazioni di %2d elementi su %2d = %ld\n",t,b,bt);
	printf("Numero combinazioni di %2d elementi su %2d = %ld\n",b,n,tnb);
	printf("\n");
	printf("Numero blocchi di %d elementi su %d necessari per la copertura esatta = %d\n",b,n,nb);
	printf("\n");
	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	// Alloca ram dinamica

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
	printf("\n");
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
	unsigned int togo;
#ifdef WDEBUG
	int wait;
#endif	// WDEBUG
#ifdef WFILE
	FILE *fp;
#endif	// WFILE

	Scan(t,n,&comb_tn,0,NULL,verbose);		// Genera combinazioni (t,n)
	Scan(b,n,&comb_bn,0,NULL,verbose);		// Genera combinazioni (b,n)

	printf("Generazione dei %ld blocchi di %d elementi su %d possibili\n\n",tnb,b,n);

	for (unsigned int i=0;i<comb_bn.numc;i++) {	// Loop sulle combinazioni di blocchi di b elementi su n
		if (verbose==0) {
			togo=comb_bn.numc-i;
			if (togo%500==0 || togo<10)
				printf("%d\n",togo);		// Stampa indice loop principale
		}
		for (unsigned int j=0;j<b;j++)
			set[j]=comb_bn.combnz[i][j];		// Eventuale transcodifica della combinazione
		Scan(t,b,&comb_tb,0,set,0);			// Genera da questa combinazione i gruppi di t elementi
		for (unsigned int j=0;j<comb_tb.numc;j++) {	// Per ciascuno di questi gruppi ricerca l'indice nel catalogo
				for (unsigned int k=0;k<comb_tn.numc;k++) {
					trovati=0;
					for (unsigned int l=0;l<t;l++)
						if (comb_tb.combnz[j][l]==comb_tn.combnz[k][l])
							trovati++;
						else
							break;
					if (trovati==t) {
						set_tb[i][j]=k;	// Carica indice della lista del catalogo (set_tb)
						break;
					}
				}
				if (trovati!=t) {
					printf("Dati incoerenti!\n");	// Ci DEVE essere
					return(1);
				}
		}
	}

	if (verbose==1)	// Eventuale stampa per ogni combinazione di b elementi delle sottocombinazioni di t elementi appena trovate
		for (unsigned int i=0;i<tnb;i++) {
			printf("{");
			for (unsigned int j=0;j<b;j++) {
				printf("%2d",comb_bn.combnz[i][j]);	// stampa elemento gruppo di b elementi
				if (j<b-1)
					printf(",");
			}
			printf("}     ");
			for (unsigned int j=0;j<bt;j++) {		// loop sottocombinazioni di t elementi
				if (j==0)
					printf("{");
				for (unsigned int k=0;k<t;k++) {	// loop sui t elementi di ogni sottocombinazione
					if (k==0)
						printf("{");
					printf("%2d",comb_tn.combnz[set_tb[i][j]][k]);	// utilizza l'indice nel catalogo (set_tb)
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
	if (verbose==0)
		printf("\n");
	printf("Nr. elementi = %ld\n",tnb);				// stampa nr. combinazioni possibili di b elementi su n

	idx=0;
	for (unsigned int i=0;i<tnb;i++)
		for (unsigned int j=0;j<nt;j++)
			MatDlx[idx++]=0;	// Azzera matrice per algoritmo DLX

	idx=0;
	for (unsigned int i=0;i<tnb;i++) {		// loop su blocchi di b elementi sugli n totali
		for (unsigned int j=0;j<bt;j++) {	// per ogni sottocombinazione di t elementi
			if (set_tb[i][j]>=nt) {
				printf("Dati incoerenti!\n!");
				return(1);
			}
			MatDlx[idx+set_tb[i][j]]=1;	// Indicatore che la combinazione (t,n) appartiene al blocco (b,n)
		}
		idx+=nt;	// Passa al blocco successivo
	}
#ifdef DEBUG
	if (verbose) {		// Eventuale stampa matrice binaria MatDlx
		idx=0;
		for (unsigned int i=0;i<tnb;i++) {
			for (unsigned int j=0;j<nt;j++)
				printf("%1d",MatDlx[idx++]);
			printf("\n");
		}
	}
#endif

#ifdef WFILE
	printf("\n");
	printf("Scrittura snapshot %s\n",SNPSHT_NAME);

	if ((fp=fopen(SNPSHT_NAME,"wb+"))!=NULL) {
		fwrite(&t,sizeof(unsigned char),1,fp);
		fwrite(&b,sizeof(unsigned char),1,fp);
		fwrite(&nb,sizeof(unsigned int),1,fp);
		fwrite(&nt,sizeof(u_int64_t),1,fp);
		fwrite(&tnb,sizeof(u_int64_t),1,fp);
		fwrite(&comb_bn.numc,sizeof(unsigned int),1,fp);
		for (unsigned int nr=0;nr<tnb;nr++)
			fwrite(comb_bn.combnz[nr],b,1,fp);
		fwrite(MatDlx,nt*tnb,1,fp);
		fclose(fp);
	}

	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");
#endif	// WFILE

#ifdef WDEBUG
	wait=1;
	while (wait==1) {
		printf("Attesa debug\n");
		sleep(1);
	}
#endif	// WDEBUG

	header=dlx_alloc(MatDlx,tnb,nt);	// Passa la matrice MatDlx all'algoritmo DLX

	if (header==NULL) {			// Test se allocazione ram per algoritmo DLX riuscita
		printf("Ram insufficiente! (dlx_alloc)\n");
		return(1);
	}

	printf("\n");

	dlx_err_ram=0;				// Reset flag errore

	dlx_solve(header,cback,NULL);		// Ricerca tutte le coperture esatte
	dlx_free(header);

	if (dlx_err_ram==1)			// Test se ram insufficiente
		printf("Ram insufficiente! Il numero di soluzioni trovate e' limitato a %d\n\n",nsol);

	return(0);
}

/*				*/
/* Visualizza una soluzione	*/
/*				*/
/* INPUT: ns  numero soluzine	*/
/*        idx indice soluzione	*/
/*				*/
void showSol(unsigned int ns,unsigned int idx) {
	printf("Soluzione nr. %6d      ",ns+1);
	printf("{");
	for (unsigned int i=0;i<nb;i++) {			// loop sui blocchi di b elementi della soluzione
		printf("{");
		for (unsigned int j=0;j<b;j++) {		// loop su ogni elemento del blocco
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

/*					*/
/*    Visualizza tutte le soluzioni	*/
/*					*/
void showAllSol(void) {

	unsigned int idx;

	printf("\n");
	printf("\n");
	printf("Trovate %d soluzioni\n\n",nsol);
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	idx=0;
	for (unsigned int ns=0;ns<nsol;ns++) {		// Loop sulle soluzioni
		showSol(ns,idx);
		idx+=(nb*b);
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

	int esito;

	esito=0;

	if (init()==0 && search()==0)	// Se inizializzazione e ricerca okay
		showAllSol();		// visualizza le eventuali soluzioni
	else
		esito=1;		// Errore

	cleanUp();			// Rilascio memoria allocata dinamicamente

	return(esito);			// Ritorna esito
}
