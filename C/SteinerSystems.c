#include <stdio.h>
#include <stdlib.h>

#define MAXDIM	24
#define MAXC	10000000

int dim;
int b;
int t;
int ind[MAXDIM];

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

void Scan2(int nn,int nsym,int n,int ii,MATRIX *m,int set[]) {

	if (n==nn) {
		if (m->numc>=MAXC) {
			printf("Not enough memory!\n");
			exit(1);
		}

		for (int i=0;i<nn;i++)
			m->combnz[m->numc][i]=(set!=NULL) ? set[ind[i]-1]:ind[i];

		m->numc++;
		return;
	}

	for (int i=ii;i<=nsym;i++) {
		ind[n]=i;

		Scan2(nn,nsym,n+1,i+1,m,set);
	}
}

void Scan(int order,int nsym,MATRIX *m,int append,int set[]) {

	if (append==0)
		m->numc=0;
	Scan2(order,nsym,0,1,m,set);

	printf("\n");
	printf("(%d,%d)\n",order,nsym);

	for (int i=0;i<m->numc;i++) {
		for (int j=0;j<order;j++)
			printf("%d ",m->combnz[i][j]);
		printf("\n");
	}
	printf("Nr. elementi = %d\n",m->numc);
}


int main(char argc,char *argv[]) {

	int nb;
	int set[]={7,6,5,4,3,2,1};

	t=2;
	b=3;
	dim=7;

// Steiner system (2,3,7)
//
	printf("\n");

	if (Comb(dim,t)%Comb(b,t)!=0) {
		printf("It sistema di Steiner (%d,%d,%d) non puo esistere!\n",t,b,dim);
		exit(1);
	}

	nb=Comb(dim,t)/Comb(b,t);

	printf("Inizio ricerca sistema di Steiner (%d,%d,%d)\n",t,b,dim);
	printf("%d blocchi da %d elementi\n",nb,b);
	printf("\n");

	Scan(b,dim,&comb_nb,0,set);
	Scan(t,dim,&comb_nt,0,NULL);
	Scan(t,b,&comb_bt,0,set);

	return(0);
}
