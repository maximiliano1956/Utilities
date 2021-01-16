#include <stdio.h>
#include <stdlib.h>

#define MAXDIM	24

int dim;
int ind[MAXDIM];
int set[MAXDIM];
int numc;

char symbols[MAXDIM];

unsigned int CaileyTable[MAXDIM][MAXDIM];

int Comb(int n,int k) {

	if (n == 0)
		return(1);

	if (k==0 || k==n)
		return(1);
	if (k==1 || k==(n-1))
		return(n);

	return(Comb(n-1,k -1)+Comb(n-1,k));
}

int checkEl(int n,int el[],int l) {

	if (l==1)
		return(1);

	for (int ind=0;ind<n;ind++)
		if (l==el[ind])
			return(1);
	return(0);
}

void Scan2(int nn,int n,int ii) {

	if (n==nn) {
		for (int i=0;i<nn;i++)
			printf("%c ",symbols[set[ind[i]-1]-1]);
		printf("\n");
		numc++;
		return;
	}

	for (int i=ii;i<=dim;i++) {
		ind[n]=i;

		Scan2(nn,n+1,i+1);
	}
}

void Scan(int order) {

	printf("\n");
	printf("order=%d\n",order);
	printf("\n");
	Scan2(order,0,1);
}


int main(char argc,char *argv[]) {

	int th;
	int errore;
	char symb;

	errore=0;

	dim=24;

	symb='a';

	for (int i=0;i<dim;i++)
		symbols[i]=symb+i;

	/* ad esempio */

	for (int i=0;i<dim;i++)
		set[i]=dim-i;

	printf("\n");
	printf("\n");
	printf("%d symbols : {",dim);
	for (int i=0;i<dim;i++) {
		if (i>0)
			printf(" ");
		printf("%c",symbols[set[i]-1]);
	}
	printf("}\n");
	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	for (int nn=1;nn<=dim;nn++) {

		numc=0;
		Scan(nn);

		th=Comb(dim,nn);

		printf("\n");
		printf("Nr. sottoinsiemi di %d simboli da un insieme di %d simboli = %d\n",nn,dim,numc);
		printf("Valore teorico = %d",th);

		if (numc==th)
			printf("  ====> OKAY!\n");
		else {
			printf("  ====> ERROR!\n");
			errore=1;
		}

		printf("\n");
		printf("Premere un tasto ");
		getchar();
	}

	printf("\n");
	printf("===================\n");
	printf("RESULTS:\n");
	if (errore==0)
		printf("NO ERRORS!\n");
	else
		printf("SOME ERRORS!\n");
	printf("===================\n");
	printf("\n");

	return(0);
}
