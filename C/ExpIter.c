#include <math.h>

unsigned int nd;
double a,b;
double hd;
double coo;	
double sum;	
unsigned int nit;
unsigned int niter;

double funz(unsigned int niter)
{
	if (niter==1)
		return(pow(coo,coo));
	return(pow(funz(niter-1),funz(niter-1)));
}

int main(int argc, char *argv[])
{	
	niter=4;
		
	a=0;
	b=1;			
	hd=0.000001;
	
	nd=(b-a)/hd;
	
	nit=nd+1;
	
	printf("Intervallo [%f,%f]\n",a,b);
	printf("Passo=%f\n",hd);
	printf("Numero punti=%d\n",nd);
	printf("\nPremere <invio> ...\n");
	getchar();
			
	sum=0;
	coo=a;
					
	while (coo<b)
		{
			sum+=funz(niter);															coo+=hd;
		}
		
	sum *=hd;
	
	printf("\nsum=%f\n",sum);
	printf("scarto=%f\n",
		fabs(pow(M_PI,2)/12-sum));

	return(0);
}