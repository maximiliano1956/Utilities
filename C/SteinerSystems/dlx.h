#ifndef DLX_H_INCLUDED
#define DLX_H_INCLUDED

struct dlx_Node;

typedef int (*dlx_Callback)(int rows_size,int *rows,void *data);

struct dlx_Node *dlx_alloc(char *data,int rows,int cols);
void dlx_free(struct dlx_Node *header);
int dlx_solve(struct dlx_Node *header,dlx_Callback callback,void *data);

struct dlx_Node *dlx_alloc_sparse(unsigned short *data,int rows,int cols,int nuni);

#endif // ndef DLX_H_INCLUDED
