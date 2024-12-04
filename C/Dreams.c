#include <math.h>

unsigned int nd;
double a,b;
double hd;
double coo;	
double sum1,sum2;
unsigned int maxn;	

double funz()
{
	return(pow(coo,-coo));
}

int main(int argc, char *argv[])
{	
	maxn=100;
	
	a=0;
	b=1;			
	hd=0.000001;
	
	nd=(b-a)/hd;
	
	printf("Intervallo [%f,%f]\n",a,b);
	printf("Passo=%f\n",hd);
	printf("Numero punti=%d\n",nd);
	printf("\nPremere <invio> ...\n");
	getchar();
			
	sum1=0;
	coo=a;
					
	while (coo<b)
		{
			sum1+=funz();															coo+=hd;
		}
		
	sum1*=hd;
	
	printf("\nsum1=%f\n",sum1);
	
	sum2=0;
	
	for (double n=1;n<=maxn;n++)
		sum2+=pow(n,-n);
		
	printf("\nsum2=%f\n",sum2);
	
	printf("scarto=%f\n",
		fabs(sum2-sum1));

	return(0);
}