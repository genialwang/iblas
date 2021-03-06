#include "uscr/uscr_bdi.h"
dsp_linknode* duscr_bdi (int m,int n,double* val,int n_val,int blda,int* ibdiag,int n_ibdiag,int nbdiag,int mb,int kb,int lb,int prpty,int* istat)
{
  int options,base,nnz,bnnz,val_ind;
  bool COPY;
  char message;
  DSPMAT* dsp_data;
  dsp_linknode* dsp_l;
  int i,j;
  options=*istat;
  *istat=-1; //if not changed later,routine has failed
  if((nbdiag!=n_ibdiag)||(blda*nbdiag*lb*lb!=n_val)||(maxval(ibdiag,n_ibdiag)>kb)||(minval(ibdiag,n_ibdiag)<-mb)||(blda!=(mb<kb?mb:kb)))
    {
      *istat = blas_error_param;
      return NULL;
    }

  dsp_l=new_dsp(istat);
  if (*istat!=0) {
      *istat=blas_error_memalloc;
      return NULL;
    }
  dsp_data=accessdata_dsp (dsp_l,istat);
  if(*istat!=0) {
      *istat=blas_error_param;
      return NULL;
    }
  dsp_data->FIDA =BDI_FORMAT;
  dsp_data->M = m;
  dsp_data->K = n;
  set_descra(dsp_data->DESCRA,prpty,istat);
  get_descra(dsp_data->DESCRA,'b',&message,istat);
  if(*istat!=0) {
      *istat=blas_error_param;
      return NULL;
    }
  if (message=='C')
    base = C_BASE;
  else//Assuming F base
    base = F_BASE;

  set_infoa(dsp_data->INFOA,'b',base,istat);
  if(*istat!=0) {
      *istat=blas_error_param;
      return NULL;
    }
  nnz = count(val,n_val,0);
  bnnz = 0;
  for(i=0;i<blda*nbdiag;i++){
    for(j=0;j<lb*lb;j++){
      val_ind = i*lb*lb+j;
      if(val[val_ind] != 0){
        bnnz++;
        break;
      }
    }
  }
  if(nnz<=blda*nbdiag*lb*lb*0.5)
    printf("too many no zero element!\n ");
  //Warning Many zeros stored//
  set_infoa(dsp_data->INFOA,'n',bnnz,istat);
  set_infoa(dsp_data->INFOA,'d',lb,istat);//row-dim of a block
  set_infoa(dsp_data->INFOA,'e',lb,istat);//col-dim of a block
  set_infoa(dsp_data->INFOA,'f',blda,istat);//blocks per diagonal
  set_infoa(dsp_data->INFOA,'g',nbdiag,istat);//no of diagonals

  if(options>0) {//then
      // decision rule whether or not to copy
      COPY=TRUE;
      if(COPY) {//then
          options=-1; //copy
        }else{
          options=0;  //reference
        }//end if
    }//end if

  dsp_data->n_A=n_val;
  dsp_data->n_IA1=n_ibdiag;
  dsp_data->n_PB=0;
  dsp_data->n_PE=0;
  dsp_data->n_BP1=0;
  dsp_data->n_BP2=0;
  dsp_data->n_IA2=0;

  if (options==0) {
      set_infoa(dsp_data->INFOA,'c',REF_OF_SOURCE,istat);
      if (*istat!=0) {
          *istat = blas_error_param;
          return NULL;
        }
      //create reference to original matrix
      dsp_data->A = val;
      dsp_data->IA1 =ibdiag;
      *istat = 0;
    }else{
      //The additional required memory is DEALLOCATED later in USDS!
      set_infoa(dsp_data->INFOA,'c',COP_OF_SOURCE,istat);
      if (*istat!=0){
          *istat = blas_error_param;
          return NULL;
        }
      //copy original data

      dsp_data->A=(double*)aligned_malloc(sizeof(double)*n_val);
      dsp_data->IA1=(int*)aligned_malloc(sizeof(int)*n_ibdiag);

      for(i=0;i<dsp_data->n_A;i++)
        dsp_data->A[i]= val[i];
      for(i=0;i<dsp_data->n_IA1;i++)
        dsp_data->IA1[i]= ibdiag[i];
      *istat = 1;
    }

    // for(i=0;i<dsp_data->n_A;i++){
    //   printf("%lf\t",dsp_data->A[i]);
    // }
    // printf("\n");
    // for(i=0;i<dsp_data->n_IA1;i++){
    //   printf("%d\t",dsp_data->IA1[i];
    // }

  if(*istat>=0)
    {
      *istat=0;
      return dsp_l;
    }
  else
    return NULL;
}
