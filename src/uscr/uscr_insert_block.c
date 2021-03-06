#include "uscr/USCR_INSERT_BLOCK.h"
#include "uscr/USCR_INSERT_ENTRY.h"
int BLAS_duscr_insert_block (d_matrix *A, double** val, int row_stride, int clo_stride, int bi, int bj)
{
  d_matrix* pmatrix;
  int istat=-1;
  pmatrix=daccess_matrix(A);
  if(pmatrix==NULL) return istat;
  switch(pmatrix->format)
    {
    case 'b':
      dINS_block (pmatrix,val,row_stride,clo_stride,bi,bj,&istat);
      break;
    case 'v':
      dINS_varblock (pmatrix,val,row_stride,clo_stride,bi,bj,&istat);
      break;
    default:
      istat = blas_error_param;
      break;
    }
 return istat;
}
