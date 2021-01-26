#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "dlx.h"

#define OPZN	"-n="
#define OPZB	"-b="
#define OPZT	"-t="

unsigned char n;
unsigned char b;
unsigned char t;

unsigned int nb;
char *soluz;			// Vettore dinamico contenente le soluzioni trovate
unsigned int nsol;		// nr soluzioni trovate
u_int64_t size;			// Size della ram allocata dinamicamente

int keepRunning;		// Flag CTRL-C pressed

char filename[512];		// nome file soluzioni


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


/*					*/
/*    Visualizza tutte le soluzioni	*/
/*					*/
void showAllSol(void) {

	unsigned int idx;

	printf("\n");
	printf("\n");
	printf("Trovate %d soluzioni\n\n",nsol);

	keyPress();

	idx=0;
	for (unsigned int ns=0;ns<nsol;ns++) {		// Loop sulle soluzioni
		if (ns==0) {
			printf("Prima soluzione trovata!\n");
			printf("\n");
			showSol(ns,idx);			// Visualizza prima soluzione
			printf("Ora visualizzo le altre... (CTRL-C per uscire)\n");
			keyPress();
		}
		else
			showSol(ns,idx);			// Visualizza la soluzione

		idx+=(nb*b);

		if (keepRunning==0) {				// Test se CTRL-C premuto
			printf("Rilevato CTRL-C: uscita\n");
			return;
		}
	}

	printf("\n");
	printf("\n");
}


/*								*/
/*	Routine che rilascia la ram allocata dinamicamente	*/
/*								*/
void deallocArr(void) {

	deallocVectChar(soluz);
}

/*						*/
/* Rilascio memoria allocata dinamicamente  	*/
/*						*/
void cleanUp(void) {

	deallocArr();
}


/*				*/
/*	Stampa parametri	*/
/*				*/
void stampaPar(void) {

	// Intestazione
  
	printf("\n");
	printf("Steiner System (%d,%d,%d)\n",t,b,n);
	printf("\n");
	printf("Nome file soluzioni: %s\n",filename);
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
	}

	if (errore==1)
		help();

	return(errore);
}


/*				*/
/*	Carica defaults		*/
/*				*/
void carDef(void) {

	n=7;
	b=3;
	t=2;
}


int main(char argc, char *argv[]) {

	FILE *fp;
	int nbyr;
	char num[16];
	int esito;

	struct sigaction act;

	keepRunning=1;                  // Default run

	carDef();

	esito=getOptions(argc,argv);	// Gestione opzioni

	if (esito==1) {
	
		printf("\n");
		printf("Bye!\n");
		printf("\n");

		return(1);
	}

	strcpy(filename,"Steiner");
	sprintf(num,"_%d",t);
	strcat(filename,num);
	sprintf(num,"_%d",b);
	strcat(filename,num);
	sprintf(num,"_%d",n);
	strcat(filename,num);
	strcat(filename,"_solutions.bin");

	stampaPar();				// Stampa i parametri dello Steiner System

	keyPress();

	size=0;

	printf("\nLettura file %s\n",filename);

	if ((fp=fopen(filename,"rb"))!=NULL) {

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
		if ((nbyr=fread(&nsol,sizeof(unsigned int),1,fp))==0) {
			printf("Lettura dato nsol errata\n");
			fclose(fp);
			exit(1);
		}

		if ((soluz=allocVectChar(b*nb*nsol))==NULL) {
			printf("Non riesco ad allocare soluz\n");
			cleanUp();
			fclose(fp);
			exit(1);
		}

		if ((nbyr=fread(soluz,b*nb*nsol,1,fp))==0) {
			printf("Lettura matrice soluz errata\n");
			fclose(fp);
			exit(1);
		}

		fclose(fp);
        }
	else {
		printf("\n");
		printf("Non riesco a leggere il file %s!\n",filename);
	
		printf("\n");
		printf("Bye!\n");
		printf("\n");

		cleanUp();
		return(1);
	}

	printf("\n");
	StampaSize();				// Stampa ram totale allocata
	printf("\n");

	keyPress();

	act.sa_handler=intHandler;
	sigaction(SIGINT,&act,NULL);		// Trap CTRL-C

	showAllSol();

	cleanUp();
	
	printf("\n");
	printf("Bye!\n");
	printf("\n");
}
