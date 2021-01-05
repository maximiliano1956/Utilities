/* This program prints the entire automorphism group of the Tutte-Coxeter graph
*/

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

	n=30;

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

	ADDONEEDGE(g,0,13,m);
	ADDONEEDGE(g,1,18,m);
	ADDONEEDGE(g,2,23,m);
	ADDONEEDGE(g,3,10,m);
	ADDONEEDGE(g,4,27,m);
	ADDONEEDGE(g,5,14,m);
	ADDONEEDGE(g,6,19,m);
	ADDONEEDGE(g,7,24,m);
	ADDONEEDGE(g,8,29,m);
	ADDONEEDGE(g,9,16,m);
	ADDONEEDGE(g,10,3,m);
	ADDONEEDGE(g,11,20,m);
	ADDONEEDGE(g,12,25,m);
	ADDONEEDGE(g,13,0,m);
	ADDONEEDGE(g,14,5,m);
	ADDONEEDGE(g,15,22,m);
	ADDONEEDGE(g,16,9,m);
	ADDONEEDGE(g,17,26,m);
	ADDONEEDGE(g,18,1,m);
	ADDONEEDGE(g,19,6,m);
	ADDONEEDGE(g,20,11,m);
	ADDONEEDGE(g,21,28,m);
	ADDONEEDGE(g,22,15,m);
	ADDONEEDGE(g,23,2,m);
	ADDONEEDGE(g,24,7,m);
	ADDONEEDGE(g,25,12,m);
	ADDONEEDGE(g,26,17,m);
	ADDONEEDGE(g,27,4,m);
	ADDONEEDGE(g,28,21,m);
	ADDONEEDGE(g,29,8,m);

	printf("\n");
	printf("Automorphisms of Tutte-Coxeter graph:\n");
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
