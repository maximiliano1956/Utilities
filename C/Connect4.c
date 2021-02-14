#include <stdio.h>
#include <string.h>

#define MAXR	10
#define MAXC	10

#define RED	0
#define BLACK	1
#define EMPTY	-1

#define CH_RED		'R'
#define CH_BLACK	'B'
#define CH_EMPTY	'.'

#define	LIMLOOP	10000000

#define ALFABETA "-alfabeta"

typedef struct
	{
	int nmoves;
	int moves[MAXC][2];
	} MOVES;

int pos[MAXR][MAXC];

int side;

int ply;

unsigned int nloop;

MOVES pv;
int indpv;
int pv_scores[MAXC];

int nranks;
int nfiles;

int alfabeta;

int pwin;

int countch;
int maxply;


void init(void)
{

	nranks=6;
	nfiles=7;
//	nranks=4;
//	nfiles=5;
//	nranks=5;
//	nfiles=6;

	for (int rank=0;rank<nranks;++rank)
		for (int file=0;file<nfiles;++file)
			pos[rank][file]=EMPTY;

	side=RED;

	ply=0;

	//
//	pos[0][0]=RED;
//	pos[0][1]=BLACK;
//	pos[1][0]=RED;
//	pos[0][4]=BLACK;
//	pos[1][4]=RED;
//	pos[0][2]=BLACK;
//	pos[1][2]=RED;
//	pos[0][3]=BLACK;
//	side=BLACK;
//	ply=1;
	//

	nloop=0;

	alfabeta=1;

	pwin=4;

	countch=0;

	maxply=nranks*nfiles+1;
}

void PrintBoard()
{
	for (int rank=nranks-1;rank>=0;--rank)
	{
		for (int file=0;file<nfiles;++file)
		{
			if (pos[rank][file]==EMPTY)
				printf("%c",CH_EMPTY);
			if (pos[rank][file]==RED)
				printf("%c",CH_RED);
			if (pos[rank][file]==BLACK)
				printf("%c",CH_BLACK);
		}
		printf("\n");
	}
	printf("\n");
}


void GenMoveAllowed(MOVES *mv)
{
	mv->nmoves=0;

	for (int file=0;file<nfiles;++file)
		for (int rank=0;rank<nranks;++rank)
			if (pos[rank][file]==EMPTY)
			{
				mv->moves[mv->nmoves][0]=rank;
				mv->moves[mv->nmoves][1]=file;
				++mv->nmoves;
				break;
			}
}


void TakeMove(int rank,int file)
{
	pos[rank][file]=side;
	++ply;
	side^=BLACK;
}


void TakeBack(int rank,int file)
{
	side^=BLACK;
	pos[rank][file]=EMPTY;
	--ply;
}


//
// Controlla se il player ha posizione persa
//
//
// OUTPUT:	0		non si sa
//		1		sconfitta


int CheckFour(void)
{
	int esito;
	int rank;
	int file;
	int rr;
	int ff;
	int count;
	int opponent;

	opponent = side ^ BLACK;

	for (rank=0;rank<nranks;++rank)
	{
		file=0;
		count=0;

		do {

			if (pos[rank][file] == opponent)
			{
				++count;
				if (count==pwin)
					return(1);
			}
			else
				count=0;

			++file;

		   } while (file<nfiles);
	}

	for (file=0;file<nfiles;++file)
	{
		rank=0;
		count=0;

		do {

			if (pos[rank][file] == opponent)
			{
				++count;
				if (count==pwin)
					return(1);
			}
			else
				count=0;

			++rank;

		   } while (rank<nranks);
	}

	for (rank=0;rank<nranks;++rank)
		for (file=0;file<nfiles;++file)
			if (pos[rank][file] == opponent)
			{
				rr=rank;
				ff=file;
				count=1;
				while (++rr<nranks && --ff>=0)
				{
					if (pos[rr][ff] == opponent)
					{	
						++count;
						if (count==pwin)
							return(1);
					}
					else
						count=0;
				}

				rr=rank;
				ff=file;
				count=1;
				while (++rr<nranks && ++ff<nfiles)
				{
					if (pos[rr][ff] == opponent)
					{	
						++count;
						if (count==pwin)
							return(1);
					}
					else
						count=0;
				}
			}
	
	return (0);
}


int Search2(int *br,int *bc,int alfa,int beta)
{
	MOVES mv;
	int value;
	int score_out;
	int rank;
	int file;

	value=-2;

	GenMoveAllowed(&mv);

	for (int nm=0;nm<mv.nmoves;++nm)
	{
		file=mv.moves[nm][0];
		rank=mv.moves[nm][1];

	        pos[file][rank]=side;
        	++ply;
        	side^=BLACK;

//		TakeMove(mv.moves[nm][0],mv.moves[nm][1]);
#ifdef DEB
		PrintBoard();
#endif
		if (CheckFour()==0)
			score_out=-Search2(br,bc,-beta,-alfa);
		else
		{
			score_out=1;
			*br=file;
			*bc=rank;
		}

		if (score_out>value)
			value=score_out;

		if (value>alfa)
			alfa=value;

		side^=BLACK;
		pos[file][rank]=EMPTY;
		--ply;

//		TakeBack(mv.moves[nm][0],mv.moves[nm][1]);
#ifdef DEB
		PrintBoard();
#endif
		if (alfabeta==1)
			if (alfa>=beta)
			{
//				printf("cut-off!\n");
				break;
			}
	}

	if (mv.nmoves==0)
		value=0;

	++nloop;
	nloop%=LIMLOOP;

	if (nloop==0)
	{
		printf(".");
		fflush(stdout);

		++countch;
		countch%=100;
		if (countch==0)
			printf("\n");

//		PrintBoard();
//		printf("ply %d side %d score %d\n",ply,side,value);
	}

#ifdef DEB
	printf("Score=%d\n",value);
	getchar();
#endif

	if (ply<maxply)
	{
		maxply=ply;
		if (countch>0)
			printf("\n");
		printf("ply=%d\n",ply);
		countch=0;
	}

	return(value);
}

int Search(int *br,int *bc)
{
	int value;
	int alfa;
	int beta;

	GenMoveAllowed(&pv);

	value=-2;

	for (int nm=0;nm<pv.nmoves;++nm)
	{
		TakeMove(pv.moves[nm][0],pv.moves[nm][1]);
#ifdef DEB
		PrintBoard();
#endif
		pv_scores[nm]=-Search2(br,bc,-2,2);

		if (pv_scores[nm]>value) {
			value=pv_scores[nm];
			indpv=nm;
		}

		TakeBack(pv.moves[nm][0],pv.moves[nm][1]);
#ifdef DEB
		PrintBoard();
#endif

		maxply=nfiles*nranks+1;
		if (countch>0)
			printf("\n");
		printf("ply=%d\n",ply);
		printf("Mossa nr. %d = [%d,%d] score %d\n",nm,pv.moves[nm][0],pv.moves[nm][1],pv_scores[nm]);
	}

	return(value);
}


void main(char argc, char *argv[])
{
	int score;
	int br;
	int bc;
	int result;

	init();

	if (argc==2 && !strncmp(argv[1],ALFABETA,strlen(ALFABETA)))
		alfabeta=1;

	printf("\nalfabeta=%d\n\n",alfabeta);

	result=Search(&br,&bc);

	printf("Risultato=%d\n",result);
	printf("pv[%d,%d]=%d ",pv.moves[indpv][0],pv.moves[indpv][1],pv_scores[indpv]);
	printf("\n");

	printf("Principal variation\n");
	for (int nm=0;nm<pv.nmoves;++nm)
	{
		printf("pv[%d,%d]=%d ",pv.moves[nm][0],pv.moves[nm][1],pv_scores[nm]);
	}
	printf("\n");

}
