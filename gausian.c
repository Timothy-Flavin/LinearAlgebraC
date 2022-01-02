#include <stdio.h>
#include <cstdlib>
// r1,r2 rows to switch
// left right, left and right sides of equation
// l_cols, r_cols number of columns on the left and right sides
void swap_r(int r1, int r2, double* mat, int l_cols, int r_cols){
  int cols = l_cols+r_cols;
  r1=r1*cols;
  r2=r2*cols;
  for(int i=0; i<cols; ++i){
    int temp = mat[r1+i];
    mat[r1+i] = mat[r2+i];
    mat[r2+i] = temp;
  }
}

// Adds r1 to r2
// left right, left and right sides of equation
// l_cols, r_cols number of columns on the left and right sides
// a scalar to multiply i by
void add_r(int r1, int r2, double a, double* mat, int l_cols, int r_cols){
  int cols = l_cols+r_cols;
  r1=r1*cols;
  r2=r2*cols;
  for(int i=0; i<cols; ++i){
    mat[r2+i] += mat[r1+i] * a;
  }
}

// i row to scale
// left right, left and right sides of equation
// l_cols, r_cols number of columns on the left and right sides
// a scalar to multiply i by
void scale_r(int r, double a, double* mat, int l_cols, int r_cols){
  int cols = l_cols+r_cols;
  r=r*cols;
  for(int i=0; i<cols; ++i){
    mat[r+i] *= a;
  }
}


void print_mat(int r, int lc, int rc, double* mat){
  int cols=lc+rc;
  printf("r: %i, lc: %i, rc: %i\n",r,lc,rc);
  for(int i=0; i<r; ++i){
    for(int j=0; j<lc; ++j){
      printf("%7.3f, ", mat[i*cols+j]);
    }
    printf(" | ");
    for(int j=0; j<rc; ++j){
      printf("%7.3f, ", mat[i*cols+lc+j]);
    }
    printf("\n");
  }
}

// Performas gausian elimination and then back substitution
// by adding the solved rows back to the original rows
// Somewhere in between gausian and gaus jordan, weird order.
// 1 2 =  1
// 2 3 =  1
// ----------------------
// 1 2 = 1
// 0 1 = 1
//
// 1 0 = -1
// 0 1 = 1
void g_elim(int rows, int l_cols, int r_cols, double* mat, bool debug=false){
  if(l_cols>rows){
    printf("Not implemented yet, needs to have l_cols<=rows");
    return;
  }
  print_mat(rows, l_cols, r_cols, mat);
  int cols = l_cols+r_cols;
  for(int c=0; c<l_cols-1; c++){
    int pivot=-1, r=c;
    while(r<rows && pivot<0){
      if(debug) printf("Testing %i,%i for pivot: %.3f\n",r,c,mat[r*cols+c]);
      if(mat[r*cols+c]!=0.0){
        pivot = r;
        if(debug) printf("%i Pivot row found\n",pivot);
      }
      r++;
    }
    if(pivot==-1)
      return;
    if(pivot!=c){
      if(debug) printf("pivot: r%i != c: r%i, so swapping pivot and c\n",pivot,c);
      swap_r(pivot,c,mat,l_cols,r_cols);
      if(debug) print_mat(rows, l_cols, r_cols, mat);
    }
    for(r=c+1; r<rows; ++r){
      double a = -(mat[r*cols+c]/mat[c*cols+c]);
      if(debug) printf("adding %.3f*r%i to r%i\n",a,c,r);
      add_r(c, r, a, mat, l_cols, r_cols);
      if(debug) print_mat(rows, l_cols, r_cols, mat);
    }
    if(debug) printf("Scaling pivot row to 1 p: %.3f, c: %.3f, a: %.3f\n",pivot,c, 1/mat[c*cols+c]);
    scale_r(c, 1/mat[c*cols+c], mat, l_cols, r_cols);
    if(debug) print_mat(rows, l_cols, r_cols, mat);
    
    if(debug) printf("---------------------------------------\n");
  }
  if(mat[(rows-1)*cols+(rows-1)] != 0.0){
    if(debug) printf("Scaling final row to 1");
    scale_r(rows-1, 1/mat[(rows-1)*cols+(rows-1)], mat, l_cols, r_cols);
    if(debug) print_mat(rows, l_cols, r_cols, mat); 
    if(debug) printf("---------------------------------------\n");
  }
  
  for(int c=l_cols-1; c>0; --c){
    if(mat[c*cols+c]==0){
      if(debug) printf("r%i diagonal is zero. skipping it...");
      continue;
    }
    for(int r=c-1; r>-1; --r){
      double a = -(mat[r*cols+c]/mat[c*cols+c]);
      if(debug) printf("adding %.3f*r%i to r%i\n",a,c,r);
      add_r(c, r, a, mat, l_cols, r_cols);
      if(debug) print_mat(rows, l_cols, r_cols, mat);
    }
    if(debug) printf("---------------------------------------\n");
  }  
}

int main(int argc, char** argv) {
  int r=3, c=4;
  double* mat = (double*) malloc(r*c*sizeof(double));
  //for(int i=0; i<12; ++i){
    //mat[i]=i+1;
  //}
  double m[12] = {1,1,1,1,1,2,4,2,1,3,9,0};
  //mat=m;
  //mat[0]=4.0;
  g_elim(r, 3, 1, m);
  print_mat(r, 3, 1, m);
}