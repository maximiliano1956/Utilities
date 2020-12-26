#include <stdio.h>

#define MAXR	10
#define MAXC	10

#define RED	0
#define BLACK	1
#define EMPTY	-1

#define CH_RED		'R'
#define CH_BLACK	'B'
#define CH_EMPTY	'.'

typedef struct
	{
	int nmoves;
	int moves[MAXC][2];
	} MOVES;

int pos[MAXR][MAXC];

int side;

int ply;

unsigned int nloop;

int nranks=6;
int nfiles=7;
//int nranks=3;
//int nfiles=4;


void init(void)
{
	for (int rank=0;rank<nranks;rank++)
		for (int file=0;file<nfiles;file++)
			pos[rank][file]=EMPTY;

	side=RED;

	ply=0;

	nloop=0;
}

void PrintBoard()
{
	for (int rank=nranks-1;rank>=0;rank--)
	{
		for (int file=0;file<nfiles;file++)
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
}


void GenMoveAllowed(MOVES *mv)
{
	mv->nmoves=0;

	for (int file=0;file<nfiles;file++)
		for (int rank=0;rank<nranks;rank++)
			if (pos[rank][file]==EMPTY)
			{
				mv->moves[mv->nmoves][0]=rank;
				mv->moves[mv->nmoves][1]=file;
				mv->nmoves++;
				break;
			}
}


void TakeMove(int rank,int file)
{
	pos[rank][file]=side;
	ply++;
	side^=BLACK;
//	printf("side= %d ply %d move %c%c\n",side,ply,'a'+rank,'1'+file);
}


void TakeBack(int rank,int file)
{
	side^=BLACK;
	pos[rank][file]=EMPTY;
	ply--;
//	printf("side= %d ply %d\n",side,ply);
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

	for (rank=0;rank<nranks;rank++)
	{
		file=0;
		count=0;

		do {

			if (pos[rank][file] == opponent)
			{
				count++;
				if (count==4)
					return(1);
			}
			else
				count=0;

			file++;

		   } while (file<nfiles);
	}

	for (file=0;file<nfiles;file++)
	{
		rank=0;
		count=0;

		do {

			if (pos[rank][file] == opponent)
			{
				count++;
				if (count==4)
					return(1);
			}
			else
				count=0;

			rank++;

		   } while (rank<nranks);
	}

	for (rank=0;rank<nranks;rank++)
		for (file=0;file<nfiles;file++)
			if (pos[rank][file] == opponent)
			{
				rr=rank;
				ff=file;
				count=1;
				while (++rr<nranks && --ff>=0)
				{
					if (pos[rr][ff] == opponent)
					{	
						count++;
						if (count==4)
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
						count++;
						if (count==4)
							return(1);
					}
					else
						count=0;
				}
			}
	
	return (0);
}


int Search(int *br,int *bc)
{
	MOVES mv;
	int score=-2;
	int score_out;

	GenMoveAllowed(&mv);

	for (int nm=0;nm<mv.nmoves;nm++)
	{
		TakeMove(mv.moves[nm][0],mv.moves[nm][1]);

//		PrintBoard();
//		getchar();

		if (CheckFour()==0)
		{
			score_out=-Search(br,bc);
		}
		else
		{
//			PrintBoard();
//			printf("%d loses! ply %d\n\n",side,ply);
			score_out=1;
		}

		if (score_out!=2 && score_out>score)
		{
			score=score_out;
			*br=mv.moves[nm][0];
			*bc=mv.moves[nm][1];
		}

		TakeBack(mv.moves[nm][0],mv.moves[nm][1]);

//		PrintBoard();
//		getchar();

		if (score==1)
			break;
	}

	if (mv.nmoves==0)
	{
		nloop++;
		score=0;

		if (nloop%1000000==0)
		{
//			PrintBoard();
//			getchar();
			printf("%d side %d score %d\n",nloop,side,score);
		}
	}

//	printf("side %d score %d\n",side,score);
	return(score);
}


void main(char argc, char *argv[])
{
	int score;
	int br;
	int bc;

	init();

//	side=BLACK;
//	pos[0][0]=RED;
//	pos[0][1]=RED;
//	pos[0][2]=RED;
//	pos[1][0]=RED;
//	pos[2][0]=RED;

	score=Search(&br,&bc);
	printf("side %d score %d\n",side,score);
	if (score==1)
		printf("Best move %c%c\n",'a'+bc,'1'+br);
}
