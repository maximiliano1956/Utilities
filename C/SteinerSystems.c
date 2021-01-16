#include <stdio.h>
#include <stdlib.h>

#define MAXDIM	24
#define MAXC	10000000

typedef struct {
        char combnz[MAXC][MAXDIM];
	int numc;
} MATRIX;

// Devono stare nell'heap e non nello stack se no va' in overflow di memoria!!!
MATRIX comb_nt;
MATRIX comb_nb;
MATRIX comb_bt;
//

int Comb(int n,int k) {

	if (n == 0)
		return(1);

	if (k==0 || k==n)
		return(1);
	if (k==1 || k==(n-1))
		return(n);

	return(Comb(n-1,k-1)+Comb(n-1,k));
}

void Scan2(unsigned char nn,unsigned char nsym,unsigned char n,unsigned char ii,MATRIX *m,
		unsigned char set[],unsigned char ind[]) {

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

		Scan2(nn,nsym,n+1,i+1,m,set,ind);
	}
}

void Scan(unsigned char order,unsigned char nsym,MATRIX *m,
		unsigned char append,unsigned char set[],unsigned char verbose,unsigned char ind[]) {

	if (append==0)
		m->numc=0;
	Scan2(order,nsym,0,1,m,set,ind);

	if (verbose==1) {
		printf("\n");
		printf("(%d,%d)\n",order,nsym);

		for (int i=0;i<m->numc;i++) {
			for (int j=0;j<order;j++)
				printf("%d ",m->combnz[i][j]);
			printf("\n");
		}
		printf("Nr. elementi = %d\n",m->numc);
	}
}


int main(char argc,char *argv[]) {

	unsigned char n;
	unsigned char b;
	unsigned char t;

	unsigned int nb;
	unsigned int tnb;
	unsigned int nbtnb;
	unsigned char set[MAXDIM];
	unsigned char ind[MAXDIM];

	t=2;
	b=3;
	n=7;

// Steiner system (2,3,7)
//
	printf("\n");

	if (Comb(n,t)%Comb(b,t)!=0) {
		printf("It sistema di Steiner (%d,%d,%d) non puo esistere!\n",t,b,n);
		exit(1);
	}

	nb=Comb(n,t)/Comb(b,t);

	printf("Inizio ricerca sistema di Steiner (%d,%d,%d)\n",t,b,n);
	printf("%d blocchi da %d elementi\n",nb,b);
	printf("\n");

	Scan(b,n,&comb_nb,0,NULL,1,ind);
	tnb=comb_nb.numc;
	Scan(t,n,&comb_nt,0,NULL,1,ind);
	Scan(t,b,&comb_bt,0,NULL,1,ind);

	Scan(nb,tnb,&comb_bt,0,NULL,1,ind);
	nbtnb=comb_nb.numc;

	printf("Fine!\n");

	return(0);
}
