#include <stdio.h>

#define MAXDIM	16

int dim;
int ind[MAXDIM];
int tabmol[MAXDIM][MAXDIM];

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

	if (n==nn) {
		printf("1 ");
		for (int i=0;i<nn;i++)
			printf("%d ",ind[i]);
		printf("%s \n",checkSub(nn,ind) ? "e' sottogruppo":"non e' sottogruppo");
		return;
	}

	if (n==0)
		printf("\nOrder=%d\n",nn+1);
	for (int i=ii;i<=dim;i++) {
		ind[n]=i;
		Scan2(nn,n+1,i+1);
	}
	if (n==0)
		printf("\n");
}

void Scan(int order) {

	Scan2(order,0,2);

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

	for (int nn=1;nn<dim-1;nn++)
		Scan(nn);

	return(0);
}
