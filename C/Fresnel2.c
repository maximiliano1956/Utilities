#include <math.h>

unsigned int nd;
double a,b;
double hd;
double coo;	
double sum;	
unsigned int nit;

double funz(void)
{
	return(sin(pow(coo,2)));
}

int main(int argc, char *argv[])
{	
	a=0;
	b=10000;			
	hd=0.0001;
	
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
			sum+=funz();															coo+=hd;
		}
		
	sum *=hd;
	
	printf("\nsum=%f\n",sum);
	printf("scarto=%f\n",fabs(sum-sqrt(M_PI/8)));
	
	return(0);
}