#include <stdio.h>
#include <stdlib.h>

#define MAXDIM	24
#define MAXC	10000000

int dim;
int b;
int t;
int ind[MAXDIM];
int set[MAXDIM];

typedef struct {
        char combnz[MAXC][MAXDIM];
	int numc;
} MATRIX;

MATRIX comb_nt;
MATRIX comb_nb;
MATRIX comb_bt;

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

void Scan2(int nn,int nsym,int n,int ii,MATRIX *m) {

	if (n==nn) {
		if (m->numc>=MAXC) {
			printf("Not enough memory!\n");
			exit(1);
		}

		for (int i=0;i<nn;i++)
			m->combnz[m->numc][i]=set[ind[i]-1];
		m->numc++;
		return;
	}

	for (int i=ii;i<=nsym;i++) {
		ind[n]=i;

		Scan2(nn,nsym,n+1,i+1,m);
	}
}

void Scan(int order,int nsym,MATRIX *m) {

	m->numc=0;
	Scan2(order,nsym,0,1,m);
}


int main(char argc,char *argv[]) {

	int nb;

	t=2;
	b=3;
	dim=7;

// Steiner system (2,3,7)
//
	printf("\n");

	if (Comb(dim,t)%Comb(b,t)!=0) {
		printf("It sistema di Steiner (%d,%d,%d) non puo esistere!\n",t,b,dim);
	}

	nb=Comb(dim,t)/Comb(b,t);

	printf("Inizio ricerca sistema di Steiner (%d,%d,%d)\n",t,b,dim);
	printf("%d blocchi da %d elementi\n",nb,b);
	printf("\n");

	for (int i=0;i<dim;i++)
		set[i]=i+1;

	Scan(b,dim,&comb_nb);

	printf("\n");
	printf("(%d,%d)\n",b,dim);

	for (int i=0;i<comb_nb.numc;i++) {
		for (int j=0;j<b;j++) {
			printf("%d ",comb_nb.combnz[i][j]);
		}
		printf("\n");
	}
	printf("Nr. elementi = %d\n",comb_nb.numc);

	Scan(t,dim,&comb_nt);

	printf("\n");
	printf("(%d,%d)\n",t,dim);

	for (int i=0;i<comb_nt.numc;i++) {
		for (int j=0;j<t;j++) {
			printf("%d ",comb_nt.combnz[i][j]);
		}
		printf("\n");
	}
	printf("Nr. elementi = %d\n",comb_nt.numc);

	Scan(t,b,&comb_bt);

	printf("\n");
	printf("(%d,%d)\n",t,b);

	for (int i=0;i<comb_bt.numc;i++) {
		for (int j=0;j<t;j++) {
			printf("%d ",comb_bt.combnz[i][j]);
		}
		printf("\n");
	}
	printf("Nr. elementi = %d\n",comb_bt.numc);
	printf("\n");

	return(0);
}
