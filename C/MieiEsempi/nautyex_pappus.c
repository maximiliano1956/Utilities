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

	n=18;

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

	ADDONEEDGE(g,0,5,m);
	ADDONEEDGE(g,1,8,m);
	ADDONEEDGE(g,2,13,m);
	ADDONEEDGE(g,3,10,m);
	ADDONEEDGE(g,4,15,m);
	ADDONEEDGE(g,5,0,m);
	ADDONEEDGE(g,6,11,m);
	ADDONEEDGE(g,7,14,m);
	ADDONEEDGE(g,8,1,m);
	ADDONEEDGE(g,9,16,m);
	ADDONEEDGE(g,10,3,m);
	ADDONEEDGE(g,11,6,m);
	ADDONEEDGE(g,12,17,m);
	ADDONEEDGE(g,13,2,m);
	ADDONEEDGE(g,14,7,m);
	ADDONEEDGE(g,15,4,m);
	ADDONEEDGE(g,16,9,m);
	ADDONEEDGE(g,17,12,m);

	lab[0]=0;
	lab[1]=2;
	lab[2]=4;
	lab[3]=6;
	lab[4]=8;
	lab[5]=10;
	lab[6]=12;
	lab[7]=14;
	lab[8]=16;
	lab[9]=1;
	lab[10]=3;
	lab[11]=5;
	lab[12]=7;
	lab[13]=9;
	lab[14]=11;
	lab[15]=13;
	lab[16]=15;
	lab[17]=17;

	ptn[0]=1;
	ptn[1]=1;
	ptn[2]=1;
	ptn[3]=1;
	ptn[4]=1;
	ptn[5]=1;
	ptn[6]=1;
	ptn[7]=1;
	ptn[8]=0;
	ptn[9]=1;
	ptn[10]=1;
	ptn[11]=1;
	ptn[12]=1;
	ptn[13]=1;
	ptn[14]=1;
	ptn[15]=1;
	ptn[16]=1;
	ptn[17]=0;

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
