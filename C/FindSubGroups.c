#include <stdio.h>

#define MAXDIM	16

int dim;
int ind[MAXDIM];
int tabmol[MAXDIM][MAXDIM];
int numg;
int numgn;

int checkEl(int n,int el[],int l) {

	if (l==1)
		return(1);

	for (int ind=0;ind<n;ind++)
		if (l==el[ind])
			return(1);
	return(0);
}

int checkSub(int n,int el[]) {

	// Test chiusura

	for (int ind1=0;ind1<n;ind1++)
		for (int ind2=0;ind2<n;ind2++)
			if (checkEl(n,el,tabmol[el[ind1]-1][el[ind2]-1])==0)
				return(0);

	// Test inverso

	for (int ind1=0;ind1<n;ind1++)
		for (int ind2=0;ind2<dim;ind2++)
			if (tabmol[el[ind1]-1][ind2]==1)
				if (checkEl(n,el,ind2+1)==0)
					return(0);

	return(1);
}

void Scan2(int nn,int n,int ii) {

	int result;
	int inv;
	int conj;

	if (n==nn) {
		result=checkSub(nn,ind);

		if (result) {
			printf("{1");
			for (int i=0;i<nn;i++)
				printf(",%d",ind[i]);

			for (int i=2;i<=dim;i++) {
				if (checkEl(nn,ind,i)==0) {
					for (int j=1;j<=dim;j++)
						if (tabmol[i-1][j-1]==1) {
							inv=j;
							break;
						}

					for (int k=0;k<nn;k++) {
						conj=tabmol[inv-1][tabmol[ind[k]-1][inv-1]];
						if (checkEl(nn,ind,conj+1)==0) {
							printf("} is a subgroup\n");
							numg++;
							return;
						}
					}
				}
			}
			printf("} is a normal subgroup\n");
			numgn++;
		}
		return;
	}

	for (int i=ii;i<=dim;i++) {
		ind[n]=i;
		Scan2(nn,n+1,i+1);
	}
}

void Scan(int order) {

	numg=0;
	numgn=0;

	printf("Finding order %d subgroups of an order %d group\n",order+1,dim);

	Scan2(order,0,2);

	if (numg>0) {
		if (numg==1)
			printf("Only 1 subgroup\n");
		else
			printf("Found %d subgroups\n",numg);
	}
	else
		printf("No subgroups\n");

	if (numgn>0) {
		if (numgn==1)
			printf("Only 1 normal subgroup\n");
		else
			printf("Found %d normal subgroups\n",numgn);
	}
	else
		printf("No normal subgroups\n");

	printf("\n");
}

int main(char argc,char *argv[]) {

	int cayley[][6] = {
		{1,2,3,4,5,6},
		{2,3,1,5,6,4},
		{3,1,2,6,4,5},
		{4,6,5,1,3,2},
		{5,4,6,2,1,3},
		{6,5,4,3,2,1}
	};

	dim=6;

	for (int i=0;i<dim;i++)
		for (int j=0;j<dim;j++)
			tabmol[i][j]=cayley[i][j];

	printf("\n");

	for (int nn=1;nn<dim-1;nn++)
		Scan(nn);

	return(0);
}
