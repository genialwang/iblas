#include "uscr/USCR_END.h"
dsp_linknode* BLAS_uscr_end(d_matrix* A)
{
int prpty,b;
int istat;
d_matrix* dpmatrix;
dsp_linknode* dsp_l;
//s_matrix* spmatrix;
//c_matrix* cpmatrix;
//z_matrix* zpmatrix;
//b=-a;
//rest = modulo(b,no_of_types);
//switch case(rest){
//case ISP_MATRIX:
//// **********************************************************************
//istat=-1
//access_matrix(ipmatrix ,a,istat)
//if(istat!=0) return istat;
//prpty= ipmatrix->property
//select case(ipmatrix->format)
//case 'b':
//   iuscr_blockend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'v':
//   iuscr_varend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'n':
//   iuscr_normend(a,prpty,istat)
//   if(istat!=0) return istat;
//case default
//   istat = blas_error_param
//   return istat;
//}//end select
//////***************************************************************************
//// **********************************************************************
//case SSP_MATRIX:
//// **********************************************************************
//istat=-1
//access_matrix(spmatrix ,a,istat)
//if(istat!=0) return istat;
//prpty= spmatrix->property
//switch(spmatrix->format){
//case 'b':
//   suscr_blockend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'v':
//   suscr_varend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'n':
//   suscr_normend(a,prpty,istat)
//   if(istat!=0) return istat;
//case default
//   istat = blas_error_param
//   return istat;
//}//end select
//// **********************************************************************
//////***************************************************************************
//case DSP_MATRIX:
// **********************************************************************
istat=-1;
dpmatrix=daccess_matrix(A);
if(dpmatrix==NULL) return NULL;
prpty=dpmatrix->property;
switch(dpmatrix->format){
case 'b':
   dsp_l=duscr_blockend(A,prpty,&istat);
   if(istat!=0) return NULL;
   break;
case 'v':
   dsp_l=duscr_varend(A,prpty,&istat);
   if(istat!=0) return NULL;
   break;
case 'n':
   dsp_l=duscr_normend(A,prpty,&istat);
   if(istat!=0) return NULL;
   break;
default:
   istat = blas_error_param;
   return NULL;
}//end select
// **********************************************************************
////***************************************************************************
//case(CSP_MATRIX)
//// **********************************************************************
//istat=-1
//access_matrix(cpmatrix ,a,istat)
//if(istat!=0) return istat;
//prpty= cpmatrix->property
//switch(cpmatrix->format){
//case 'b':
//   cuscr_blockend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'v':
//   cuscr_varend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'n':
//   cuscr_normend(a,prpty,istat)
//   if(istat!=0) return istat;
//case default
//   istat = blas_error_param
//   return istat;
//}//end select
//// **********************************************************************
//////***************************************************************************
//case(ZSP_MATRIX)
//// **********************************************************************
//istat=-1
//access_matrix(zpmatrix ,a,istat)
//if(istat!=0) return istat;
//prpty= zpmatrix->property
//switch(zpmatrix->format){
//case 'b':
//   zuscr_blockend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'v':
//   zuscr_varend(a,prpty,istat)
//   if(istat!=0) return istat;
//case 'n':
//   zuscr_normend(a,prpty,istat)
//   if(istat!=0) return istat;
//case default
//   istat = blas_error_param
//   return istat;
//}//end select
//// **********************************************************************
//////***************************************************************************
//// **********************************************************************
//case default
//   istat = blas_error_param
//   return istat;
//}
return dsp_l;
}
