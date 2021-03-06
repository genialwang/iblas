#include "uscr/USCR_BEGIN.h"
d_matrix* BLAS_duscr_begin(int m,int n)
{
  int mb;
  d_matrix* dpmatrix;
  mb=1;

  if((m<=0)||(n<=0)) {
      return NULL;
    }else{//else
      dpmatrix=new_d_matrix(mb);
      dpmatrix=daccess_matrix(dpmatrix);
      if(dpmatrix==NULL) return NULL;
      dpmatrix->DIM[1]=m; //nb_of_rows
      dpmatrix->DIM[2]=n; //nb_of_cols
      dpmatrix->format='n';
    }//end if
  return dpmatrix;
}
