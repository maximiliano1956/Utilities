#include <stdio.h>

#define MAXTILE	100
#define OPZ1 "-n="
#define OPZ2 "-ssq="
#define SEP ","

struct Tile {
	unsigned int size;
	unsigned char used;
};

struct Tile tiles[MAXTILE];
unsigned int n;
unsigned int ntiles;

void InitTiles(int argc,char *argv[])
{
	char *str;

	ntiles=0;

	for (int ind=1;ind<argc;ind++)
	{
		if (!strncmp(argv[ind],OPZ1,strlen(OPZ1)))
		{
			sscanf(argv[ind]+strlen(OPZ1),"%d",&n);
		}
		if (!strncmp(argv[ind],OPZ2,strlen(OPZ2)))
		{
			str=strtok(argv[ind]+strlen(OPZ2),SEP);
			while (str)
			{
				sscanf(str,"%d",&tiles[ntiles].size);
				tiles[ntiles].used=0;
				ntiles++;
				str=strtok(NULL,SEP);
			}
		}
	}

	if (ntiles==0)
		for (int ind=1;ind<=n;ind++)
		{
			for (int i=0;i<ind;i++)
			{
				tiles[ntiles].size=ind;
				tiles[ntiles].used=0;
				ntiles++;
			}
		}

	printf("n=%d\n",n);
	printf("ntiles=%d\n",ntiles);

	for (int ntile=0;ntile<ntiles;ntile++)
		printf("%d %d\n",tiles[ntile].size,tiles[ntile].used);
}

int main(int argc,char *argv[])
{
	n=8;

	InitTiles(argc,argv);

	return(0);
}
