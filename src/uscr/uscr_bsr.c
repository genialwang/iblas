#include "uscr/uscr_bsr.h"
dsp_linknode* duscr_bsr (int m,int n,double* val,int n_val,int* bindx,int n_bindx,int* bpntrb,int n_bpntrb ,int* bpntre,int n_bpntre,int mb,int kb,int lb,int prpty,int* istat)
{
  int options,base,nnz;
  bool COPY;
  char message;
  DSPMAT* dsp_data;
  dsp_linknode* dsp_l;
  int i;
  options=*istat;
  *istat=-1; //if not changed later,routine has failed
  dsp_l=new_dsp(istat);
  if(*istat!=0) {
      *istat=blas_error_memalloc;
      return NULL;
    }//end if
  dsp_data=accessdata_dsp(dsp_l,istat);
  if(*istat!=0) {
      *istat=blas_error_param;
      return NULL;
    }
  dsp_data->FIDA=BSR_FORMAT;
  dsp_data->M=m;
  dsp_data->K=n;
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
  nnz =n_bindx;//no. of nonzero blocks
  set_infoa(dsp_data->INFOA,'n',nnz,istat);
  set_infoa(dsp_data->INFOA,'d',lb,istat);//row-dim of a block
  set_infoa(dsp_data->INFOA,'e',lb,istat);//col-dim of a block
  set_infoa(dsp_data->INFOA,'f',mb,istat);//row-dim in blocks
  set_infoa(dsp_data->INFOA,'g',kb,istat);//col-dim in blocks
      if((mb!=n_bpntrb)||(m!=mb*lb)||(n!=kb*lb)||(minval(bindx,n_bindx)<base)||(maxval(bindx,n_bindx)>kb-1+base)||(mb!=n_bpntre)||(nnz*lb*lb!=n_val))
  {
    BLAS_usds(dsp_l,3);
    *istat = blas_error_param;
    return NULL;
  }



  dsp_data->n_A=n_val;
  dsp_data->n_IA1=n_bindx;
  dsp_data->n_PB=n_bpntrb;
  dsp_data->n_PE=n_bpntre;
  dsp_data->n_BP1=0;
  dsp_data->n_BP2=0;
  dsp_data->n_IA2=0;
  if(options>0) {//then
      // decision rule whether or not to copy
      COPY=TRUE;
      if(COPY) {//then
          options=-1; //copy
        }else{
          options=0;  //reference
        }//end if
    }//end if
  if (options==0) {
      set_infoa(dsp_data->INFOA,'c',REF_OF_SOURCE,istat);
      if(*istat!=0) {
          *istat=blas_error_param;
          return NULL;
        }
      // create reference to original matrix
      dsp_data->A=val;
      dsp_data->IA1=bindx;
      dsp_data->PB=bpntrb;
      dsp_data->PE=bpntre;
      *istat=0;
    }
  else
    {
      // The additional required memory is DEALLOCATED later in USDS!
      set_infoa(dsp_data->INFOA,'c',COP_OF_SOURCE,istat);
      if(*istat!=0) {
          *istat=blas_error_param;
          return NULL;
        }
      // copy original data
      dsp_data->A=(double*)aligned_malloc (sizeof(double)*n_val);
      dsp_data->IA1=(int*)aligned_malloc (sizeof(int)*n_bindx);
      dsp_data->PB=(int*)aligned_malloc (sizeof(int)*n_bpntrb);
      dsp_data->PE=(int*)aligned_malloc (sizeof(int)*n_bpntre);


      if(*istat!=0) {
          *istat=blas_error_memalloc;
          return NULL;
        }
      for(i=0;i<dsp_data->n_A;i++)
        dsp_data->A[i]=val[i];
      for(i=0;i<dsp_data->n_IA1;i++)
        dsp_data->IA1[i]=bindx[i];
      for(i=0;i<dsp_data->n_PB;i++)
        dsp_data->PB[i]=bpntrb[i];
      for(i=0;i<dsp_data->n_PE;i++)
        dsp_data->PE[i]=bpntre[i];
      *istat=1;
    }
  if(*istat>=0)
    {
      *istat=0;
      return dsp_l;
    }
  else
    return NULL;
}

