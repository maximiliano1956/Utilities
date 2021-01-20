#include <stdio.h>
#include <stdlib.h>

#include "dlx.h"

#define MAXDIM	24
#define MAXC	1000000
#define MAXS	200000
#define MAXMAT	1200000000
#define MAXBN	800000
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
unsigned int set_tb[MAXBN][MAXTB];
char MatDlx[MAXMAT];
//
  
unsigned int nsol;
unsigned int nb;

unsigned char n;
unsigned char b;
unsigned char t;

unsigned char dlx_err_ram;

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
		unsigned char set[],unsigned char verbose) {

	if (n==nn) {
		if (m->numc>=MAXC) {
			printf("Not enough memory! (MAXC=%d)\n",MAXC);
			exit(1);
		}

		for (int i=0;i<nn;i++)
			m->combnz[m->numc][i]=(set!=NULL) ? set[ind[i]-1]:ind[i];

		m->numc++;

		return;
	}

	for (int i=ii;i<=nsym;i++) {
		ind[n]=i;

		Scan2(nn,nsym,n+1,i+1,m,set,verbose);
	}
}

void Scan(unsigned char order,unsigned char nsym,MATRIX *m,
		unsigned char append,unsigned char set[],unsigned char verbose) {

	if (append==0)
		m->numc=0;
	Scan2(order,nsym,0,1,m,set,verbose);

	if (verbose==1) {
		printf("\n");
		printf("(");

		for (int i=0;i<nsym;i++) {
			if (set!=NULL)
				printf("%2d",set[i]);
			else
				printf("%2d",i+1);
			if (i<nsym-1)
				printf(",");
		}
		printf(")\n");

		for (int i=0;i<m->numc;i++) {
			for (int j=0;j<order;j++)
				printf("%2d ",m->combnz[i][j]);
			printf("\n");
		}
		printf("Nr. elementi = %d\n",m->numc);
	}
}


int cback(int rows_size,int *rows,void *data) {

	int j;

	if (rows_size!=nb) {
		printf("Incoerent data!\n!");
		return(1);
	}

	if (nsol>=MAXS) {
		dlx_err_ram=1;
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
	unsigned int trovati;
	char verbose;

	unsigned int idx;
	struct dlx_Node *header;
/*
	t=2;
	b=3;
	n=7;		// Steiner system (t,b,n)
*/
  
	t=5;
	b=8;
	n=24;
  

	if (n>MAXDIM) {
		printf("n too high! (%d>=%d)\n",n,MAXDIM);
		exit(1);
	}

	verbose=0;

	nsol=0;
  
	printf("\n");

	if (b==3)
		if (n%6!=1 && n%6!=3) {
			printf("It sistema di Steiner (%d,%d,%d) non puo esistere!\n",t,b,n);
			exit(1);
		}

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

	if (tnb>=MAXBN) {
		printf("Not enough memory! (%d>=%d) (MAXBN)\n",tnb,MAXBN);
		exit(1);
	}

	if (bt>=MAXTB) {
		printf("Not enough memory! (%d>=%d) (MAXTB)\n",bt,MAXTB);
		exit(1);
	}

	if (tnb*nt>MAXMAT) {
		printf("Not enough memory! (%d>=%d) (MAXMAT)\n",tnb*nt,MAXMAT);
		exit(1);
	}

	printf("Numero combinazioni di %d elementi su %d = %d\n",t,n,nt);
	printf("Numero combinazioni di %d elementi su %d = %d\n",b,n,tnb);
	printf("Numero blocchi di %d elementi su %d necessari per la copertura esatta = %d\n",b,n,nb);
	printf("\n");
	printf("Premere un tasto ");
	getchar();
	printf("\n");

	Scan(t,n,&comb_tn,0,NULL,verbose);
	Scan(b,n,&comb_bn,0,NULL,verbose);

	printf("Generazione dei %d blocchi di %d elementi su %d possibili\n\n",tnb,b,n);

	for (int i=0;i<comb_bn.numc;i++) {
		if (verbose==0)
			if (i%500==0 || comb_bn.numc-i<10)
				printf("%d\n",i);
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
				printf("%2d",comb_bn.combnz[i][j]);
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
					printf("%2d",comb_tn.combnz[set_tb[i][j]][k]);
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
	printf("Nr. elementi = %d\n",tnb);

	idx=0;
	for (int i=0;i<tnb;i++)
		for (int j=0;j<nt;j++)
			MatDlx[idx++]=0;

	idx=0;
	for (int i=0;i<tnb;i++) {
		for (int j=0;j<bt;j++) {
			if (set_tb[i][j]>=nt) {
				printf("Incoerent data!\n!");
				exit(1);
			}
			for (int k=0;k<t;k++)
				MatDlx[idx+set_tb[i][j]]=1;
		}
		idx+=nt;
	}
#if 0
	if (verbose) {
		idx=0;
		for (int i=0;i<tnb;i++) {
			for (int j=0;j<nt;j++) {
				printf("%1d",MatDlx[idx++]);
			}
			printf("\n");
		}
	}
#endif
	header=dlx_alloc(MatDlx,tnb,nt);

	if (header==NULL) {
		printf("Not enough memory! (dlx_alloc)\n");
		exit(1);
	}

	dlx_err_ram=0;

	dlx_solve(header,cback,NULL);
	dlx_free(header);

	if (dlx_err_ram==1)
		printf("Nr. soluzioni limitato a %d (MAXS)\n\n",MAXS);

	printf("\n");
	printf("\n");
	printf("Trovate %d soluzioni\n\n",nsol);
	printf("Premere un tasto ");
	getchar();
	printf("\n");

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
