      module mod_rsbv_bsc
// **********************************************************************
//     Author : C. Voemel
//     Date of last modification : 7.7.00
//     Description : PERFORMS TRI. SOLVE WITH MATRIX IN 'BSC'-STORAGE
//                   rsbv=Right Solve By Vector
// **********************************************************************
                  
      interface rsbv_bsc
        module procedure irsbv_bsc
        module procedure srsbv_bsc
        module procedure drsbv_bsc
        module procedure crsbv_bsc
        module procedure zrsbv_bsc
      end interface
      #include "sbv.h"
// **********************************************************************
// **********************************************************************
      void irsbv_bsc (ISPMAT* mat,int* x,int n_x,int* ierr)
      {
      
int j,n,base,pntr,ofs,bofs,mm,nn,mb,nb,dd,nn_sq;
      char diag,part,store;
      int* y;
      *ierr=-1;
      n=n_x;
       get_infoa(mat->INFOA,'b',&base,*ierr);
      
      ofs=1-base;
      bofs=-base;
       get_infoa(mat->INFOA,'d',&mm,*ierr);
      
       get_infoa(mat->INFOA,'e',&nn,*ierr);
      
       get_infoa(mat->INFOA,'f',&mb,*ierr);
      
       get_infoa(mat->INFOA,'g',&nb,*ierr);
      
       get_descra(mat->DESCRA,'d',&diag,*ierr);
      
       get_descra(mat->DESCRA,'f',&store,*ierr);
      
       get_descra(mat->DESCRA,'a',&part,*ierr);
      
      if ((part!='U')&&(part!='L')) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      *ierr=-1;
      if ((mat->FIDA!='BSC')||(mat->M!=n)||(mat->K!=n)||(mm!=nn)||(nb!=mb)) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      nn_sq=nn*nn;
      y=malloc(nn*sizeof(y*));
      
      y=0 
      if (part=='L') {//then
         for(j=0;j<nb;j++){//end for
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_left_lower(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr)
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }else{/*else*/ 
         do(j=nb-1;j>=0;j--){//do
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_right_upper(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr);
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }//end if
} //end function irsbv_bsc 
// **********************************************************************
// **********************************************************************
      void srsbv_bsc (ISPMAT* mat,int* x,int n_x,int* ierr)
      {
      
      int j,n,base,pntr,ofs,bofs,mm,nn,mb,nb,dd,nn_sq;
      char diag,part,store;
      float* y;
      *ierr=-1;
      n=n_x;
       get_infoa(mat->INFOA,'b',&base,*ierr);
      
      ofs=1-base;
      bofs=-base;
       get_infoa(mat->INFOA,'d',&mm,*ierr);
      
       get_infoa(mat->INFOA,'e',&nn,*ierr);
      
       get_infoa(mat->INFOA,'f',&mb,*ierr);
      
       get_infoa(mat->INFOA,'g',&nb,*ierr);
      
       get_descra(mat->DESCRA,'d',&diag,*ierr);
      
       get_descra(mat->DESCRA,'f',&store,*ierr);
      
       get_descra(mat->DESCRA,'a',&part,*ierr);
      
      if ((part!='U')&&(part!='L')) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      *ierr=-1;
      if ((mat->FIDA!='BSC')||(mat->M!=n)||(mat->K!=n)||(mm!=nn)||(nb!=mb)) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      nn_sq=nn*nn;
      y=malloc(nn*sizeof(y*));
      
      y=0.0e0 
      if (part=='L') {//then
         for(j=0;j<nb;j++){//end for
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_left_lower(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr)
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }else{/*else*/ 
         do(j=nb-1;j>=0;j--){//do
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_right_upper(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr);
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }//end if
} //end function srsbv_bsc 
// **********************************************************************
// **********************************************************************
      void drsbv_bsc (ISPMAT* mat,int* x,int n_x,int* ierr)
      {
      
      
      int j,n,base,pntr,ofs,bofs,mm,nn,mb,nb,dd,nn_sq;
      char diag,part,store;
      double* y;
      *ierr=-1;
      n=n_x;
       get_infoa(mat->INFOA,'b',&base,*ierr);
      
      ofs=1-base;
      bofs=-base;
       get_infoa(mat->INFOA,'d',&mm,*ierr);
      
       get_infoa(mat->INFOA,'e',&nn,*ierr);
      
       get_infoa(mat->INFOA,'f',&mb,*ierr);
      
       get_infoa(mat->INFOA,'g',&nb,*ierr);
      
       get_descra(mat->DESCRA,'d',&diag,*ierr);
      
       get_descra(mat->DESCRA,'f',&store,*ierr);
      
       get_descra(mat->DESCRA,'a',&part,*ierr);
      
      if ((part!='U')&&(part!='L')) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      *ierr=-1;
      if ((mat->FIDA!='BSC')||(mat->M!=n)||(mat->K!=n)||(mm!=nn)||(nb!=mb)) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      nn_sq=nn*nn;
      y=malloc(nn*sizeof(y*));
      
      y=0.0d0 
      if (part=='L') {//then
         for(j=0;j<nb;j++){//end for
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_left_lower(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr)
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }else{/*else*/ 
         do(j=nb-1;j>=0;j--){//do
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_right_upper(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr);
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }//end if
} //end function drsbv_bsc 
// **********************************************************************
// **********************************************************************
      void crsbv_bsc (ISPMAT* mat,int* x,int n_x,int* ierr)
      {
      int j,n,base,pntr,ofs,bofs,mm,nn,mb,nb,dd,nn_sq;
      char diag,part,store;
      complex_f* y;
      *ierr=-1;
      n=n_x;
       get_infoa(mat->INFOA,'b',&base,*ierr);
      
      ofs=1-base;
      bofs=-base;
       get_infoa(mat->INFOA,'d',&mm,*ierr);
      
       get_infoa(mat->INFOA,'e',&nn,*ierr);
      
       get_infoa(mat->INFOA,'f',&mb,*ierr);
      
       get_infoa(mat->INFOA,'g',&nb,*ierr);
      
       get_descra(mat->DESCRA,'d',&diag,*ierr);
      
       get_descra(mat->DESCRA,'f',&store,*ierr);
      
       get_descra(mat->DESCRA,'a',&part,*ierr);
      
      if ((part!='U')&&(part!='L')) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      *ierr=-1;
      if ((mat->FIDA!='BSC')||(mat->M!=n)||(mat->K!=n)||(mm!=nn)||(nb!=mb)) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      nn_sq=nn*nn;
      y=malloc(nn*sizeof(y*));
      
      y=(0.0e0,0.0e0) 
      if (part=='L') {//then
         for(j=0;j<nb;j++){//end for
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_left_lower(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr)
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }else{/*else*/ 
         do(j=nb-1;j>=0;j--){//do
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_right_upper(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr);
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }//end if
} //end function crsbv_bsc 
// **********************************************************************
// **********************************************************************
      void zrsbv_bsc (ISPMAT* mat,int* x,int n_x,int* ierr)
      {
      int j,n,base,pntr,ofs,bofs,mm,nn,mb,nb,dd,nn_sq;
      char diag,part,store;
      complex_d* y;
      *ierr=-1;
      n=n_x;
       get_infoa(mat->INFOA,'b',&base,*ierr);
      
      ofs=1-base;
      bofs=-base;
       get_infoa(mat->INFOA,'d',&mm,*ierr);
      
       get_infoa(mat->INFOA,'e',&nn,*ierr);
      
       get_infoa(mat->INFOA,'f',&mb,*ierr);
      
       get_infoa(mat->INFOA,'g',&nb,*ierr);
      
       get_descra(mat->DESCRA,'d',&diag,*ierr);
      
       get_descra(mat->DESCRA,'f',&store,*ierr);
      
       get_descra(mat->DESCRA,'a',&part,*ierr);
      
      if ((part!='U')&&(part!='L')) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      *ierr=-1;
      if ((mat->FIDA!='BSC')||(mat->M!=n)||(mat->K!=n)||(mm!=nn)||(nb!=mb)) {//then
         *ierr=blas_error_param;
         return;
      }//end if
      nn_sq=nn*nn;
      y=malloc(nn*sizeof(y*));
      
      y=(0.0d0,0.0d0) 
      if (part=='L') {//then
         for(j=0;j<nb;j++){//end for
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_left_lower(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr)
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }else{/*else*/ 
         do(j=nb-1;j>=0;j--){//do
            if (diag=='U') {//then
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr); 
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  pntr++;
               }//end do 
            }else{/*else*/
               pntr=mat->pb[j];
               dd=-1;
               while((pntr<mat->pe[j])&&(mat->IA1[pntr+ofs]+ofs!=j)){//while
                  pntr++;
               }//end do
               if (mat->IA1[pntr+ofs]+ofs==j) {//then
                  dd=pntr;
               }//end if
               if(dd==-1) {//then
                  *ierr=blas_error_singtria;
                  return;
               }else{/*else*/
                   invert_right_upper(mat->A[(dd+bofs)*nn_sq+1][(dd+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,store,*ierr);
               }//end if
               if(*ierr!=0) {//then
                  *ierr=blas_error_singtria;
                  return;
               }//end if
               pntr=mat->pb[j];
               while(pntr<mat->pe[j]){//while
                  if(mat->IA1[pntr+ofs]+ofs!=j) {//then
                   block_mult_vec(mat->A[(pntr+bofs)*nn_sq+1][(pntr+bofs+1)*nn_sq],x[j*nn+1][j*nn],nn,y,nn,store,*ierr);
      x[(mat->IA1[pntr+ofs]+bofs)*nn+1][(mat->IA1[pntr+ofs]+bofs+1)*nn]-=y;
                  }//end if 
                  pntr++;
               }//end do 
            }//end if
         }//end do
         free(y);
         if(*ierr!=0) {//then
            *ierr=blas_error_memdeloc;
            return;
         }//end if
      }//end if
} //end function zrsbv_bsc 
// **********************************************************************
// **********************************************************************
      end module mod_rsbv_bsc
