#include <time.h>
#include <stdlib.h>

#define MAXDIM 16

unsigned int
	DoSim(unsigned int ndir, unsigned int ntent)
	{
		int posiz[MAXDIM];
		unsigned int dir;
		int incr;
		int posold;
		float r;
		unsigned int flz;
		
		flz = ndir;
		
		for (dir = 0; dir < ndir; dir++)
			posiz[dir] = 0;
		
		for (unsigned int nt = 0; nt < ntent; nt++)
			{
				r = (float)rand() / RAND_MAX;
				dir = r * ndir;
				
				r = (float)rand() / RAND_MAX;
				incr = r > 0.5 ? 1 : -1;
				
				posold = posiz[dir];		
				posiz[dir] += incr;
				
				if (posiz[dir] == 0)
					{
						flz++;					
						if (flz == ndir)
							return(1);
					}
				else
					if (posold == 0)
						flz--;
			}
			
		return(0);
	}

int main(int argc, char *argv[])
{
	unsigned int nd = 2;
	
	unsigned int nsim = 1000;
	unsigned int nt = 10000000;
	
	unsigned res = 0;
	unsigned resold = 0;
	
	if (nd > MAXDIM)
		{
			printf("nd troppo grande\n");
			return(1);
		}
		
	srand(time(NULL));
	
	for (unsigned int ns = 0; ns < nsim; ns++)
		{
			res += DoSim(nd, nt);
			printf("%d ", nsim - ns);
			
			if (res != resold)
				{
					resold = res;
					printf("%d\n", res);
				}
			else
				printf("-\n");
		}
		
	printf("\n%f\n", (float)res / nsim);
	
	return(0);
}