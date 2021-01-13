#include <stdio.h>
#include <stdlib.h>

#define	MAXN	16			// Massima dimensione delle matrici
#define MAXSOL	100000			// Numero massimo elementi in ogni tipologia di gruppo

typedef struct {
	int matrix[MAXN][MAXN];
} MATRIX;

MATRIX m;				// Matrice corrente
int gl[MAXSOL][MAXN][MAXN];		// Lista elementi del gruppo GL
int sl[MAXSOL][MAXN][MAXN];		// Lista elementi del gruppo SL
int pgl[MAXSOL][MAXN][MAXN];		// Lista elementi del gruppo PGL
int psl[MAXSOL][MAXN][MAXN];		// Lista elementi del gruppo PSL
int ns;					// Indice in lista elementi del gruppo GL
int nss;				// Indice in lista elementi del gruppo SL
int pns;				// Indice in lista elementi del gruppo PGL
int pnss;				// Indice in lista elementi del gruppo PSL

int n;					// Dimensione matrice
int k;					// Ordine del campo (deve essere un numero primo >=2

int go;					// Controllo loop scansione matrici

// Funzione che stampa una matrice nxn con elementi nel campo {0,1,2,...,k-1}
//
void Stampa(void) {

	printf("\n");
	for (int i=0;i<n;i++) {
		for (int j=0;j<n;j++)
			printf("%d ",m.matrix[i][j]);
		printf("\n");
	}
}

// Funzione che carica il cofattore di mat[p][q] in temp[][]. dim e' la dimensione corrente di mat[][]
//
void getCofactor(MATRIX *mat, MATRIX *temp, int p, int q, int dim) {

	int i=0;
	int j=0;
 
	// Loop su ogni elemento della matrice
	for (int row=0;row<dim;row++)
		for (int col=0;col<dim;col++)
			//  Si copia nella matrice temporanea solo quegli elementi che non si trovano nella riga o colonna data
			if (row!=p && col!=q) {
				temp->matrix[i][j++]=mat->matrix[row][col];
 
				// Una riga e' stata copiata, percio' incrementa l'indice di riga e azzera l'indice di colonna
				if (j==dim-1) {
					j=0;
					i++;
					}
				}
}
 
// Funzione ricorsiva per calcolare il determinante di una matrice. dim e' la dimensione corrente di mat[][].
//
int determinantOfMatrix(int dim,MATRIX *mat)
{
	int D=0;							// Inizializza il valore del determinante
	int sign=1;							// Inizializza il registro del segno
	MATRIX temp;
 
	//  Caso base : la matrice e' di dimensione 1
	if (dim==1)
    		return(mat->matrix[0][0]);
 
	// Itera per ogni elemento della prima riga
	for (int f=0;f<dim;f++) {
		// Calcola il co-fattore di mat[0][f]
		getCofactor(mat,&temp,0,f,dim);

		// Aggiorna il valore del determinante
		D+=sign*mat->matrix[0][f]*determinantOfMatrix(dim-1,&temp);
 
		// terms are to be added with alternate sign
		sign=-sign;
		}
 
return(D);
}

//	Controlla se la matrice corrente e' ottenibile come prodotto
//	di uno scalare per una matrice di GL
//
//	OUTPUT:		0		la matrice e' ottenibile
//	OUTPUT:		1		la matrice non e' ottenibile
//
int checkPG(void) {

	int esci;

	for (int inds=0;inds<ns;inds++)
		for (int l=2;l<k;l++) {
			esci=0;
			for (int i=0;i<n;i++)
				if (esci==0) {
					for (int j=0;j<n;j++)
						if (m.matrix[i][j]!=(l*gl[inds][i][j])%k) {
							esci=1;
							break;
						}
				}
				else
					break;
			if (esci==0)
				return(0);
		}
	return(1);
}

//	Controlla se la matrice corrente e' ottenibile come prodotto
//	di uno scalare per una matrice di SL.
//	Dovendo avere determinante unitario l'unica possibilita'
//	che cio' avvenga e' che ESSENDO n PARI la matrice corrente sia
//	(-1) * una matrice di SL , cioe' sia una matrice di SL con elementi di segno opposto
//
//	OUTPUT:		0		la matrice e' ottenibile
//	OUTPUT:		1		la matrice non e' ottenibile
//
int checkPS(void) {

	int esci;

	if (n%2==1)			// se n non e' pari non sara' possibile per nessuna matrice
		return(1);

	for (int inds=0;inds<nss;inds++) {
		esci=0;
		for (int i=0;i<n;i++)
			if (esci==0) {
				for (int j=0;j<n;j++)
					if ((m.matrix[i][j]+sl[inds][i][j])%k!=0) {
						esci=1;
						break;
					}
			}
			else
				break;
		if (esci==0)
			return(0);
	}
	return(1);
}

//	Questa routine calcola tutti gli elementi di GL(n,k) PGL(n,k) SL(n,k) PSL(n,k)
//
int main(char argc,char *argv[]) {

	int nr;		// Nr. riga corrente
	int nc;		// Nr. colonna corrente
	int nm;		// Counter matrici esaminate
	int det;	// Appoggio per il valore del determinante

	n=3;		// Dimensioni della matrice
	k=3;		// Numero simboli nel campo (deve essere un numero primo >= 2)

	if (n>MAXN) {
		printf("n is greater than %d!\n",MAXN);
		exit(1);
	}

	nm=0;		// Init counter matrici esaminate

	ns=0;		// Init counter matrici in GL
	nss=0;		// Init counter matrici in SL
	pns=0;		// Init counter matrici in PGL
	pnss=0;		// Init counter matrici in PSL

	for (int i=0;i<n;i++)
		for (int j=0;j<n;j++)
			m.matrix[i][j]=0;		// Init matrice corrente

	go=1;						// Attiva il loop

	while (go) {

		nr=0;		// Init riga corrente = 0
		nc=0;		// Init colonna corrente = 0

		nm++;		// Aggiorna counter matrici esaminate

		Stampa();

		det=determinantOfMatrix(n,&m)%k;	// Calcola il determinante della matrice (nel campo avente k elementi)

		while(det<0)
			det+=k;				// Se il determinante e' negativo lo riporta nel range {0,k-1}
  
		if (det!=0) {				// Accetta la matrice solo se ha determinante non nullo (cioe' e' invertibile)
			if (det==1) {			// Se la matrice e' uguale a 1 e' un elemento di SL (oltre che di GL)
				if (nss>=MAXSOL) {
					printf("Not enough memory!\n");
					exit(1);
				}
				for (int i=0;i<n;i++)
					for (int j=0;j<n;j++)
						sl[nss][i][j]=m.matrix[i][j];		// Carica la matrice nella lista SL
				nss++;							// Aggiorna indice a lista

				if (checkPS()) {					// Check se matrice in PSL
					if (pnss>=MAXSOL) {
						printf("Not enough memory!\n");
						exit(1);
					}
					for (int i=0;i<n;i++)
						for (int j=0;j<n;j++)
							psl[pnss][i][j]=m.matrix[i][j];	// Carica la matrice nella lista PSL
					pnss++;						// Aggiorna indice a lista
				}
			}

			if (ns>=MAXSOL) {
				printf("Not enough memory!\n");
				exit(1);
			}

			for (int i=0;i<n;i++)
				for (int j=0;j<n;j++)
					gl[ns][i][j]=m.matrix[i][j];			// Carica la matrice nella lista GL
			ns++;								// Aggiorna indice a lista

			if (checkPG()) {						// Check se matrice in PGL
				if (pns>=MAXSOL) {
					printf("Not enough memory!\n");
					exit(1);
				}
				for (int i=0;i<n;i++)
					for (int j=0;j<n;j++)
						pgl[pns][i][j]=m.matrix[i][j];		// Carica la matrice nella lista PGL
				pns++;							// Aggiorna indice a lista
			}

		}

		while (1) {	// Loop secondario di incremento elemento con gestione carry eventualmente multiplo
			m.matrix[nr][nc]=(m.matrix[nr][nc]+1)%k;			// Incrementa elemento corrente della matrice
			if (m.matrix[nr][nc]==0) {					// Test se carry all'elemento successivo
				nc=(nc+1)%n;						// in tal caso passa alla colonna successiva
				if (nc==0) {						// ed eventualmente...
					nr=(nr+1)%n;					// alla riga successiva
					if (nr==0) {					// se ultima riga..
						go=0;					// fine scansione: disattiva il loop principale
						break;					// esce comunque dal loop secondario
					}
				}
			}
			else
				break;							// se no carry ritorna al loop principale
		}

	}

	printf("\n");

	printf("GL\n");						// Stampa lista matrici in GL(n,k)
	for (int inds=0;inds<ns;inds++) {
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++)
				printf("%d ",gl[inds][i][j]);
			printf("\n");
		}
		printf("%d\n",inds+1);
		printf("\n");
	}

	printf("SL\n");						// Stampa lista matrici in SL(n,k)
	for (int inds=0;inds<nss;inds++) {
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++)
				printf("%d ",sl[inds][i][j]);
			printf("\n");
		}
		printf("%d\n",inds+1);
		printf("\n");
	}

	printf("PGL\n");					// Stampa lista matrici in PGL(n,k)
	for (int inds=0;inds<pns;inds++) {
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++)
				printf("%d ",pgl[inds][i][j]);
			printf("\n");
		}
		printf("%d\n",inds+1);
		printf("\n");
	}

	printf("PSL\n");					// Stampa lista matrici in PSL(n,k)
	for (int inds=0;inds<pnss;inds++) {
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++)
				printf("%d ",psl[inds][i][j]);
			printf("\n");
		}
		printf("%d\n",inds+1);
		printf("\n");
	}

	printf("\n\nRiassunto:\n\n");
	printf("GL(%d,%d)  order = %d\n",n,k,ns);
	printf("PGL(%d,%d) order = %d\n",n,k,pns);
	printf("SL(%d,%d)  order = %d\n",n,k,nss);
	printf("PSL(%d,%d) order = %d\n",n,k,pnss);
	printf("\n");

	return(0);
}
