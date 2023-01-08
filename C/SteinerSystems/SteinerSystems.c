#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "dlx.h"

#define OPZN	"-n="
#define OPZB	"-b="
#define OPZT	"-t="
#define OPZV	"-v"
#define OPZWSN	"-wsn"
#define OPZWSO	"-wso"
#define OPZD	"-d"

#define LENNAME	256


/*
 * Ricerca sistemi di Steiner (t,b,n)
*/


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
unsigned short *MatDlx;		// Vettore dinamico utilizzata nell'algoritmo DLX

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

int keepRunning;		// Flag CTRL-C pressed

char verbose;			// se = 1 attiva delle stampe di debug
char wsnap;			// se = 1 attiva la scrittura dello snapshot
char wsol;			// se = 1 attiva la scrittura del file delle soluzioni
char deb;			// se = 1 attiva la il wait per il debug e alcune stampe

unsigned char dlx_err_ram;	// errore rilevato in callback dell'algoritmo DLX

char fname_sn[256];		// nome file snapshot
char fname_so[256];		// nome file soluzioni
char lbl_tbn[16];		// etichetta "t_b_n"

void showSol(unsigned int ns,unsigned int idx);


/*						*/
/* Routine che attende la pressione di un tasto	*/
/*						*/
void keyPress(void) {
	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");
}

/*							*/
/* Routine che stampa il size della ram totale allocata	*/
/*							*/
void StampaSize(void) {

	char str_size[]={'B','K','M','G'};
	float sz;

	sz=(float)size;

	printf("Ram totale allocata = ");

	for (int ind=0;ind<4;ind++) {

		if (sz<1024) {
			printf("%f %cbytes",sz,str_size[ind]);
			break;
		}

		sz/=1024;
	}

	printf("\n\n");

	if (sz>=1024)
		printf("Ram size too large!\n");
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
int allocMem(char *strerr) {

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

	MatDlx=(unsigned short *)allocVectChar((tnb*bt*2)*sizeof(unsigned short));
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

	StampaSize();		// Stampa ram totale allocata

	return(0);
}

/*								*/
/*	Routine che rilascia la ram allocata dinamicamente	*/
/*								*/
void deallocMem(void) {

	deallocMatChar(comb_bn.combnz,tnb);
	deallocMatChar(comb_tn.combnz,nt);
	deallocMatChar(comb_tb.combnz,bt);

	deallocMatInt(set_tb,tnb);
	deallocVectChar((unsigned char *)MatDlx);
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

	if (nsol==0) {
		printf("Prima soluzione trovata!\n");
		printf("\n");
		showSol(nsol,idx_base);			// Visualizza prima soluzione
		printf("Ora visualizzo le altre... (CTRL-C per uscire)\n");
		keyPress();
	}
	else
		showSol(nsol,idx_base);			// Visualizza la soluzione

	nsol++;						// aggiorna il numero di soluzioni trovate

	if (keepRunning==0) {				// Test se CTRL-C premuto
		printf("Rilevato CTRL-C: uscita\n");
		return(1);
	}

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
	keyPress();

	// Alloca ram dinamica

	if (allocMem(str)==1) {
		printf("Non riesco ad allocare %s!\n",str);
		return(1);
	}

	return(0);
}

/*						*/
/* Rilascio memoria allocata dinamicamente  	*/
/*						*/
void cleanUp(void) {

	deallocMem();
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
	int wait;
	FILE *fp;

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
		if (keepRunning==0) {
			printf("Rilevato CTRL-C ==> abort\n");
			return(1);
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
	for (unsigned int i=0;i<tnb;i++) {		// loop su blocchi di b elementi sugli n totali
		for (unsigned int j=0;j<bt;j++) {	// per ogni sottocombinazione di t elementi
			if (set_tb[i][j]>=nt) {
				printf("Dati incoerenti!\n!");
				return(1);
			}
			MatDlx[idx++]=i;
			MatDlx[idx++]=set_tb[i][j];	// Indicatore che la combinazione (t,n) appartiene al blocco (b,n)
		}
	}

	if (deb==1) {
		printf("\n");
		printf("\n");
		printf("Matrice sparsa MatDlx\n");
		printf("\n");
		idx=0;
		for (unsigned int i=0;i<tnb;i++) {
			for (unsigned int j=0;j<bt;j++) {
				printf("(%2d,%2d) ",MatDlx[idx],MatDlx[idx+1]);
				idx+=2;
			}
			printf("\n");
		}
	}

	if (wsnap==1) {
		printf("\n");
		printf("Scrittura snapshot %s\n",fname_sn);

		if ((fp=fopen(fname_sn,"wb+"))!=NULL) {
			fwrite(&t,sizeof(unsigned char),1,fp);
			fwrite(&b,sizeof(unsigned char),1,fp);
			fwrite(&nb,sizeof(unsigned int),1,fp);
			fwrite(&nt,sizeof(u_int64_t),1,fp);
			fwrite(&tnb,sizeof(u_int64_t),1,fp);
			fwrite(&bt,sizeof(u_int64_t),1,fp);
			fwrite(&comb_bn.numc,sizeof(unsigned int),1,fp);
			for (unsigned int nr=0;nr<tnb;nr++)
				fwrite(comb_bn.combnz[nr],b,1,fp);
			fwrite(MatDlx,(tnb*bt*2)*sizeof(unsigned short),1,fp);
			fclose(fp);
		}
		else
			printf("Non riesco a creare il file %s!\n",fname_sn);

		keyPress();
	}

	if (deb==1) {

		wait=1;
		while (wait==1) {
			printf("Attesa debug\n");
			sleep(1);
		}
	}

	printf("Allocazione ram per pacchetto DLX\n");

	header=dlx_alloc_sparse(MatDlx,tnb,nt,tnb*bt);	// Passa la matrice sparsa MatDlx all'algoritmo DLX

	if (header==NULL) {			// Test se allocazione ram per algoritmo DLX riuscita
		printf("Ram insufficiente! (dlx_alloc)\n");
		return(1);
	}

	StampaSize();

	printf("Ora inizia la ricerca DLX\n");

	keyPress();

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

	int count_ch;

	count_ch=0;

	printf("Soluzione nr. %6d\n",ns+1);
	for (unsigned int i=0;i<nb;i++) {			// loop sui blocchi di b elementi della soluzione

		if (count_ch+(2+b*2)>=80) {
			printf("\n");
			count_ch=0;
		}

		printf("{");
		for (unsigned int j=0;j<b;j++) {		// loop su ogni elemento del blocco
			printf("%2d",soluz[idx++]);
			if (j<b-1)
				printf(",");
		}
		printf("}");

		count_ch+=(2+b*2);

		if (i<nb-1) {
			printf(",");
			count_ch++;
		}
	}
	printf("\n\n");
}

/*					*/
/*    Visualizza tutte le soluzioni	*/
/*					*/
void showAllSol(void) {

	unsigned int idx;

	if (verbose==0)
		return;

	printf("\n");
	printf("\n");
	printf("Trovate %d soluzioni\n\n",nsol);

	keyPress();

	idx=0;
	for (unsigned int ns=0;ns<nsol;ns++) {		// Loop sulle soluzioni
		showSol(ns,idx);
		idx+=(nb*b);
	}

	printf("\n");
	printf("\n");
}


/*						*/
/*      Salva tutte le soluzioni in un file	*/
/*						*/
void writeSol(void) {

	FILE *fp;

	if (wsol==0 || nsol==0)
		return;

	printf("\n");
	printf("Scrittura file soluzioni %s\n",fname_so);

	if ((fp=fopen(fname_so,"wb+"))!=NULL && nsol>0) {
		fwrite(&b,sizeof(unsigned char),1,fp);
		fwrite(&nb,sizeof(unsigned int),1,fp);
		fwrite(&nsol,sizeof(unsigned int),1,fp);
		fwrite(soluz,nb*b*nsol,1,fp);
		fclose(fp);
		}
	else
		printf("Non riesco a creare il file %s!\n",fname_so);
}


/*				*/
/*	Trapping CTRL-C		*/
/*				*/
void intHandler(int s) {
	keepRunning=0;		// Flags stop
}


/*				*/
/*	  Stampa help		*/
/*				*/
void help(void) {
	printf("\n");
	printf("Optioni:\n");
	printf("%snumero simboli disponibili\n",OPZN);
	printf("%snumero simboli in un blocco\n",OPZB);
	printf("%snumero simboli nella combinazione da coprire\n",OPZT);
	printf("%s attiva modo verbose\n",OPZV);
	printf("%s attiva la scrittura del file di snapshot\n",OPZWSN);
	printf("%s attiva la scrittura del file delle soluzioni\n",OPZWSO);
	printf("%s attiva la scrittura del file delle soluzioni\n",OPZD);
}


/*				*/
/*	  Parse opzioni		*/
/*				*/
/* OUTPUT= 0	OKAY		*/
/*       = 1	ERRORE		*/
/*				*/
int getOptions(char argc,char *argv[]) {

	int intval;
	int errore;

	errore=0;

	for (int opt=1;opt<argc;opt++) {

		if (!strncmp(argv[opt],OPZN,strlen(OPZN))) {
			if (sscanf(argv[opt]+strlen(OPZN),"%d",&intval)!=1) {
				errore=1;
				break;
			}
			n=intval;
		}

		if (!strncmp(argv[opt],OPZB,strlen(OPZB))) {
			if (sscanf(argv[opt]+strlen(OPZB),"%d",&intval)!=1) {
				errore=1;
				break;
			}
			b=intval;
		}

		if (!strncmp(argv[opt],OPZT,strlen(OPZT))) {
			if (sscanf(argv[opt]+strlen(OPZT),"%d",&intval)!=1) {
				errore=1;
				break;
			}
			t=intval;
		}

		if (!strncmp(argv[opt],OPZV,strlen(OPZV)))
			verbose=1;

		if (!strncmp(argv[opt],OPZWSN,strlen(OPZWSN)))
			wsnap=1;

		if (!strncmp(argv[opt],OPZWSO,strlen(OPZWSO)))
			wsol=1;

		if (!strncmp(argv[opt],OPZD,strlen(OPZD)))
			deb=1;
	}

	if (errore==1)
		help();

	return(errore);
}

/*				*/
/*	Stampa parametri	*/
/*				*/
void stampaPar(void) {

	printf("\n");
	printf("Parametri impostati:\n");
	printf("\n");
	printf("t=%d\n",t);
	printf("b=%d\n",b);
	printf("n=%d\n",n);
	printf("\n");
	printf("verbose=%d\n",verbose);
	printf("Scrittura shapshot =%d\n",wsnap);
	printf("Scrittura soluzioni=%d\n",wsol);
	printf("Debug attivo=%d\n",deb);
	printf("\n");
	printf("Nome file snapshot: %s\n",fname_sn);
	printf("Nome file soluzioni: %s\n",fname_so);
}

/*				*/
/*	Carica defaults		*/
/*				*/
void carDef(void) {

	n=7;
	b=3;
	t=2;		// Steiner system (2,3,7)

	verbose=0;	// no verbose
	wsnap=0;	// no scrittura snapshot
	wsol=0;		// no scrittura file soluzioni
	deb=0;		// no debug
}


/*				*/
/*	Genera nomi files	*/
/*				*/
void genNomi(void) {

	char num[16];

	sprintf(lbl_tbn,"Steiner_%d_",t);
	sprintf(num,"%d_",b);
	strcat(lbl_tbn,num);
	sprintf(num,"%d",n);
	strcat(lbl_tbn,num);

	strcpy(fname_sn,lbl_tbn);
	strcat(fname_sn,"_snapshot.bin");

	strcpy(fname_so,lbl_tbn);
	strcat(fname_so,"_solutions.bin");
}


/*				*/
/*	Routine principale	*/
/*				*/
/* OUTPUT:	0	OKAY	*/	
/*		1	ERRORE	*/
/*				*/
int main(char argc,char *argv[]) {

	int esito;

	struct sigaction act;

	keepRunning=1;			// Default run

	carDef();			// Carica valori di default

	if ((esito=getOptions(argc,argv))==0) {	// Gestione opzioni

		genNomi();			// Genera nomi files

		stampaPar();			// Stampa i parametri impostati

		keyPress();

		act.sa_handler=intHandler;
		sigaction(SIGINT,&act,NULL);	// Trap CTRL-C

		esito=0;

		if (init()==0 && search()==0)	// Se inizializzazione e ricerca okay
			showAllSol();		// visualizza le eventuali soluzioni
		else
			esito=1;		// Errore

		writeSol();			// Salva le soluzioni in un file
	
		cleanUp();			// Rilascio memoria allocata dinamicamente
	}
	
	printf("\n");
	printf("Bye!\n");
	printf("\n");

	return(esito);			// Ritorna esito
}
