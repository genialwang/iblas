#include "properties.h"
#include "conv/conv_tools.h"
#include "link.h"
#include "conv/usconv.h"
void usconv_csr2bco(void* spmat,int* ierr)
{
  int res,rest,i;
  int bnnz,mb,kb;
  ISPMAT*  isp_data;
  SSPMAT*  ssp_data;
  DSPMAT*  dsp_data;
  CSPMAT*  csp_data;
  ZSPMAT*  zsp_data;
  (*ierr)=-1;
  int lb = 2;
  rest=((dsp_linknode*)spmat)->number;
  switch (rest){
    case ISP_MATRIX:
      // **********************************************************************;

      break;
      // **********************************************************************;
    case SSP_MATRIX:
      // **********************************************************************;

      break;
      // **********************************************************************;
    case DSP_MATRIX:
      // **********************************************************************;
      dsp_data=accessdata_dsp(spmat,ierr);
      if (*ierr!=0) {
          *ierr = blas_error_param;
          return;
        }
      if(dsp_data->FIDA==CSR_FORMAT) {
          get_infoa(dsp_data->INFOA ,'c',&res,ierr);
          if(res==COP_OF_SOURCE) {
              dsp_data->FIDA=BCO_FORMAT;
              dpre_usconv_csr2bco(dsp_data->M,dsp_data->K,&(dsp_data->A),dsp_data->n_A,&(dsp_data->IA1),&(dsp_data->IA2),dsp_data->PB,dsp_data->PE,lb,lb,&bnnz,&mb,&kb);
              dsp_data->n_IA1 = bnnz;
              dsp_data->n_IA2 = bnnz;
              dsp_data->n_A = bnnz*lb*lb;
              if(dsp_data->PB != NULL){
                aligned_free(dsp_data->PB);
                dsp_data->PB = NULL;
                dsp_data->n_PB = 0;
              }
              if(dsp_data->PE != NULL){
                aligned_free(dsp_data->PE);
                dsp_data->PE = NULL;
                dsp_data->n_PE = 0;
              }
              set_infoa(dsp_data->INFOA,'n',bnnz,ierr);
              set_infoa(dsp_data->INFOA,'d',lb,ierr); //row-dim of a block
              set_infoa(dsp_data->INFOA,'e',lb,ierr); //col-dim of a block
              set_infoa(dsp_data->INFOA,'f',mb,ierr); //row-dim in blocks
              set_infoa(dsp_data->INFOA,'g',kb,ierr); //col-dim in blocks

              // printf("\n------------------------CSR2BCO---------------------\n");
              // printf("bnnz:%d  lb:%d  mb:%d  kb:%d\n",bnnz,lb,mb,kb);
              // printf("n_IA1:%d  n_IA2:%d  n_A:%d\n",dsp_data->n_IA1,dsp_data->n_IA2,dsp_data->n_A);
              // printf("\n");
              // for(i=0;i<dsp_data->n_IA1;i++){
              //   printf("%d\t%d",dsp_data->IA1[i],dsp_data->IA2[i]);
              // }
              // printf("\n");
              // printf("\n");
              // for(i=0;i<dsp_data->n_A;i++){
              //   printf("%lf\t",dsp_data->A[i]);
              // }
              // printf("\n------------------------CSR2BCO---------------------\n");

            }
        }else{
          *ierr = blas_error_param;
          return;
        }
      break;
      // **********************************************************************;
    case CSP_MATRIX:
      // **********************************************************************;

      break;
      // **********************************************************************;
    case ZSP_MATRIX:
      // **********************************************************************;

      break;
      // **********************************************************************;
    default:
      *ierr = blas_error_param;
      return;
    }
}
