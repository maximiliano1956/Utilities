#include <stdio.h>
#include <math.h>

#define MAXTILE 100
#define MAXN 300
#define OPZ1 "-n="
#define OPZ2 "-ssq="
#define SEP ","

struct Tile {
	unsigned int size;
	unsigned char used;
};

struct BoardSquare {
	unsigned int matrix[MAXN][MAXN];
	unsigned int size;
	unsigned int solutions;
	unsigned int attempts;
};

struct TileList {
	unsigned int tc;
	struct Tile tiles[MAXTILE];
};

struct TileList tlist;
struct BoardSquare board;
unsigned int n;


void AddTile(int sr,int sc,int sz)
{
    for (int nr=sr;nr<sr+sz;nr++)
        for (int nc=sc;nc<sc+sz;nc++)
            board.matrix[nr][nc]=sz;
    board.attempts++;
}

void DelTile(int sr,int sc,int sz)
{
    for (int nr=sr;nr<sr+sz;nr++)
        for (int nc=sc;nc<sc+sz;nc++)
            board.matrix[nr][nc]=0;
}

void show(void)
{
    unsigned int sz=board.size;

    printf("\n");
    for (int row=0;row<sz;row++)
    	for (int col=0;col<sz;col++)
	{
	    int bd=board.matrix[row][col];

	    if (col<sz-1)
	        printf("%d,",bd);
	    else
                printf("%d\n",bd);
	}
}

unsigned int canfit(int r,int c,int sz) {
    if (r+sz>board.size || c+sz>board.size)
        return(0);

    for (int row=r;row<r+sz;row++)
        for (int col=c;col<c+sz;col++)
                if (board.matrix[row][col]!=0)
                    return(0);
    return(1);
}

void go_deep(unsigned int sr,unsigned int sc)
{
    int ls=0;
    for (int row=sr;row<sr+board.size;row++)
	for (int col=sc;col<sc+board.size;col++) {
	    for (int tl=0;tl<tlist.tc;tl++) {
		int sz=tlist.tiles[tl].size;
		if (!tlist.tiles[tl].used &&
		    sz!=ls) {
		    if (canfit(row,col,sz)) {
		        AddTile(row,col,sz);
			ls=sz;
			tlist.tiles[tl].used=1;
			show();
			int r=row;
			int c=col+sz;
			if (c==board.size) {
			    c=0;
			    r++;
			}
			if (r<board.size)
		           go_deep(r,c);
			else
			   printf("Soluzione\n");
		        DelTile(row,col,sz);
			tlist.tiles[tl].used=0;
		        show();
	    	    }
		}
	    }
	    return;
	}
}

unsigned int Init(int argc,char *argv[])
{
	char *str;
	int area;

	tlist.tc=0;

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
				sscanf(str,"%d",&tlist.tiles[tlist.tc].size);
				tlist.tiles[tlist.tc].used=0;
				tlist.tc++;
				str=strtok(NULL,SEP);
			}
		}
	}

	if (tlist.tc==0)
		for (int ind=1;ind<=n;ind++)
		{
			for (int i=0;i<ind;i++)
			{
				tlist.tiles[tlist.tc].size=ind;
				tlist.tiles[tlist.tc].used=0;
				tlist.tc++;

				if (tlist.tc>=MAXTILE)
					return(1);
			}
		}

	printf("n=%d\n",n);
	printf("ntiles=%d\n",tlist.tc);

	for (int ntile=0;ntile<tlist.tc;ntile++)
		printf("%d %d\n",tlist.tiles[ntile].size,tlist.tiles[ntile].used);

	area=0;
	for (int ntile=0;ntile<tlist.tc;ntile++)
		area+=pow(tlist.tiles[ntile].size,2);

	printf("%d %d\n",tlist.tc,area);

	board.size=(unsigned int)sqrt(area);

	if (board.size>MAXN)
		return(2);

	if (area!=pow(board.size,2))
		return(3);

	for (int ntile=0;ntile<tlist.tc;ntile++)
		tlist.tiles[ntile].used=0;

	for (int row=0;row<board.size;row++)
		for (int col=0;col<board.size;col++)
			board.matrix[row][col]=0;

	return(0);
}

int main(int argc,char *argv[])
{
	unsigned int result;

	n=4;

	result=Init(argc,argv);

	printf("result=%d\n",result);

	go_deep(0,0);

	printf("Attempts=%d\n",board.attempts);

	return(0);
}
