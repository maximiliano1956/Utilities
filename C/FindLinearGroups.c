#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>

#define	MAXN	16
#define MAXK	3
#define MAXSOL	10000

typedef struct {
	int matrix[MAXN][MAXN];
} MATRIX;

MATRIX m;
int gl[MAXSOL][MAXN][MAXN];
int sl[MAXSOL][MAXN][MAXN];
int pgl[MAXSOL][MAXN][MAXN];
int psl[MAXSOL][MAXN][MAXN];
int ns;
int nss;
int pns;
int pnss;

int n;
int k;

int go;

int calcDet(void);

void Stampa(void) {

	printf("\n");
	for (int i=0;i<n;i++) {
		for (int j=0;j<n;j++)
			printf("%d ",m.matrix[i][j]);
		printf("\n");
	}
}

// Function to get cofactor of mat[p][q] in temp[][]. dim is current dimension of mat[][]
void getCofactor(MATRIX *mat, MATRIX *temp, int p, int q, int dim) {

	int i=0;
	int j=0;
 
	// Looping for each element of the matrix
	for (int row=0;row<dim;row++)
		for (int col=0;col<dim;col++)
			//  Copying into temporary matrix only those element which are not in given row and column
			if (row!=p && col!=q) {
				temp->matrix[i][j++]=mat->matrix[row][col];
 
				// Row is filled, so increase row index and reset col index
				if (j==dim-1) {
					j=0;
					i++;
					}
				}
}
 
// Recursive function for finding determinant of matrix. dim is current dimension of mat[][].
int determinantOfMatrix(int dim,MATRIX *mat)
{
	int D=0;							// Initialize result
	int sign=1;							// To store sign multiplier
	MATRIX temp;
 
	//  Base case : if matrix contains single element
	if (dim==1)
    		return(mat->matrix[0][0]);
 
	// Iterate for each element of first row
	for (int f=0;f<dim;f++) {
		// Getting Cofactor of mat[0][f]
		getCofactor(mat, &temp, 0, f, dim);
		D+=sign*mat->matrix[0][f]*determinantOfMatrix(dim-1,&temp);
 
		// terms are to be added with alternate sign
		sign=-sign;
		}
 
return(D);
}


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

int checkPS(void) {

	int esci;

	if (n%2==1)
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

int main(char argc,char *argv[]) {

	int nr;
	int nc;
	int nm;
	int det;

	n=2;
	k=7;

	if (n>MAXN) {
		printf("n is greater than %d!\n",MAXN);
		exit(1);
	}

	nm=0;
	ns=0;
	nss=0;
	pns=0;
	pnss=0;

	for (int i=0;i<n;i++)
		for (int j=0;j<n;j++)
			m.matrix[i][j]=0;

	go=1;

	while (go) {

		nr=0;
		nc=0;

		nm++;

		Stampa();

		det=determinantOfMatrix(n,&m)%k;

		while(det<0)
			det+=k;
  
		if (det!=0) {
			if (det==1) {
				if (nss>=MAXSOL) {
					printf("Not enough memory!\n");
					exit(1);
				}
				for (int i=0;i<n;i++)
					for (int j=0;j<n;j++)
						sl[nss][i][j]=m.matrix[i][j];
				nss++;

				if (checkPS()) {
					if (pnss>=MAXSOL) {
						printf("Not enough memory!\n");
						exit(1);
					}
					for (int i=0;i<n;i++)
						for (int j=0;j<n;j++)
							psl[pnss][i][j]=m.matrix[i][j];
					pnss++;
				}
			}

			if (ns>=MAXSOL) {
				printf("Not enough memory!\n");
				exit(1);
			}

			for (int i=0;i<n;i++)
				for (int j=0;j<n;j++)
					gl[ns][i][j]=m.matrix[i][j];
			ns++;

			if (checkPG()) {
				if (pns>=MAXSOL) {
					printf("Not enough memory!\n");
					exit(1);
				}
				for (int i=0;i<n;i++)
					for (int j=0;j<n;j++)
						pgl[pns][i][j]=m.matrix[i][j];
				pns++;
			}

		}

		while (1) {
			m.matrix[nr][nc]=(m.matrix[nr][nc]+1)%k;
			if (m.matrix[nr][nc]==0) {
				nc=(nc+1)%n;
				if (nc==0) {
					nr=(nr+1)%n;
					if (nr==0) {
						go=0;
						break;
					}
				}
			}
			else
				break;
		}

	}

	printf("\n");

	printf("GL\n");
	for (int inds=0;inds<ns;inds++) {
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++)
				printf("%d ",gl[inds][i][j]);
			printf("\n");
		}
		printf("%d\n",inds+1);
		printf("\n");
	}

	printf("SL\n");
	for (int inds=0;inds<nss;inds++) {
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++)
				printf("%d ",sl[inds][i][j]);
			printf("\n");
		}
		printf("%d\n",inds+1);
		printf("\n");
	}

	printf("PGL\n");
	for (int inds=0;inds<pns;inds++) {
		for (int i=0;i<n;i++) {
			for (int j=0;j<n;j++)
				printf("%d ",pgl[inds][i][j]);
			printf("\n");
		}
		printf("%d\n",inds+1);
		printf("\n");
	}

	printf("PSL\n");
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
	printf("GL\n");
	printf("ns = %d\n\n",ns);
	printf("PGL\n");
	printf("pns = %d\n\n",pns);
	printf("SL\n");
	printf("nss = %d\n\n",nss);
	printf("PSL\n");
	printf("pnss = %d\n\n",pnss);

	return(0);
}
