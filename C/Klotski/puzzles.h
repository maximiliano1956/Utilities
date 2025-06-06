// this file contains several sample puzzles
// to solve via solveKlotski by Matthias Stirner

/* #define LAYTON159 */
#define MOVBLOCK

#if defined( DADS )
#define W 4
#define H 5
char puzzle[H][W] =
{
	{ 'A', 'A', 'B', 'B' },
	{ 'A', 'A', 'C', 'C' },
	{ 'D', 'E', '.', '.' },
	{ 'F', 'G', 'H', 'H' },
	{ 'F', 'G', 'I', 'I' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ 'A', 'A', '.', '.' },
	{ 'A', 'A', '.', '.' }
};
#elif defined(LAYTONW)
#define W 4
#define H 6
char puzzle[H][W] =
{
	{ 'A', 'B', 'B', 'C' },
	{ 'A', 'B', 'B', 'C' },
	{ 'D', 'E', 'E', 'F' },
	{ 'D', 'G', 'G', 'F' },
	{ 'H', 'I', 'J', 'K' },
	{ 'H', '.', '.', 'L' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', 'B', 'B', '.' },
	{ '.', 'B', 'B', '.' }
};
#elif defined(SIMPLE)
#define W 4
#define H 6
char puzzle[H][W] =
{
	{ '.', 'B', 'B', '.' },
	{ '.', 'B', 'B', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', 'B', 'B', '.' },
	{ '.', 'B', 'B', '.' }
};
#elif defined(LAYTON159)
#define W 8
#define H 8
char puzzle[H][W] =
{
	{ '#', '#', '.', '.', 'C', 'C', '#', '#' },
	{ '#', '.', '.', '.', 'C', 'D', '.', '#' },
	{ 'B', 'B', '.', '#', 'D', 'D', '.', '.' },
	{ 'B', 'B', '#', '#', 'E', 'F', '.', '.' },
	{ 'G', 'H', 'H', 'E', 'E', 'F', 'F', 'I' },
	{ 'G', 'G', 'H', '.', 'E', 'F', 'I', 'I' },
	{ '#', '.', 'J', 'K', 'K', 'L', 'L', '#' },
	{ '#', '#', 'J', 'J', 'K', 'L', '#', '#' }
};
char solve[H][W] =
{
	{ '#', '#', '.', 'B', 'B', '.', '#', '#' },
	{ '#', '.', '.', 'B', 'B', '.', '.', '#' },
	{ '.', '.', '.', '#', '.', '.', '.', '.' },
	{ '.', '.', '#', '#', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.', '.', '.' },
	{ '#', '.', '.', '.', '.', '.', '.', '#' },
	{ '#', '#', '.', '.', '.', '.', '#', '#' }
};
#elif defined(LAYTON170_1)
#define W 5
#define H 6
char puzzle[H][W] =
{
	{ 'P', '.', 'A', 'A', 'B' },
	{ 'P', 'L', 'A', 'A', 'B' },
	{ 'C', 'C', 'D', 'D', 'E' },
	{ 'C', 'C', 'D', 'D', 'E' },
	{ 'F', 'F', 'G', 'H', 'H' },
	{ '.', '.', 'I', 'I', 'I' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', 'P' },
	{ 'L', '.', '.', '.', 'P' }
};
#elif defined(LAYTON170_2)
#define W 6
#define H 6
char puzzle[H][W] =
{
	{ 'L', 'A', 'A', 'B', 'C', 'P' },
	{ 'D', 'D', 'E', '.', 'F', 'P' },
	{ 'D', 'D', 'E', 'F', 'F', 'F' },
	{ '.', 'G', 'G', 'H', 'H', '.' },
	{ '#', 'I', 'I', 'J', 'J', '#' },
	{ '#', 'K', '#', '#', 'M', '#' },
};
char solve[H][W] =
{
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', 'P', '.', '.', '.', '.' },
	{ '.', 'P', '.', '.', 'L', '.' },
};
#elif defined FORGET
#define W 4
#define H 5
char puzzle[H][W] =
{
	{ 'A', 'S', 'S', 'C' },
	{ 'A', 'S', 'S', 'C' },
	{ 'B', 'E', 'E', 'D' },
	{ 'B', 'F', 'G', 'D' },
	{ 'H', ' ', ' ', 'I' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', '.', '.', '.' },
	{ '.', 'S', 'S', '.' },
	{ '.', 'S', 'S', '.' }
};
#elif defined JIGSAW
#define W 6
#define H 6
char puzzle[H][W] =
{
	{ '.', 'J', '.', 'J', '.', '.' },
	{ 'J', 'J', 'J', 'J', 'J', '.' },
	{ '.', 'J', '.', 'J', '.', '.' },
	{ 'J', 'J', 'J', 'J', 'J', '.' },
	{ '.', 'J', '.', 'J', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', 'J', '.', 'J', '.' },
	{ '.', 'J', 'J', 'J', 'J', 'J' },
	{ '.', '.', 'J', '.', 'J', '.' },
	{ '.', 'J', 'J', 'J', 'J', 'J' },
	{ '.', '.', 'J', '.', 'J', '.' }
};
#elif defined SUNSHINE
#define W 17
#define H 17
char puzzle[H][W] =
{
	{ 'A','A','B','C','C','D','D','D','D','D','.','E','E','F','G','G','.','H','H','H','H','H','I','I','J','K','K' },
	{ 'A','A','B','C','C','D','D','D','D','D','.','E','E','F','G','G','.','H','H','H','H','H','I','I','J','K','K' },
	{ 'L','L','.','M','M','D','D','D','D','D','.','N','N','.','O','O','.','H','H','H','H','H','P','P','.','Q','Q' },
	{ 'R','R','S','T','T','D','D','D','D','D','.','U','U','V','X','X','.','H','H','H','H','H','Y','Y','Z','a','a' },
	{ 'R','R','S','T','T','D','D','D','D','D','.','U','U','V','X','X','.','H','H','H','H','H','Y','Y','Z','a','a' },
	{ 'J', 'J', 'J', 'J', 'J', '.' },
	{ '.', 'J', '.', 'J', '.', '.' },
	{ 'J', 'J', 'J', 'J', 'J', '.' },
	{ '.', 'J', '.', 'J', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', 'J', '.', 'J', '.' },
	{ '.', 'J', 'J', 'J', 'J', 'J' },
	{ '.', '.', 'J', '.', 'J', '.' },
	{ '.', 'J', 'J', 'J', 'J', 'J' },
	{ '.', '.', 'J', '.', 'J', '.' }
};
#elif defined MOVBLOCK
#define W 6
#define H 6
char puzzle[H][W] =
{
	{ 'A', 'B', 'B', '.', '.', 'E' },
	{ 'A', 'C', 'C', 'D', 'D', 'E' },
	{ 'F', 'F', 'G', '.', '.', 'E' },
	{ '.', 'H', 'G', 'I', 'I', '.' },
	{ '.', 'H', 'G', 'L', '.', '.' },
	{ '.', 'H', '.', 'L', 'M', 'M' }
};
char solve[H][W] =
{
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', 'F', 'F' },
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' },
	{ '.', '.', '.', '.', '.', '.' }
};
#endif
