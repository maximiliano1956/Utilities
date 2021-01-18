#include <stdio.h>
#include <stdlib.h>

#define MAXDIM	24
#define MAXC	10000000
#define MAXS	1000

typedef struct {
        char combnz[MAXC][MAXDIM];
	int numc;
} MATRIX;

// Devono stare nell'heap e non nello stack se no va' in overflow di memoria!!!
MATRIX comb_nbtnb;
MATRIX comb_tnb;
MATRIX comb_bn;
//
int soluz[MAXS][MAXDIM][MAXDIM];
int nsol;

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

int checkSteiner(int t) {

	int trovato;

	for (int i=0;i<comb_tnb.numc-1;i++) {
		for (int j=i+1;j<comb_tnb.numc;j++) {
			trovato=0;
			for (int k=0;k<t;k++) {
				for (int l=0;l<t;l++) {
					if (comb_tnb.combnz[i][k]==comb_tnb.combnz[j][l]) {
						trovato++;
						break;
					}
				}
			}
			if (trovato==t)
				return(0);
		}
	}
	return(1);
}


int main(char argc,char *argv[]) {

	unsigned char n;
	unsigned char b;
	unsigned char t;

	unsigned int nb;
	unsigned int tnb;
	unsigned int cp_tnb;
	unsigned int nbtnb;
	unsigned char set[MAXDIM];
	unsigned char ind[MAXDIM];

	char verbose;

	t=2;
	b=3;
	n=7;

	nsol=0;

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
		comb_tnb.numc=0;
		if (verbose==1)
			printf("%d\n",comb_nbtnb.numc-i);
		for (int j=0;j<nb;j++) {
			if (verbose==1)
				printf("(");
			for (int k=0;k<b;k++) {
				set[k]=comb_bn.combnz[comb_nbtnb.combnz[i][j]-1][k];
				soluz[nsol][j][k]=set[k];
				if (verbose==1) {
					if (k>0)
						printf(",");
					printf("%d",set[k]);
				}
			}
			cp_tnb=comb_tnb.numc;
			Scan(t,b,&comb_tnb,1,set,0,ind);
			if (verbose==1) {
				printf(")  ");
				for (int l=0;l<comb_tnb.numc-cp_tnb;l++) {
					printf("(");
					for (int m=0;m<t;m++) {
						if (m>0)
							printf(",");
						printf("%d",comb_tnb.combnz[l+cp_tnb][m]);
					}
					printf(") ");
				}
				printf("\n");
			}
		}
		if (verbose==1)
			printf("\n");

		if (checkSteiner(t)) {
			nsol++;
			printf("Trovata soluzione nr. %d\n",nsol);
		}
	}

	printf("\n");
	printf("\n");
	printf("Trovate %d soluzioni\n\n",nsol);
	for (int ns=0;ns<nsol;ns++) {
		printf("Soluzione nr. %6d      ",ns+1);
		printf("{");
		for (int i=0;i<nb;i++) {
			printf("{");
			for (int j=0;j<b;j++) {
				printf("%d",soluz[ns][i][j]);
				if (j<b-1)
					printf(",");
			}
			printf("}");
			if (i<nb-1)
				printf(",");
		}
		printf("}\n");
	}

	printf("\n");
	printf("\n");

	return(0);
}
