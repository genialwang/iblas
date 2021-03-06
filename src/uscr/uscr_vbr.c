#include "uscr/uscr_vbr.h"
dsp_linknode* duscr_vbr(int m,int n,double* val,int n_val,int* indx,int n_indx,int* bindx,int n_bindx,int* rpntr,int n_rpntr,int* cpntr,int n_cpntr,int* bpntrb,int n_bpntrb,int* bpntre,int n_bpntre,int mb,int kb,int prpty,int* istat)
{
  int  options,base ,nnz;
  bool  COPY;
  char  message;
  DSPMAT* dsp_data;
  dsp_linknode* dsp_l;
  int i;
  options=*istat;
  *istat=-1; //if not changed later,routine has failed
  dsp_l=new_dsp(istat);
  if(*istat!=0) {//then
      *istat=blas_error_memalloc;
      return NULL;
    }//end if
  dsp_data=accessdata_dsp(dsp_l,istat);
  if(*istat!=0) {//then
      *istat=blas_error_param;
      return NULL;
    }//end if
  dsp_data->FIDA=VBR_FORMAT;
  dsp_data->M=m;
  dsp_data->K=n;
  set_descra(dsp_data->DESCRA,prpty,istat);
  get_descra(dsp_data->DESCRA,'b',&message,istat);
  if(*istat!=0) {//then
      *istat=blas_error_param;
      return NULL;
    }//end if
  if(message=='C') {//then
      base=C_BASE;
    }else{ //Assuming F base
      base=F_BASE;
    }//end if
  set_infoa(dsp_data->INFOA,'b',base,istat);
  nnz=n_bindx; //no. of nonzero blocks
  set_infoa(dsp_data->INFOA,'n',nnz,istat);
  set_infoa(dsp_data->INFOA,'d',-1,istat); //row-dim of block NOT fixed
  set_infoa(dsp_data->INFOA,'e',-1,istat); //col-dim of block NOT fixed
  set_infoa(dsp_data->INFOA,'f',mb,istat); //row-dim in blocks
  set_infoa(dsp_data->INFOA,'g',kb,istat); //col-dim in blocks
  if((mb!=n_bpntrb)||(mb!=n_bpntre)||(n_val!=maxval(indx,n_indx)-base)||(minval(indx,n_indx)<base)||(minval(bindx,n_bindx)<base)||(maxval(bindx,n_bindx)>kb-1+base)) {//then

      BLAS_usds(dsp_l,3);
          *istat=blas_error_param;
      return NULL;
    }//end if
  if(options>0) {//then
      // decision rule whether or not to copy
      COPY=TRUE;
      if(COPY) {//then
          options=-1; //copy
        }else{
          options=0;  //reference
        }//end if
    }//end if
  if(options==0) {//then
      set_infoa(dsp_data->INFOA,'c',REF_OF_SOURCE,istat);
      if(*istat!=0) {//then
          *istat=blas_error_param;
          return NULL;
        }//end if
      // create reference to original matrix

      dsp_data->A=(double*)val;
      dsp_data->IA1=bindx;
      dsp_data->IA2=indx;
      dsp_data->PB=bpntrb;
      dsp_data->PE=bpntre;
      dsp_data->BP1=rpntr;
      dsp_data->BP2=cpntr;

      dsp_data->n_A=n_val;
      dsp_data->n_IA1=n_bindx;
      dsp_data->n_IA2=n_indx;
      dsp_data->n_PB=n_bpntrb;
      dsp_data->n_PE=n_bpntre;
      dsp_data->n_BP1=n_rpntr;
      dsp_data->n_BP2=n_cpntr;

      *istat=0;
    }else{
      // The additional required memory is DEALLOCATED later in USDS//
      set_infoa(dsp_data->INFOA,'c',COP_OF_SOURCE,istat);
      if(*istat!=0) {//then
          *istat=blas_error_param;
          return NULL;
        }//end if
      // copy original data

      dsp_data->A=(double*)aligned_malloc(sizeof(*dsp_data->A)*n_val);
      dsp_data->IA1=(int*)aligned_malloc(sizeof(*dsp_data->IA1)*n_bindx);
      dsp_data->IA2=(int*)aligned_malloc(sizeof(*dsp_data->IA2)*n_indx);
      dsp_data->PB=(int*)aligned_malloc(sizeof(*dsp_data->PB)*n_bpntrb);
      dsp_data->PE=(int*)aligned_malloc(sizeof(*dsp_data->PE)*n_bpntre);
      dsp_data->BP1=(int*)aligned_malloc(sizeof(*dsp_data->BP1)*n_rpntr);
      dsp_data->BP2=(int*)aligned_malloc(sizeof(*dsp_data->BP2)*n_cpntr);

      dsp_data->n_A=n_val;
      dsp_data->n_IA1=n_bindx;
      dsp_data->n_IA2=n_indx;
      dsp_data->n_PB=n_bpntrb;
      dsp_data->n_PE=n_bpntre;
      dsp_data->n_BP1=n_rpntr;
      dsp_data->n_BP2=n_cpntr;


      //assign value
      for(i=0;i<n_val;i++)
        dsp_data->A[i]=val[i];
      for(i=0;i<n_bindx;i++)
        dsp_data->IA1[i]=bindx[i];
      for(i=0;i<n_indx;i++)
      dsp_data->IA2[i]=indx[i];
      for(i=0;i<n_bpntrb;i++)
      dsp_data->PB[i]=bpntrb[i];
      for(i=0;i<n_bpntre;i++)
      dsp_data->PE[i]=bpntre[i];
      for(i=0;i<n_rpntr;i++)
      dsp_data->BP1[i]=rpntr[i];
      for(i=0;i<n_cpntr;i++)
      dsp_data->BP2[i]=cpntr[i];
          *istat=1;
    }//end if
  if(*istat>=0)
{
*istat=0;
  return dsp_l;
 }
 else return NULL;
}//end subroutine duscr_vbr
