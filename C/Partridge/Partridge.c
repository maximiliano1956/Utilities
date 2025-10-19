#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <Xm/MainW.h>
#include <xpm.h>

#define NUMPIX 87

#define N_DIRPIX	"./pixmaps/normal/"
#define S_DIRPIX	"./pixmaps/small/"

#define DIRSOL		"./solutions/"

#define N_LPIX	26
#define S_LPIX	12

#define BPIX 4

#define BUTWIDTH	240	
#define BUTHEIGHT	120

#define MW	150
#define MH	200

#define LARGE 0
#define SMALL 1



#define MAXTILE 100
#define MAXN 300
#define OPZ1 "-n="
#define OPZ2 "-ssq="
#define OPZ3 "-v"
#define OPZ4 "-h"
#define OPZ5 "-do1"
#define OPZ6 "-do2"
#define OPZ7 "-pname="
#define SEP ","

struct Tile {
	unsigned int size;
	unsigned char used;
};

unsigned int field[MAXN][MAXN];
unsigned int size;
unsigned int solutions;
unsigned int attempts;

struct TileList {
	unsigned int tc;
	struct Tile tiles[MAXTILE];
};

void show(unsigned int fv);
void InitGra(int argc,char *argv[]);

struct TileList tlist;
unsigned int n;
unsigned int verbose;
unsigned int do1;
unsigned int do2;
unsigned int npez;
unsigned int maxnpez;
unsigned int ar;
unsigned int armax;

XtAppContext app;
Widget toplevel;
Display *dsp;
Display *dpy;
Screen *scr;
GC gc;
Pixmap pixmap;
Pixmap pix[NUMPIX];

int cex;

int sz;

int lpix;

int lpiu2;
int lpiu6;
int lpiu8;

int nr_max;
int nc_max;

int h_dra;
int w_dra;

int width;
int height;

int bx;
int by;

char puzzle[256];

char *pixmap_names[NUMPIX]=
	{
	"poly1.xpm",				/*   0 */
	"poly1-1.xpm",				/*   1 */
	"poly1-2.xpm",				/*   2 */
	"poly1-3.xpm",				/*   3 */
	"poly2.xpm",				/*   4 */
	"poly2-2.xpm",				/*   5 */
	"poly2-3.xpm",				/*   6 */
	"poly2-6.xpm",				/*   7 */
	"poly3.xpm",				/*   8 */
	"poly3-2.xpm",				/*   9 */
	"poly4.xpm",				/*  10 */
	"poly4-2.xpm",				/*  11 */
	"poly4-3.xpm",				/*  12 */
	"poly4-6.xpm",				/*  13 */
	"poly5.xpm",				/*  14 */
	"poly5-3.xpm",				/*  15 */
	"poly6.xpm",				/*  16 */
	"poly6-2.xpm",				/*  17 */
	"poly7.xpm",				/*  18 */
	"poly7-3.xpm",				/*  19 */
	"poly8.xpm",				/*  20 */
	"poly10.xpm",				/*  21 */
	"poly10-2.xpm",				/*  22 */
	"poly10-3.xpm",				/*  23 */
	"poly10-7.xpm",				/*  24 */
	"fpoly1.xpm",				/*  25 */
	"fpoly1-1.xpm",				/*  26 */
	"fpoly1-2.xpm",				/*  27 */
	"fpoly1-3.xpm",				/*  28 */
	"fpoly2.xpm",				/*  29 */
	"fpoly2-2.xpm",				/*  30 */
	"fpoly2-3.xpm",				/*  31 */
	"fpoly2-6.xpm",				/*  32 */
	"fpoly3.xpm",				/*  33 */
	"fpoly3-2.xpm",				/*  34 */
	"fpoly4.xpm",				/*  35 */
	"fpoly4-2.xpm",				/*  36 */
	"fpoly4-3.xpm",				/*  37 */
	"fpoly4-6.xpm",				/*  38 */
	"fpoly5.xpm",				/*  39 */
	"fpoly5-3.xpm",				/*  40 */
	"fpoly6.xpm",				/*  41 */
	"fpoly6-2.xpm",				/*  42 */
	"fpoly7.xpm",				/*  43 */
	"fpoly7-3.xpm",				/*  44 */
	"fpoly8.xpm",				/*  45 */
	"fpoly10.xpm",				/*  46 */
	"fpoly10-2.xpm",			/*  47 */
	"fpoly10-3.xpm",			/*  48 */
	"fpoly10-7.xpm",			/*  49 */
	"epoly1.xpm",				/*  50 */
	"epoly1-1.xpm",				/*  51 */
	"epoly1-2.xpm",				/*  52 */
	"epoly1-3.xpm",				/*  53 */
	"epoly2.xpm",				/*  54 */
	"epoly2-2.xpm",				/*  55 */
	"epoly2-3.xpm",				/*  56 */
	"epoly2-6.xpm",				/*  57 */
	"epoly3.xpm",				/*  58 */
	"epoly3-2.xpm",				/*  59 */
	"epoly4.xpm",				/*  60 */
	"epoly4-2.xpm",				/*  61 */
	"epoly4-3.xpm",				/*  62 */
	"epoly4-6.xpm",				/*  63 */
	"epoly5.xpm",				/*  64 */
	"epoly5-3.xpm",				/*  65 */
	"epoly6.xpm",				/*  66 */
	"epoly6-2.xpm",				/*  67 */
	"epoly7.xpm",				/*  68 */
	"epoly7-3.xpm",				/*  69 */
	"epoly8.xpm",				/*  70 */
	"epoly10.xpm",				/*  71 */
	"epoly10-2.xpm",			/*  72 */
	"epoly10-3.xpm",			/*  73 */
	"epoly10-7.xpm",			/*  74 */
	"poly10-8.xpm",				/*  75 */
	"fpoly10-8.xpm",			/*  76 */
	"epoly10-8.xpm",			/*  77 */
	"poly1-4.xpm",				/*  78 */
	"fpoly1-4.xpm",				/*  79 */
	"epoly1-4.xpm",				/*  80 */
	"poly2-7.xpm",				/*  81 */
	"fpoly2-7.xpm",				/*  82 */
	"epoly2-7.xpm",				/*  83 */
	"poly4-7.xpm",				/*  84 */
	"fpoly4-7.xpm",				/*  85 */
	"epoly4-7.xpm",				/*  86 */
	};



/********************************/
/*				*/
/*	    GetLato		*/
/*				*/
/********************************/

/* Get lato field in pixel */

/* INPUT:	nel		numero elementi del lato */
/* OUTPUT:	lato		size lato in pixel */

int GetLato(int nel)
{
return(nel*(lpix+2)+2+2*BPIX);
}


/********************************/
/*				*/
/*	    ReadPixmap		*/
/*				*/
/********************************/

/* Lettura pixmap */

void ReadPixmap(void)
{
int npix;
char name_pix[64];

printf("\nNow reading pixmaps...\r\n");

for (npix=0;npix<NUMPIX;npix++)
	{
	if (sz==LARGE) strcpy(name_pix,N_DIRPIX);
	else	       strcpy(name_pix,S_DIRPIX);
	strcat(name_pix,pixmap_names[npix]);
	if ((XpmReadFileToPixmap(dsp,RootWindowOfScreen(scr),
		name_pix,&pix[npix],NULL,NULL))!=XpmSuccess)
			{
			printf("\nCan't load pixmap!\n");
			exit(1);
			}
	}
printf("\nDone.\r\n");
}

/********************************/
/*				*/
/*	    DrawField		*/
/*				*/
/********************************/

/* Visualizza il field */

/* INPUT:	d		drawable		*/
/*		Nr		nr. righe field		*/
/*		Nc;		nr. colonne field	*/

void DrawField(Drawable d,int Nr,int Nc)
{
int nr;
int nc;
int x;
int y;

bx=0;
by=0;

x=bx;
y=by;

for (nr=0;nr<Nr;nr++)
	for (nc=0;nc<Nc;nc++)
		{
		if (nr==0 && nc==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[53],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[52],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[51],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[50],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[80],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[28],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[27],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[26],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[25],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[79],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[3],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[2],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[1],d,gc,0,0,lpiu8,lpiu8,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[0],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[78],d,gc,0,0,lpiu8,lpiu8,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			}
		if (nr==0 && nc==Nc-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[59],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[58],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[34],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[33],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[9],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[8],d,gc,0,0,lpiu6,lpiu8,x,y);
					x=bx;
					y+=lpiu8;
					continue;
					}
				}
			}
		if (nr==Nr-1 && nc==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[67],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[66],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[42],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[41],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[17],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[16],d,gc,0,0,lpiu8,lpiu6,x,y);
					x+=lpiu8;
					continue;
					}
				}
			}
		if (nr==Nr-1 && nc==Nc-1)
			{
			if (field[nr][nc]==0)
				{
				XCopyArea(dpy,pix[70],d,gc,0,0,lpiu6,lpiu6,x,y);
				continue;
				}
			if (field[nr][nc]==1)
				{
				XCopyArea(dpy,pix[45],d,gc,0,0,lpiu6,lpiu6,x,y);
				continue;
				}
			if (field[nr][nc]>1)
				{
				XCopyArea(dpy,pix[20],d,gc,0,0,lpiu6,lpiu6,x,y);
				continue;
				}
			}
		if (nr==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[57],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[55],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[56],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[54],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[83],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[32],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[30],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[31],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[29],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[82],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[7],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[5],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[6],d,gc,0,0,lpiu2,lpiu8,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[4],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[81],d,gc,0,0,lpiu2,lpiu8,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			}
		if (nr==Nr-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[69],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[68],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[44],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[43],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1])
					{
					XCopyArea(dpy,pix[19],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1])
					{
					XCopyArea(dpy,pix[18],d,gc,0,0,lpiu2,lpiu6,x,y);
					x+=lpiu2;
					continue;
					}
				}
			}
		if (nc==0)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[63],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[62],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[61],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[60],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[86],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[38],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[37],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[36],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[35],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[85],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[13],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[12],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[11],d,gc,0,0,lpiu8,lpiu2,x,y);
					x+=lpiu8;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[10],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[84],d,gc,0,0,lpiu8,lpiu2,x,y);
						x+=lpiu8;
						continue;
						}
					}
				}
			}
		if (nc==Nc-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[65],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[64],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[40],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[39],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[15],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[14],d,gc,0,0,lpiu6,lpiu2,x,y);
					x=bx;
					y+=lpiu2;
					continue;
					}
				}
			}
		if (nr!=Nr-1 && nc!=Nc-1)
			{
			if (field[nr][nc]==0)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[74],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[72],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[73],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[71],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[77],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]==1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[49],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[47],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[48],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[46],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[76],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			if (field[nr][nc]>1)
				{
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[24],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]!=field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					XCopyArea(dpy,pix[22],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]!=field[nr+1][nc])
					{
					XCopyArea(dpy,pix[23],d,gc,0,0,lpiu2,lpiu2,x,y);
					x+=lpiu2;
					continue;
					}
				if (field[nr][nc]==field[nr][nc+1] && field[nr][nc]==field[nr+1][nc])
					{
					if (field[nr][nc]==field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[21],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					if (field[nr][nc]!=field[nr+1][nc+1])
						{
						XCopyArea(dpy,pix[75],d,gc,0,0,lpiu2,lpiu2,x,y);
						x+=lpiu2;
						continue;
						}
					}
				}
			}
		}
}




/********************************/
/*				*/
/*	    SavePixmap		*/
/*				*/
/********************************/

/* Save soluzione in formato pixmap */

/* INPUT:	pixname		nome pixmap da salvare */

void SavePixmap(char *pixname,int nr,int nc)
{
XImage *xim;

DrawField(pixmap,nr,nc);
xim=XGetImage(dsp,pixmap,bx,by,GetLato(nr),GetLato(nc),0xffffffff,XYPixmap);
XpmWriteFileFromImage(dsp,pixname,xim,NULL,NULL);
XDestroyImage(xim);
}

void InitGra(int argc,char *argv[]) {

cex=(XOpenDisplay(NULL)==NULL) ? 0:1;

if (!cex)
	return;

sz=SMALL;

lpix=(sz==LARGE) ? N_LPIX:S_LPIX;

lpiu2=lpix+2;
lpiu6=lpiu2+BPIX;
lpiu8=lpiu6+2;

nr_max=200;
nc_max=200;

toplevel=XtVaAppInitialize (&app,"",NULL,0,&argc,argv,NULL,NULL);
dsp=XtDisplay(toplevel);
scr=XtScreen(toplevel);

dpy=dsp;

width=WidthOfScreen(scr);
height=HeightOfScreen(scr);

w_dra=nc_max*lpix+2+2*BPIX+2*MW;
h_dra=nr_max*lpix+2+2*BPIX+2*MH;
if (w_dra<(width-BUTWIDTH-20))   w_dra=width-BUTWIDTH-20;
if (h_dra<(height-BUTHEIGHT-20)) h_dra=height-BUTHEIGHT-20;

gc=XCreateGC(dsp,RootWindowOfScreen(scr),0,NULL);

pixmap=XCreatePixmap(dsp,RootWindowOfScreen(scr),w_dra,h_dra,DefaultDepthOfScreen(scr));

ReadPixmap();
}

void WriteSol(void) {

	char namesol[256];
	char num[256];

	if (cex) {
		strcpy(namesol,DIRSOL);
		strcat(namesol,puzzle);
		strcat(namesol,"-");
		sprintf(num,"%d",solutions);
		strcat(namesol,num);
		strcat(namesol,".xpm");
		SavePixmap(namesol,size,size);
	}
}

void AddTile(int sr,int sc,int sz) {
	int flag=0;

	for (int nr=sr;nr<sr+sz;nr++)
		for (int nc=sc;nc<sc+sz;nc++)
			field[nr][nc]=sz;
	npez++;
	if (npez>maxnpez) {
		maxnpez=npez;
		show(1);
		flag=1;
		printf("maxnpez=%d",maxnpez);
		printf("    attempts=%d\n",attempts);
	}
	attempts++;
	ar+=(unsigned int)pow(sz,2);
	if (ar>armax) {
		armax=ar;
		if (flag==0)
			show(1);
		printf("empty squares=%d\n",(unsigned int)pow(size,2)-armax);
	}
}

void DelTile(int sr,int sc,int sz) {
	for (int nr=sr;nr<sr+sz;nr++)
		for (int nc=sc;nc<sc+sz;nc++)
			field[nr][nc]=0;
	npez--;
	ar-=(unsigned int)pow(sz,2);
}

void show(unsigned int fv) {
	if (!fv && !verbose)
		return;

	printf("\n");
	for (int row=0;row<size;row++)
		for (int col=0;col<size;col++) {
			int bd=field[row][col];

			if (col<size-1)
				printf("%d,",bd);
			else
				printf("%d\n",bd);
		}
}

unsigned int canfit(int r,int c,int sz) {
	if (r+sz>size || c+sz>size)
		return(0);

	for (int row=r;row<r+sz;row++)
		for (int col=c;col<c+sz;col++)
			if (field[row][col]!=0)
				return(0);
	return(1);
}

int fe(unsigned int *r,unsigned int *c) {
	for (*r=0;*r<size;(*r)++)
		for (*c=0;*c<size;(*c)++)
			if (field[*r][*c]==0)
				return(1);
	return(0);
}

unsigned int ck1(unsigned int r,unsigned int c,unsigned int size) {
	if (do2 || size!=1)
		return(1);
	if (r>2 && r<size-2 && c>2 && c<size-2)
		return(1);

	return(0);
}

void go_deep(unsigned int sr,unsigned int sc) {
	int ls=0;
	for (int row=sr;row<sr+size;row++)
		for (int col=sc;col<sc+size;col++) {
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
							printf("Soluzione # %d\n",++solutions);
							WriteSol();
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
	printf("-pname=abcd where abcd is the puzzlename\n");
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
	solutions=0;
	npez=0;
	maxnpez=0;
	ar=0;
	armax=0;

	strcpy(puzzle,"prova_sol");

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
		else if (!strncmp(argv[ind],OPZ7,strlen(OPZ7)))
			strcpy(puzzle,argv[ind]+strlen(OPZ7));
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

	size=(unsigned int)sqrt(area);

	if (size>MAXN)
		return(2);

	if (area!=pow(size,2))
		return(3);

	for (int row=0;row<size;row++)
		for (int col=0;col<size;col++)
			field[row][col]=0;

	return(0);
}

int main(int argc,char *argv[]) {
	unsigned int result;

	n=3;

	InitGra(argc,argv);

	result=Init(argc,argv);

	printf("\nresult=%d\n\n",result);

	if (!result) {

		go_deep(0,0);

		printf("\ntotal attempts=%d\n\n",attempts);

		return(0);
	}
	return(1);
}
