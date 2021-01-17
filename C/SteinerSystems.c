#include <stdio.h>
#include <stdlib.h>

#define MAXDIM	24
#define MAXC	10000000

typedef struct {
        char combnz[MAXC][MAXDIM];
	int numc;
} MATRIX;

// Devono stare nell'heap e non nello stack se no va' in overflow di memoria!!!
MATRIX comb_nbtnb;
MATRIX comb_tnb;
MATRIX comb_bn;
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

	char verbose;

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

	printf("Steiner System (%d,%d,%d)\n",t,b,n);
	printf("\n");

	nb=Comb(n,t)/Comb(b,t);
	tnb=Comb(n,b);
	nbtnb=Comb(tnb,nb);

	printf("Comb(%d,%d)/Comb(%d,%d) = %d\n",n,t,b,t,nb);
	printf("Comb(%d,%d)=%d\n",n,b,tnb);
	printf("Comb(%d,%d)=%d\n",tnb,nb,nbtnb);
	printf("\n");

	Scan(b,n,&comb_bn,0,NULL,0,ind);
	Scan(nb,comb_bn.numc,&comb_nbtnb,0,NULL,0,ind);

	printf("%d possibili configurazioni\n",comb_nbtnb.numc);
	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	verbose=0;

	for (int i=0;i<comb_nbtnb.numc;i++) {
		printf("%d\n",comb_nbtnb.numc-i);
		for (int j=0;j<nb;j++) {
			if (verbose==1)
				printf("(");
			for (int k=0;k<b;k++) {
				set[k]=comb_bn.combnz[comb_nbtnb.combnz[i][j]-1][k];
				if (verbose==1) {
					if (k>0)
						printf(",");
					printf("%d",set[k]);
				}
			}
			Scan(t,b,&comb_tnb,0,set,0,ind);
			if (verbose==1) {
				printf(")  ");
				for (int l=0;l<comb_tnb.numc;l++) {
					printf("(");
					for (int m=0;m<t;m++) {
						if (m>0)
							printf(",");
						printf("%d",comb_tnb.combnz[l][m]);
					}
					printf(") ");
				}
				printf("\n");
			}
		}
		if (verbose==1)
			printf("\n");
	}

	return(0);
}
