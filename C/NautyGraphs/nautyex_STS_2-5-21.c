#include "nauty.h"    /* which includes <stdio.h> */
#include "naugroup.h"

/**************************************************************************/
/* Called by allgroup.  Just writes the permutation p. */
/**************************************************************************/

void writeautom(int *p, int n) {
	for (int i = 0; i < n; ++i) printf(" %2d",p[i]); printf("\n");
}

int main(int argc, char *argv[]) {
	DYNALLSTAT(graph,g,g_sz);
	DYNALLSTAT(int,lab,lab_sz);
	DYNALLSTAT(int,ptn,ptn_sz);
	DYNALLSTAT(int,orbits,orbits_sz);
	static DEFAULTOPTIONS_GRAPH(options);
	statsblk stats;

	int n,m,v;
	grouprec *group;

 	/* The following cause nauty to call two procedures which
           store the group information as nauty runs. */
        
	options.userautomproc = groupautomproc;
	options.userlevelproc = grouplevelproc;

	n=42;

	m = SETWORDSNEEDED(n);
	nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

	DYNALLOC2(graph,g,g_sz,m,n,"malloc");
	DYNALLOC1(int,lab,lab_sz,n,"malloc");
	DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
	DYNALLOC1(int,orbits,orbits_sz,n,"malloc");

	EMPTYGRAPH(g,m,n);

	options.writeautoms = TRUE;
	options.linelength=120;

	for (v = 0; v < n; ++v) {
		ADDONEEDGE(g,v,(v+1)%n,m);
	}

/*
{{6, 12, 32}, {9, 19, 39}, {3}, {17, 25, 33}, {5}, {1, 15, 
  35}, {7}, {13, 21, 25}, {9}, {15, 33, 41}, {11}, {1, 17, 
  23}, {13}, {3, 27, 37}, {15}, {21, 29, 39}, {17}, {7, 27, 
  41}, {19}, {3, 35, 41}, {21}, {5, 11, 37}, {23}, {15, 19, 
  31}, {25}, {11, 35, 39}, {27}, {5, 9, 23}, {29}, {3, 7, 
  11}, {31}, {1, 21, 27}, {33}, {13, 19, 29}, {35}, {9, 17, 
  31}, {37}, {7, 23, 33}, {39}, {5, 13, 31}, {41}, {25, 29, 37}}
*/

	ADDONEEDGE(g,1,8,m);
	ADDONEEDGE(g,1,18,m);
	ADDONEEDGE(g,1,38,m);

	ADDONEEDGE(g,3,16,m);
	ADDONEEDGE(g,3,24,m);
	ADDONEEDGE(g,3,32,m);

	ADDONEEDGE(g,5,0,m);
	ADDONEEDGE(g,5,14,m);
	ADDONEEDGE(g,5,34,m);

	ADDONEEDGE(g,7,12,m);
	ADDONEEDGE(g,7,20,m);
	ADDONEEDGE(g,7,24,m);

	ADDONEEDGE(g,9,14,m);
	ADDONEEDGE(g,9,32,m);
	ADDONEEDGE(g,9,40,m);

	ADDONEEDGE(g,11,0,m);
	ADDONEEDGE(g,11,16,m);
	ADDONEEDGE(g,11,22,m);

	ADDONEEDGE(g,13,2,m);
	ADDONEEDGE(g,13,26,m);
	ADDONEEDGE(g,13,36,m);

	ADDONEEDGE(g,15,20,m);
	ADDONEEDGE(g,15,28,m);
	ADDONEEDGE(g,15,38,m);

	ADDONEEDGE(g,17,6,m);
	ADDONEEDGE(g,17,26,m);
	ADDONEEDGE(g,17,40,m);

	ADDONEEDGE(g,19,2,m);
	ADDONEEDGE(g,19,34,m);
	ADDONEEDGE(g,19,40,m);

	ADDONEEDGE(g,21,4,m);
	ADDONEEDGE(g,21,10,m);
	ADDONEEDGE(g,21,36,m);

	ADDONEEDGE(g,23,14,m);
	ADDONEEDGE(g,23,18,m);
	ADDONEEDGE(g,23,30,m);

	ADDONEEDGE(g,25,10,m);
	ADDONEEDGE(g,25,34,m);
	ADDONEEDGE(g,25,38,m);

	ADDONEEDGE(g,27,4,m);
	ADDONEEDGE(g,27,8,m);
	ADDONEEDGE(g,27,22,m);

	ADDONEEDGE(g,29,2,m);
	ADDONEEDGE(g,29,6,m);
	ADDONEEDGE(g,29,10,m);

	ADDONEEDGE(g,31,0,m);
	ADDONEEDGE(g,31,20,m);
	ADDONEEDGE(g,31,26,m);

	ADDONEEDGE(g,33,12,m);
	ADDONEEDGE(g,33,18,m);
	ADDONEEDGE(g,33,28,m);

	ADDONEEDGE(g,35,8,m);
	ADDONEEDGE(g,35,16,m);
	ADDONEEDGE(g,35,30,m);

	ADDONEEDGE(g,37,6,m);
	ADDONEEDGE(g,37,22,m);
	ADDONEEDGE(g,37,32,m);

	ADDONEEDGE(g,39,4,m);
	ADDONEEDGE(g,39,12,m);
	ADDONEEDGE(g,39,30,m);

	ADDONEEDGE(g,41,24,m);
	ADDONEEDGE(g,41,28,m);
	ADDONEEDGE(g,41,36,m);

	lab[0]=0;
	lab[1]=2;
	lab[2]=4;
	lab[3]=6;
	lab[4]=8;
	lab[5]=10;
	lab[6]=12;
	lab[7]=14;
	lab[8]=16;
	lab[9]=18;
	lab[10]=20;
	lab[11]=22;
	lab[12]=24;
	lab[13]=26;
	lab[14]=28;
	lab[15]=30;
	lab[16]=32;
	lab[17]=34;
	lab[18]=36;
	lab[19]=38;
	lab[20]=40;

	lab[21]=1;
	lab[22]=3;
	lab[23]=5;
	lab[24]=7;
	lab[25]=9;
	lab[26]=11;
	lab[27]=13;
	lab[28]=15;
	lab[29]=17;
	lab[30]=19;
	lab[31]=21;
	lab[32]=23;
	lab[33]=25;
	lab[34]=27;
	lab[35]=29;
	lab[36]=31;
	lab[37]=33;
	lab[38]=35;
	lab[39]=37;
	lab[40]=39;
	lab[41]=41;

	ptn[0]=1;
	ptn[1]=1;
	ptn[2]=1;
	ptn[3]=1;
	ptn[4]=1;
	ptn[5]=1;
	ptn[6]=1;
	ptn[7]=1;
	ptn[8]=1;
	ptn[9]=1;
	ptn[10]=1;
	ptn[11]=1;
	ptn[12]=1;
	ptn[13]=1;
	ptn[14]=1;
	ptn[15]=1;
	ptn[16]=1;
	ptn[17]=1;
	ptn[18]=1;
	ptn[19]=1;
	ptn[20]=0;

	ptn[21]=1;
	ptn[22]=1;
	ptn[23]=1;
	ptn[24]=1;
	ptn[25]=1;
	ptn[26]=1;
	ptn[27]=1;
	ptn[28]=1;
	ptn[29]=1;
	ptn[30]=1;
	ptn[31]=1;
	ptn[32]=1;
	ptn[33]=1;
	ptn[34]=1;
	ptn[35]=1;
	ptn[36]=1;
	ptn[37]=1;
	ptn[38]=1;
	ptn[39]=1;
	ptn[40]=1;
	ptn[41]=0;

	options.defaultptn=FALSE;

	printf("\n");
	printf("Generators of graph automorphisms group:\n");
	printf("\n");
	densenauty(g,lab,ptn,orbits,&options,&stats,m,n,NULL);
	printf("\n");
	printf("\n");
	printf("Press a key to continue ");
	getchar();

	/* Get a pointer to the structure in which the group information
	   has been stored.  If you use TRUE as an argument, the
	   structure will be "cut loose" so that it won't be used
	   again the next time nauty() is called.  Otherwise, as
	   here, the same structure is used repeatedly. */
                
	group = groupptr(FALSE);

	/* Expand the group structure to include a full set of coset
	   representatives at every level.  This step is necessary
	   if allgroup() is to be called. */
                
	makecosetreps(group);

	/* Call the procedure writeautom() for every element of the group.
	   The first call is always for the identity. */
                
	printf("\n");
	printf("Cailey table = ");
	printf("\n");
	printf("\n");
	allgroup(group,writeautom);

	printf("\n");
	printf("Automorphism group size = ");
	writegroupsize(stdout,stats.grpsize1,stats.grpsize2);
	printf("\n");
	printf("\n");

	return(0);
}
