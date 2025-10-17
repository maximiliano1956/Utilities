#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXTILE 100
#define MAXN 300
#define OPZ1 "-n="
#define OPZ2 "-ssq="
#define OPZ3 "-v"
#define OPZ4 "-h"
#define OPZ5 "-do1"
#define OPZ6 "-do2"
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

void show(unsigned int fv);

struct TileList tlist;
struct BoardSquare board;
unsigned int n;
unsigned int verbose;
unsigned int do1;
unsigned int do2;
unsigned int npez;
unsigned int maxnpez;
unsigned int ar;
unsigned int armax;


void AddTile(int sr,int sc,int sz) {
	int flag=0;

	for (int nr=sr;nr<sr+sz;nr++)
		for (int nc=sc;nc<sc+sz;nc++)
			board.matrix[nr][nc]=sz;
	npez++;
	if (npez>maxnpez) {
		maxnpez=npez;
		show(1);
		flag=1;
		printf("maxnpez=%d",maxnpez);
		printf("    attempts=%d\n",board.attempts);
	}
	board.attempts++;
	ar+=(unsigned int)pow(sz,2);
	if (ar>armax) {
		armax=ar;
		if (flag==0)
			show(1);
		printf("empty squares=%d\n",(unsigned int)pow(board.size,2)-armax);
	}
}

void DelTile(int sr,int sc,int sz) {
	for (int nr=sr;nr<sr+sz;nr++)
		for (int nc=sc;nc<sc+sz;nc++)
			board.matrix[nr][nc]=0;
	npez--;
	ar-=(unsigned int)pow(sz,2);
}

void show(unsigned int fv) {
	unsigned int sz=board.size;

	if (!fv && !verbose)
		return;

	printf("\n");
	for (int row=0;row<sz;row++)
		for (int col=0;col<sz;col++) {
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

int fe(unsigned int *r,unsigned int *c) {
	for (*r=0;*r<board.size;(*r)++)
		for (*c=0;*c<board.size;(*c)++)
			if (board.matrix[*r][*c]==0)
				return(1);
	return(0);
}

unsigned int ck1(unsigned int r,unsigned int c,unsigned int size) {
	if (do2 || size!=1)
		return(1);
	if (r>2 && r<board.size-2 && c>2 && c<board.size-2)
		return(1);

	return(0);
}

void go_deep(unsigned int sr,unsigned int sc) {
	int ls=0;
	for (int row=sr;row<sr+board.size;row++)
		for (int col=sc;col<sc+board.size;col++) {
			for (int tl=0;tl<tlist.tc;tl++) {
				int sz=tlist.tiles[tl].size;
				if (!tlist.tiles[tl].used && (do1 || sz!=ls) && ck1(row,col,sz)) {
					if (canfit(row,col,sz)) {
						AddTile(row,col,sz);
						ls=sz;
						tlist.tiles[tl].used=1;
						show(0);
						unsigned int r,c;
						if (fe(&r,&c))
							go_deep(r,c);
						else {
							show(1);
							printf("Soluzione # %d\n",++board.solutions);
						}
					DelTile(row,col,sz);
					tlist.tiles[tl].used=0;
					show(0);
					}
				}
			}
			return;
		}
}

void help(void) {
	printf("\nPartridge puzzle solver\n\n");
	printf("Command line options:\n");
	printf("-n=a where a is the partridge number\n");
	printf("-ssq=a,b,c,... where a b c are squares sides\n");
	printf("-do1 disattiva test pezzi uguali\n");
	printf("-do2 disattiva controllo su pezzo di lato 1\n");
	printf("-v verbose\n");
	printf("-h this help\n");
	printf("\n");
}

unsigned int Init(int argc,char *argv[]) {
	char *str;
	int area;

	verbose=0;
	do1=0;
	do2=0;
	board.solutions=0;
	npez=0;
	maxnpez=0;
	ar=0;
	armax=0;

	tlist.tc=0;

	for (int ind=1;ind<argc;ind++) {
		if (!strncmp(argv[ind],OPZ1,strlen(OPZ1))) {
			sscanf(argv[ind]+strlen(OPZ1),"%d",&n);
		}
		else if (!strncmp(argv[ind],OPZ2,strlen(OPZ2))) {
			str=strtok(argv[ind]+strlen(OPZ2),SEP);
			while (str) {
				sscanf(str,"%d",&tlist.tiles[tlist.tc].size);
				tlist.tiles[tlist.tc].used=0;
				tlist.tc++;
				str=strtok(NULL,SEP);
			}
		}
		else if (!strncmp(argv[ind],OPZ3,strlen(OPZ3)))
			verbose=1;
		else if (!strncmp(argv[ind],OPZ4,strlen(OPZ4))) {
			help();
			exit(1);
		}
		else if (!strncmp(argv[ind],OPZ5,strlen(OPZ5)))
			do1=1;
		else if (!strncmp(argv[ind],OPZ6,strlen(OPZ6)))
			do2=1;
		else {
			help();
			exit(1);
		}
	}

	if (tlist.tc==0)
		for (int ind=1;ind<=n;ind++)
			for (int i=0;i<ind;i++) {
				tlist.tiles[tlist.tc].size=ind;
				tlist.tiles[tlist.tc].used=0;
				tlist.tc++;

				if (tlist.tc>=MAXTILE)
					return(1);
			}

	printf("n=%d\n\n",n);
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

	for (int row=0;row<board.size;row++)
		for (int col=0;col<board.size;col++)
			board.matrix[row][col]=0;

	return(0);
}

int main(int argc,char *argv[]) {
	unsigned int result;

	n=3;

	result=Init(argc,argv);

	printf("\nresult=%d\n\n",result);

	if (!result) {

		go_deep(0,0);

		printf("\ntotal attempts=%d\n\n",board.attempts);

		return(0);
	}
	return(1);
}
