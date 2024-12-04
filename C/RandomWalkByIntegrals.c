#include <math.h>

#define MAXDIM 16

unsigned int nd;
double a,b;
double hd;
double coo[MAXDIM];	
double sum;
double prb,un;	
unsigned int lv,ch_lv;
unsigned int dim;
unsigned int nit;

double funz(void)
{
	double sumc;
	
	sumc=dim;
	
	for (unsigned int d=0;d<dim;d++)
		sumc-=cos(coo[d]);
		
	return(1/sumc);
}

int main(int argc, char *argv[])
{

	
	dim=3;
	
	a=0.01;
	b=M_PI;			
	hd=0.02;
	
	nd=(b-a)/hd;
	
	nit=nd+1;
	
	printf("Dimensione=%d\n",dim);
	printf("\n");
	printf("Intervallo [%f,%f]\n",a,b);
	printf("Passo=%f\n",hd);
	printf("Numero punti=%d\n",nd);
	printf("Numero punti calcolati=%f\n"
		,pow(nd,dim));
	printf("\nPremere <invio> ...\n");
	getchar();
			
	sum=0;
	
	if (dim>=MAXDIM)
		{
			printf("dim maggiore di %d\n",MAXDIM);
			return(1);
		}
					
	for (unsigned int d;d<dim;d++)
		coo[d]=a;
		
	ch_lv=1;
					
	while (coo[0]<b)
		{
			if (ch_lv)
			{
				lv=dim-1;
				ch_lv=0;
			}
			sum+=funz();															coo[lv]+=hd;
			while (coo[lv]>=b)
					if (lv>0)
					{
						coo[lv]=a;
						lv--;
						coo[lv]+=hd;
						if (lv==0)
							printf("%d\n",nit--);
						ch_lv=1;
					}
					else
						break;
			}

	sum*=pow(hd,dim);
	
	printf("\nsum=%f\n",sum);
	
	un=(dim/pow(M_PI,dim))*sum;
	
	printf("un=%f\n",un);
	
	prb=1-1/un;
	
	printf("prob=%f\n",prb);
	
	return(0);
}