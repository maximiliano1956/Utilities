#include <stdio.h>
#include <stdlib.h>

#include "dlx.h"

typedef struct {                // Struttura dinamica per le combinazioni
	char **combnz;          // Pointer matrice dinamica contenente la lista di combinazioni
	unsigned int numc;      // Numero di elementi in lista
} MATRIX;

u_int64_t size;			// Ram allocata

unsigned char t;
unsigned char b;
u_int64_t nt;
u_int64_t nb;
u_int64_t tnb;
u_int64_t bt;
int dlx_err_ram;


char *soluz;                    // Vettore dinamico contenente le soluzioni trovate
unsigned int nsol;              // nr soluzioni trovate
MATRIX comb_bn;                 // Combinazioni (b,n)
unsigned short *MatDlx;		// Vettore dinamico sparso utilizzata nell'algoritmo DLX

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
void deallocMatChar(char **mat,int nrow) {

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

	if ((vect=malloc(nel))==NULL)
		return(NULL);

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

	int idx;
	int idx_base;

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


/*								*/
/*	Routine che rilascia la ram allocata dinamicamente	*/
/*								*/
void deallocArr(void) {

	deallocMatChar(comb_bn.combnz,tnb);
	deallocVectChar((unsigned char *)MatDlx);

	if (soluz!=NULL)
		free(soluz);
}

/*						*/
/* Rilascio memoria allocata dinamicamente  	*/
/*						*/
void cleanUp(void) {

	deallocArr();
}

int main(char argc, char *argv[]) {

	FILE *fp;
	int nbyr;
	struct dlx_Node *header;

	size=0;
	soluz=NULL;
	nsol=0;

	if ((fp=fopen("./Steiner_snapshot.bin","rb"))!=NULL) {

		if ((nbyr=fread(&t,sizeof(unsigned char),1,fp))==0) {
			printf("Lettura dato t errata\n");
			fclose(fp);
			exit(1);
		}
		if ((nbyr=fread(&b,sizeof(unsigned char),1,fp))==0) {
			printf("Lettura dato b errata\n");
			fclose(fp);
			exit(1);
		}
		if ((nbyr=fread(&nb,sizeof(unsigned int),1,fp))==0) {
			printf("Lettura dato nb errata\n");
			fclose(fp);
			exit(1);
		}
		if ((nbyr=fread(&nt,sizeof(u_int64_t),1,fp))==0) {
			printf("Lettura dato nt errata\n");
			fclose(fp);
			exit(1);
		}
		if ((nbyr=fread(&tnb,sizeof(u_int64_t),1,fp))==0) {
			printf("Lettura dato tnb errata\n");
			fclose(fp);
			exit(1);
		}
		if ((nbyr=fread(&bt,sizeof(u_int64_t),1,fp))==0) {
			printf("Lettura dato tnb errata\n");
			fclose(fp);
			exit(1);
		}

		if ((comb_bn.combnz=allocMatChar(tnb,b))==NULL) {
			printf("Non riesco ad allocare comb_bn.combnz\n");
			fclose(fp);
			exit(1);
		}
		if ((MatDlx=(unsigned short *)allocVectChar((tnb*bt*2)*sizeof(unsigned short)))==NULL) {
			printf("Non riesco ad allocare MatDlx\n");
			cleanUp();
			fclose(fp);
			exit(1);
		}

		if (size>1024*1024*1024)
			printf("Ram dinamica allocata = %f Gbytes\n\n",(float)size/(1024*1024*1024));
		else if (size>1024*1024)
			printf("Ram dinamica allocata = %f Mbytes\n\n",(float)size/(1024*1024));
		else if (size>1024)
			printf("Ram dinamica allocata = %f Kbytes\n\n",(float)size/1024);
		else
			printf("Ram dinamica allocata = %f Bytes\n\n",(float)size);

		if ((nbyr=fread(&comb_bn.numc,sizeof(unsigned int),1,fp))==0) {
			printf("Lettura dato comb_bn.numc errata\n");
			cleanUp();
			fclose(fp);
			exit(1);
		}
		for (unsigned int nr=0;nr<tnb;nr++)
			if ((nbyr=fread(comb_bn.combnz[nr],b,1,fp))==0) {
				printf("Lettura dato comb_bn.combnz errata\n");
				cleanUp();
				fclose(fp);
				exit(1);
			}
		if ((nbyr=fread(MatDlx,(bt*tnb*2)*sizeof(unsigned short),1,fp))==0) {
			printf("Lettura dato MatDlx errata\n");
			cleanUp();
			fclose(fp);
			exit(1);
		}
		fclose(fp);
        }
	else {
		cleanUp();
		return(1);
	}

	header=dlx_alloc_sparse(MatDlx,tnb,nt,tnb*bt);	// Passa la matrice MatDlx all'algoritmo DLX

	if (header==NULL) {			// Test se allocazione ram per algoritmo DLX riuscita
		printf("Ram insufficiente! (dlx_alloc)\n");
		cleanUp();
		return(1);
	}

	printf("\n");

	dlx_err_ram=0;				// Reset flag errore

	dlx_solve(header,cback,NULL);		// Ricerca tutte le coperture esatte
	dlx_free(header);

	if (dlx_err_ram==1)			// Test se ram insufficiente
		printf("Ram insufficiente! Il numero di soluzioni trovate e' limitato a %d\n\n",nsol);

	cleanUp();
}
