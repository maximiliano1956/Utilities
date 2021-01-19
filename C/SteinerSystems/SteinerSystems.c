#include <stdio.h>
#include <stdlib.h>

#include "dlx.h"

#define MAXDIM	24
#define MAXC	10000000
#define MAXS	1000
#define MAXMAT	1000
#define MAXBN	100
#define MAXTB	100

typedef struct {
        char combnz[MAXC][MAXDIM];
	int numc;
} MATRIX;

// Devono stare nell'heap e non nello stack se no va' in overflow di memoria!!!
MATRIX comb_nbtnb;
MATRIX comb_tnb;
MATRIX comb_bn;
MATRIX comb_tn;
MATRIX comb_tb;
unsigned char soluz[MAXS][MAXDIM][MAXDIM];
unsigned char ind[MAXDIM];
unsigned char set_tb[MAXBN][MAXTB];
char MatDlx[MAXMAT];
//
  
int nsol;
unsigned int nb;

unsigned char n;
unsigned char b;
unsigned char t;

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
		unsigned char set[]) {

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

void Scan(unsigned char order,unsigned char nsym,MATRIX *m,
		unsigned char append,unsigned char set[],unsigned char verbose) {

	if (append==0)
		m->numc=0;
	Scan2(order,nsym,0,1,m,set);

	if (verbose==1) {
		printf("\n");
		printf("(");

		for (int i=0;i<nsym;i++) {
			if (set!=NULL)
				printf("%d",set[i]);
			else
				printf("%d",i+1);
			if (i<nsym-1)
				printf(",");
		}
		printf(")\n");

		for (int i=0;i<m->numc;i++) {
			for (int j=0;j<order;j++)
				printf("%d ",m->combnz[i][j]);
			printf("\n");
		}
		printf("Nr. elementi = %d\n",m->numc);
	}
}


int callback1(int rows_size,int *rows,void *data) {

	int j;

	if (rows_size!=nb) {
		printf("Incoerent data!\n!");
		return(1);
	}

for (int i=0;i<rows_size;i++)
	for (int j=0;j<b;j++)
		soluz[nsol][i][j]=comb_bn.combnz[rows[i]][j];

nsol++;

return(0);
}


int main(char argc,char *argv[]) {

	unsigned int tnb;
	unsigned int cp_tnb;
	unsigned int nt;
	unsigned int bt;
	unsigned char set[MAXDIM];
	int trovati;
	int idx;

	struct dlx_Node *header;
	char solutions[100];

	char verbose;
  
	t=2;
	b=3;
	n=7;

	verbose=0;

	nsol=0;

// Steiner system (2,3,7)
  
	printf("\n");

	if (Comb(n,t)%Comb(b,t)!=0) {
		printf("It sistema di Steiner (%d,%d,%d) non puo esistere!\n",t,b,n);
		exit(1);
	}

	printf("Steiner System (%d,%d,%d)\n",t,b,n);
	printf("\n");

	nt=Comb(n,t);
	bt=Comb(b,t);
	nb=nt/bt;
	tnb=Comb(n,b);

	printf("Numero combinazioni di %d elementi su %d = %d\n",t,n,nt);
	printf("Numero combinazioni di %d elementi su %d = %d\n",b,n,tnb);
	printf("Numero blocchi di %d elementi su %d necessari per la copertura esatta = %d\n",b,n,nb);
	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	Scan(t,n,&comb_tn,0,NULL,0);
	Scan(b,n,&comb_bn,0,NULL,0);

	for (int i=0;i<comb_bn.numc;i++) {
		for (int j=0;j<b;j++)
			set[j]=comb_bn.combnz[i][j];
		Scan(t,b,&comb_tb,0,set,0);
		for (int j=0;j<comb_tb.numc;j++) {
				for (int k=0;k<comb_tn.numc;k++) {
					trovati=0;
					for (int l=0;l<t;l++)
						if (comb_tb.combnz[j][l]==comb_tn.combnz[k][l])
							trovati++;
					if (trovati==t) {
						set_tb[i][j]=k;
						break;
					}
				}
				if (trovati!=t) {
					printf("Incoerent data!\n");
					exit(1);
				}
		}
	}

	if (verbose==1)
		for (int i=0;i<tnb;i++) {
			printf("{");
			for (int j=0;j<b;j++) {
				printf("%d",comb_bn.combnz[i][j]);
				if (j<b-1)
					printf(",");
			}
			printf("}     ");
			for (int j=0;j<bt;j++) {
				if (j==0)
					printf("{");
				for (int k=0;k<t;k++) {
					if (k==0)
						printf("{");
					printf("%d",comb_tn.combnz[set_tb[i][j]][k]);
					if (k<t-1)
						printf(",");
					else
						printf("}");
				}
				if (j<bt-1)
					printf(",");
				else
					printf("}\n");
			}
		}

	idx=0;
	for (int i=0;i<tnb;i++)
		for (int j=0;j<nt;j++)
			MatDlx[idx++]=0;

	idx=0;
	for (int i=0;i<tnb;i++) {
		for (int j=0;j<bt;j++)
			MatDlx[idx+set_tb[i][j]]=1;
		idx+=nt;
	}

	header=dlx_alloc(MatDlx,tnb,nt);
	dlx_solve(header,callback1,&solutions);
	dlx_free(header);

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
