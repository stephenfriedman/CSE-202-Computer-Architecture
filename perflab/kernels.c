


/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
//
/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "SAF217+REF217",              /* Team name */

    "Stephen A. Friedman",     /* First member full name */
    "saf217@lehigh.edu",  /* First member email address */

    "Ryan E. Fitzgerald",                   /* Second member full name (leave blank if none) */
    "ref217@lehigh.edu"                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
  int i, j;
  for(i=0; i < dim; i++)
    {
      for(j=0; j < dim; j++)
	{
	  dst[RIDX(dim-1-j,i,dim)] = src[RIDX(i,j,dim)];
	}
    }
  
 }

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
  int i;
  for (i = 0; i < dim*dim; i++)
    {
       int newJ = i/dim;
      int oldJ = i%dim;
      int newI = dim-1-oldJ;
      dst[dim*newI+newJ] = src[i];
      
      
      }
  return;
}


void rotate1(int dim, pixel *src, pixel *dst)
{
  int i, j, i1,j1;
  for(i = 0; i < dim; i+=32)
    {
      for(j = 0; j < dim; j+=32)
	{ 
	      for(i1 = i; i1 < i+32; i1++)
		{
		  
		  for(j1 = j; j1 < j+32; j1++)
		    {
		      dst[RIDX(dim-1-j1,i1,dim)] = src[RIDX(i1,j1,dim)];
		      /*int spot = i1*dim+j1;
		      int newJ = spot/dim;
		      int oldJ = spot%dim;
		      int newI = dim-1-oldJ;
		      dst[dim*newI+newJ] = src[spot];
		      */
		    }
		}
	}
    }


  return;
}

void rotate3(int dim, pixel *src, pixel *dst)
{
  int i, j, i1, j1,x,dimmi1m1;

  int count = 0;

  for (i = 0; i < dim; i+=32){

    for (j = 0; j < dim; j+=32){

      for(i1 = i; i1 < i +32; i1++){

	 x = dim * i1;
	 dimmi1m1 = dim-i1-1;

	for(j1 = j; j1 < j + 32; j1++){

	  dst[x + j1] = src[dimmi1m1 + (dim * j1)];

	}

      }

    }

  }
}
/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
  //add_rotate_function(&naive_rotate, naive_rotate_descr);   
    //add_rotate_function(&rotate, rotate_descr);
    add_rotate_function(&rotate3, rotate_descr);
    //
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
78 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}
 
/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    naive_smooth(dim, src, dst);
}

void smooth2(int dim, pixel *src, pixel *dst)
{
  int sumRed,sumGreen,sumBlue;
  int x,y,i,j,ii,jj,imm,ipp,jmm,jpp,dimdim1,dimdim2;
  int block=32;
  int dim1=dim-1;
  int dim2=dim-2;
  
  
  //C's
  /*for(ii=0;ii<dim-1;ii+=block)
    {
      for(jj=0;jj<dim-1;jj+=block)
	{*/
  for(i=1;i<dim1;i++)
    {
      for(j=1;j<dim1;j++)
	{
	  y=dim*i;
	  x=y+j;
	  imm=i-1;
	  ipp=i+1;
	  jmm=j-1;
	  jpp=j+1;
	  
	  sumRed = src[dim*(imm)+(jmm)].red+
	    src[dim*(imm)+j].red+
	    src[dim*(imm)+(jpp)].red+
	    src[y+(jmm)].red+
	    src[x].red+
	    src[y+(jpp)].red+
	    src[dim*(ipp)+(jmm)].red+
	    src[dim*(ipp)+j].red+
	    src[dim*(ipp)+(jpp)].red;
	  
	  sumGreen =src[dim*(imm)+(jmm)].green+
	    src[dim*(imm)+j].green+
	    src[dim*(imm)+(jpp)].green+
	    src[y+(jmm)].green+
	    src[x].green+
	    src[y+(jpp)].green+
	    src[dim*(ipp)+(jmm)].green+
	    src[dim*(ipp)+j].green+
	    src[dim*(ipp)+(jpp)].green;

	  sumBlue = src[dim*(imm)+(jmm)].blue+
	    src[dim*(imm)+j].blue+
	    src[dim*(imm)+(jpp)].blue+
	    src[y+(jmm)].blue+
	    src[x].blue+
	    src[y+(jpp)].blue+
	    src[dim*(ipp)+(jmm)].blue+
	    src[dim*(ipp)+j].blue+
	    src[dim*(ipp)+(jpp)].blue;
	  //
	  dst[x].red = sumRed/9;
	  dst[x].green = sumGreen/9;
	  dst[x].blue = sumBlue/9;
	  //}
	  // }
	}
    }

  int dimj,dimjpp,dimjmm;
  //B Top
  for(j=1;j<dim1;j++)
    {
      jmm=j-1;
      jpp=j+1;
      dimj = dim+j;
      dimjpp=dim+jpp;
      dimjmm=dim+jmm;

      sumRed=src[j].red+
      src[jpp].red+
      src[jmm].red+
      src[dimj].red+
      src[dimjpp].red+
      src[dimjmm].red;

      sumGreen=src[j].green+
      src[jpp].green+
      src[jmm].green+
      src[dimj].green+
      src[dimjpp].green+
      src[dimjmm].green;

      sumBlue=src[j].blue+
      src[jpp].blue+
      src[jmm].blue+
      src[dimj].blue+
      src[dimjpp].blue+
      src[dimjmm].blue;

dst[j].red = sumRed/6; 
dst[j].green = sumGreen/6;
dst[j].blue = sumBlue/6;

        jmm=j-1;
    jpp=j+1;
    dimj= dim*j;
    dimjmm=dim*jmm;
    dimjpp=dim*jpp;
    dimdim2=dim*dim2;
    dimdim1=dim*dim1;
    
    sumRed=src[dimdim2+j].red+
    src[dimdim2+jpp].red+
    src[dimdim2+jmm].red+
    src[dimdim1+j].red+
    src[dimdim1+jpp].red+
    src[dimdim1+jmm].red;

sumGreen=src[dimdim2+j].green+
src[dimdim2+jpp].green+
src[dimdim2+jmm].green+
src[dimdim1+j].green+
src[dimdim1+jpp].green+
src[dimdim1+jmm].green;

sumBlue=src[dimdim2+j].blue+
  src[dimdim2+jpp].blue+
  src[dimdim2+jmm].blue+
  src[dimdim1+j].blue+
  src[dimdim1+jpp].blue+
  src[dimdim1+jmm].blue;


//x = dim*(dim1)+j;
dst[dimdim1+j].red = sumRed/6;
dst[dimdim1+j].green = sumGreen/6;
dst[dimdim1+j].blue = sumBlue/6;


}

  int dimi, dimimm,dimipp;
  //B Left
  for(i=1; i<dim1;i++)
    {
      imm=i-1;
      ipp=i+1;
      dimi= dim*i;
      dimimm=dim*imm;
      dimipp=dim*ipp;
      sumRed = src[dimimm].red+
	src[dimi].red+
	src[dimipp].red+
	src[dimimm+1].red+
	src[dimi+1].red+
	src[dimipp+1].red;

      sumGreen = src[dimimm].green+
	src[dimi].green+
	src[dimipp].green+
	src[dimimm+1].green+
	src[dimi+1].green+
	src[dimipp+1].green;


      sumBlue = src[dimimm].blue+
	src[dimi].blue+
	src[dimipp].blue+
	src[dimimm+1].blue+
	src[dimi+1].blue+
	src[dimipp+1].blue;
      
      dst[dimi].red = sumRed/6;
      dst[dimi].green = sumGreen/6;
      dst[dimi].blue = sumBlue/6;
      

    imm=i-1;
    ipp=i+1;
    dimimm=dim*imm;
    dimi=dim*i;
    dimipp=dim*ipp;
    
    sumRed=src[dimimm+dim1].red+
    src[dimi+dim1].red+
    src[dimipp+dim1].red+
    src[dimimm+dim2].red+
    src[dimi+dim2].red+
    src[dimipp+dim2].red;

    sumGreen=src[dimimm+dim1].green+
          src[dimi+dim1].green+
          src[dimipp+dim1].green+
          src[dimimm+dim2].green+
          src[dimi+dim2].green+
          src[dimipp+dim2].green;

    sumBlue=src[dimimm+dim1].blue+
          src[dimi+dim1].blue+
          src[dimipp+dim1].blue+
          src[dimimm+dim2].blue+
          src[dimi+dim2].blue+
          src[dimipp+dim2].blue;
    
    
 x = (dimi)+(dim1);
 dst[x].red = sumRed/6;
 dst[x].green = sumGreen/6;
 dst[x].blue=sumBlue/6;
    }

/*//B bottom
for(j=1;j<dim1;j++)
  {

    jmm=j-1;
    jpp=j+1;
    dimj= dim*j;
    dimjmm=dim*jmm;
    dimjpp=dim*jpp;
    dimdim2=dim*dim2;
    dimdim1=dim*dim1;
    
    sumRed=src[dimdim2+j].red+
    src[dimdim2+jpp].red+
    src[dimdim2+jmm].red+
    src[dimdim1+j].red+
    src[dimdim1+jpp].red+
    src[dimdim1+jmm].red;

sumGreen=src[dimdim2+j].green+
src[dimdim2+jpp].green+
src[dimdim2+jmm].green+
src[dimdim1+j].green+
src[dimdim1+jpp].green+
src[dimdim1+jmm].green;

sumBlue=src[dimdim2+j].blue+
  src[dimdim2+jpp].blue+
  src[dimdim2+jmm].blue+
  src[dimdim1+j].blue+
  src[dimdim1+jpp].blue+
  src[dimdim1+jmm].blue;


//x = dim*(dim1)+j;
dst[dimdim1+j].red = sumRed/6;
dst[dimdim1+j].green = sumGreen/6;
dst[dimdim1+j].blue = sumBlue/6;

}    */

/*
//B right
for(i=0;i<dim1;i++)
  {
    imm=i-1;
    ipp=i+1;
    dimimm=dim*imm;
    dimi=dim*i;
    dimipp=dim*ipp;
    
    sumRed=src[dimimm+dim1].red+
    src[dimi+dim1].red+
    src[dimipp+dim1].red+
    src[dimimm+dim2].red+
    src[dimi+dim2].red+
    src[dimipp+dim2].red;

    sumGreen=src[dimimm+dim1].green+
          src[dimi+dim1].green+
          src[dimipp+dim1].green+
          src[dimimm+dim2].green+
          src[dimi+dim2].green+
          src[dimipp+dim2].green;

    sumBlue=src[dimimm+dim1].blue+
          src[dimi+dim1].blue+
          src[dimipp+dim1].blue+
          src[dimimm+dim2].blue+
          src[dimi+dim2].blue+
          src[dimipp+dim2].blue;
    
    
 x = (dimi)+(dim1);
 dst[x].red = sumRed/6;
 dst[x].green = sumGreen/6;
 dst[x].blue=sumBlue/6;
  }               */

//A Top Left
//current_pixel = dst[0];
 sumRed=src[0].red+
 src[1].red+
 src[dim].red+
 src[dim+1].red;

 sumGreen=src[0].green+
   src[1].green+
   src[dim].green+
   src[dim+1].green;

 sumBlue=src[0].blue+
   src[1].blue+
   src[dim].blue+
   src[dim+1].blue;

 dst[0].red=sumRed>>2;
 dst[0].green=sumGreen>>2;
 dst[0].blue=sumBlue>>2;

 //A Top Right
 //current_pixel = dst[0];
 sumRed=src[dim1].red+
   src[dim2].red+
   src[dim1+dim].red+
   src[dim2+dim].red;

 sumGreen=src[dim1].green+
   src[dim2].green+
   src[dim1+dim].green+
   src[dim2+dim].green;

 sumBlue=src[dim1].blue+
   src[dim2].blue+
   src[dim1+dim].blue+
   src[dim2+dim].blue;
 // x = dim-1;
 dst[dim1].red=sumRed>>2;
 dst[dim1].green=sumGreen>>2;
 dst[dim1].blue=sumBlue>>2;

 //A Bottom Left
 //current_pixel = dst[0];
 sumRed=src[dimdim2].red+
   src[dimdim2+1].red+
   src[dimdim1].red+
   src[dimdim1+1].red;


 sumGreen=src[dimdim2].green+
      src[dimdim2+1].green+
      src[dimdim1].green+
   src[dimdim1+1].green;
 
sumBlue=src[dimdim2].blue+
     src[dimdim2+1].blue+
     src[dimdim1].blue+
  src[dimdim1+1].blue;
 
 x = dim*dim;//-dim;
 dst[x-dim].red=sumRed>>2;
 dst[x-dim].green=sumGreen>>2;
 dst[x-dim].blue=sumBlue>>2;
 
 //A Bottom Right
 //current_pixel = dst[0];
 sumRed=src[dimdim2+dim2].red+
   src[dimdim2+dim1].red+
   src[dimdim1+dim2].red+
   src[dimdim1+dim1].red;


sumGreen=src[dimdim2+dim2].green+
     src[dimdim2+dim1].green+
     src[dimdim1+dim2].green+
  src[dimdim1+dim1].green;


 sumBlue=src[dimdim2+dim2].blue+
      src[dimdim2+dim1].blue+
      src[dimdim1+dim2].blue+
   src[dimdim1+dim1].blue;
 
 x = dim*dim-1;
 dst[x].red=sumRed>>2;
 dst[x].green=sumGreen>>2;
 dst[x].blue=sumBlue>>2;
 

}

void smooth1(int dim, pixel *src, pixel *dst)
  {

    pixel_sum sum;
    //pixel current_pixel;
    initialize_pixel_sum(&sum);


    //A Top Left
    //current_pixel = dst[0];
    accumulate_sum(&sum,src[RIDX(0,0,dim)]);
    accumulate_sum(&sum,src[RIDX(0,1,dim)]);
    accumulate_sum(&sum,src[RIDX(1,0,dim)]);
    accumulate_sum(&sum,src[RIDX(1,1,dim)]);
    assign_sum_to_pixel(&dst[0], sum);


    initialize_pixel_sum(&sum);

    //A Top Right
    //current_pixel= dst[dim-1];
    accumulate_sum(&sum,src[RIDX(0,dim-1,dim)]);
    accumulate_sum(&sum, src[RIDX(0,dim-2,dim)]);
    accumulate_sum(&sum,src[RIDX(1,dim-1,dim)]);
    accumulate_sum(&sum, src[RIDX(1,dim-2,dim)]);
    assign_sum_to_pixel(&dst[dim-1], sum);


    initialize_pixel_sum(&sum);

    //A bottom Left
    //current_pixel = dst[dim*dim-dim];
    accumulate_sum(&sum,src[RIDX(dim-1,0,dim)]);
    accumulate_sum(&sum,src[RIDX(dim-2,0,dim)]);
    accumulate_sum(&sum,src[RIDX(dim-1,1,dim)]);
    accumulate_sum(&sum,src[RIDX(dim-2,1,dim)]);
    assign_sum_to_pixel(&dst[dim*dim-dim], sum);


    initialize_pixel_sum(&sum);

    //A bottom right
    //current_pixel = dst[dim*dim-1];
    accumulate_sum(&sum,src[RIDX(dim-1,dim-1,dim)]);
    accumulate_sum(&sum,src[RIDX(dim-2,dim-2,dim)]);
    accumulate_sum(&sum,src[RIDX(dim-2,dim-1,dim)]);
    accumulate_sum(&sum,src[RIDX(dim-1,dim-2,dim)]);
    assign_sum_to_pixel(&dst[dim*dim-1], sum);


    initialize_pixel_sum(&sum);




    //C's
    int i,ii,j,jj;
    int block=32;
    for(ii=0;ii<dim;ii+=block)
      {
	for(jj=0;jj<dim;jj+=block)
	  {	    
	    for(i=ii;i<ii+block;i++)
	      {


		//B Top
		//current_pixel = dst[i];
		accumulate_sum(&sum,src[RIDX(0,i,dim)]);
		accumulate_sum(&sum,src[RIDX(0,i+1,dim)]);
		accumulate_sum(&sum,src[RIDX(0,i-1,dim)]);
		accumulate_sum(&sum,src[RIDX(1,i,dim)]);
		accumulate_sum(&sum,src[RIDX(1,i+1,dim)]);
		accumulate_sum(&sum,src[RIDX(1,i-1,dim)]);

		assign_sum_to_pixel(&dst[i], sum);
		initialize_pixel_sum(&sum);

		//B Left
		//current_pixel = dst[dim*i];
		accumulate_sum(&sum,src[RIDX(i,0,dim)]);
		accumulate_sum(&sum,src[RIDX(i-1,0,dim)]);
		accumulate_sum(&sum,src[RIDX(i+1,0,dim)]);
		accumulate_sum(&sum,src[RIDX(i,1,dim)]);
		accumulate_sum(&sum,src[RIDX(i-1,1,dim)]);
		accumulate_sum(&sum,src[RIDX(i+1,1,dim)]);

		assign_sum_to_pixel(&dst[dim*i], sum);
		initialize_pixel_sum(&sum);






		//B Right
		//current_pixel = dst[(dim*i)+(dim-1)];
		accumulate_sum(&sum,src[RIDX(i,dim-1,dim)]);
		accumulate_sum(&sum,src[RIDX(i-1,dim-1,dim)]);
		accumulate_sum(&sum,src[RIDX(i+1,dim-1,dim)]);
		accumulate_sum(&sum,src[RIDX(i-1,dim-2,dim)]);
		accumulate_sum(&sum,src[RIDX(i,dim-2,dim)]);
		accumulate_sum(&sum,src[RIDX(i+1,dim-2,dim)]);

		assign_sum_to_pixel(&dst[(dim*i)+(dim-1)], sum);
		initialize_pixel_sum(&sum);

		//B bottom
		//current_pixel = dst[dim*(dim-1)+i];
		accumulate_sum(&sum,src[RIDX(dim-1,i,dim)]);
		accumulate_sum(&sum,src[RIDX(dim-1,i-1,dim)]);
		accumulate_sum(&sum,src[RIDX(dim-2,i-1,dim)]);
		accumulate_sum(&sum,src[RIDX(dim-2,i,dim)]);
		accumulate_sum(&sum,src[RIDX(dim-2,i+1,dim)]);
		accumulate_sum(&sum,src[RIDX(dim-1,i+1,dim)]);
		assign_sum_to_pixel(&dst[dim*(dim-1)+i], sum);

		initialize_pixel_sum(&sum);

		for(j=jj;j<jj+block;j++)
		  {
		    //current_pixel = dst[dim*i+j];
		    accumulate_sum(&sum,src[RIDX(i-1,j-1,dim)]);
		    accumulate_sum(&sum,src[RIDX(i-1,j,dim)]);
		    accumulate_sum(&sum,src[RIDX(i-1,j+1,dim)]);
		    accumulate_sum(&sum,src[RIDX(i,j-1,dim)]);
		    accumulate_sum(&sum,src[RIDX(i,j,dim)]);
		    accumulate_sum(&sum,src[RIDX(i,j+1,dim)]);
		    accumulate_sum(&sum,src[RIDX(i+1,j-1,dim)]);
		    accumulate_sum(&sum,src[RIDX(i+1,j,dim)]);
		    accumulate_sum(&sum,src[RIDX(i+1,j+1,dim)]);
		    assign_sum_to_pixel(&dst[dim*i+j], sum);

		    initialize_pixel_sum(&sum);
		  }
	      }
	  }
      }
  }

void smooth7(int dim, pixel *src, pixel *dst)
{
  pixel_sum sum;
  //pixel current_pixel;
  initialize_pixel_sum(&sum);

  //
  //C's
  int i,j,ii,jj;
  int block=32;
    for(ii=0;ii<dim-1;ii+=block)
    {
      for(jj=0;jj<dim-1;jj+=block)
	{
  	  for(i=ii;i<ii+block;i++)
	    {
	      for(j=jj;j<jj+block;j++)
		{
		  //current_pixel = dst[dim*i+j];
		  accumulate_sum(&sum,src[RIDX(i-1,j-1,dim)]);
		  accumulate_sum(&sum,src[RIDX(i-1,j,dim)]);
		  accumulate_sum(&sum,src[RIDX(i-1,j+1,dim)]);
		  accumulate_sum(&sum,src[RIDX(i,j-1,dim)]);
		  accumulate_sum(&sum,src[RIDX(i,j,dim)]);
		  accumulate_sum(&sum,src[RIDX(i,j+1,dim)]);
		  accumulate_sum(&sum,src[RIDX(i+1,j-1,dim)]);
		  accumulate_sum(&sum,src[RIDX(i+1,j,dim)]);
		  accumulate_sum(&sum,src[RIDX(i+1,j+1,dim)]);
		  assign_sum_to_pixel(&dst[dim*i+j], sum);

		  initialize_pixel_sum(&sum);

		  accumulate_sum(&sum,src[RIDX(0,j,dim)]);
		  accumulate_sum(&sum,src[RIDX(0,j+1,dim)]);
		  accumulate_sum(&sum,src[RIDX(0,j-1,dim)]);
		  accumulate_sum(&sum,src[RIDX(1,j,dim)]);
		  accumulate_sum(&sum,src[RIDX(1,j+1,dim)]);
		  accumulate_sum(&sum,src[RIDX(1,j-1,dim)]);

		  assign_sum_to_pixel(&dst[j], sum);
		  initialize_pixel_sum(&sum);
		  
		  
		}
	    }
	}
    }
  
  
  //A Top Left
  //current_pixel = dst[0];
  accumulate_sum(&sum,src[RIDX(0,0,dim)]);
  accumulate_sum(&sum,src[RIDX(0,1,dim)]);
  accumulate_sum(&sum,src[RIDX(1,0,dim)]);
  accumulate_sum(&sum,src[RIDX(1,1,dim)]);
  assign_sum_to_pixel(&dst[0], sum);


  initialize_pixel_sum(&sum);

  //A Top Right
  //current_pixel= dst[dim-1];
  accumulate_sum(&sum,src[RIDX(0,dim-1,dim)]);
  accumulate_sum(&sum, src[RIDX(0,dim-2,dim)]);
  accumulate_sum(&sum,src[RIDX(1,dim-1,dim)]);
  accumulate_sum(&sum, src[RIDX(1,dim-2,dim)]);
  assign_sum_to_pixel(&dst[dim-1], sum);


  initialize_pixel_sum(&sum);

  //A bottom Left
  //current_pixel = dst[dim*dim-dim];
  accumulate_sum(&sum,src[RIDX(dim-1,0,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,0,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-1,1,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,1,dim)]);
  assign_sum_to_pixel(&dst[dim*dim-dim], sum);


  initialize_pixel_sum(&sum);

  //A bottom right
  //current_pixel = dst[dim*dim-1];
  accumulate_sum(&sum,src[RIDX(dim-1,dim-1,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,dim-2,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,dim-1,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-1,dim-2,dim)]);
  assign_sum_to_pixel(&dst[dim*dim-1], sum);


  initialize_pixel_sum(&sum);

    
  

  //B Top
  //int j;
  /*for(j=1;j<dim-1;j++)
    {
      //current_pixel = dst[j];
      accumulate_sum(&sum,src[RIDX(0,j,dim)]);
      accumulate_sum(&sum,src[RIDX(0,j+1,dim)]);
      accumulate_sum(&sum,src[RIDX(0,j-1,dim)]);
      accumulate_sum(&sum,src[RIDX(1,j,dim)]);
      accumulate_sum(&sum,src[RIDX(1,j+1,dim)]);
      accumulate_sum(&sum,src[RIDX(1,j-1,dim)]);

      assign_sum_to_pixel(&dst[j], sum);
      initialize_pixel_sum(&sum);
    }

  initialize_pixel_sum(&sum);
  */
  //B Left
  //int i;
  for(i=1; i<dim-1;i++)
    {      //current_pixel = dst[dim*i];
      accumulate_sum(&sum,src[RIDX(i,0,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,0,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,0,dim)]);
      accumulate_sum(&sum,src[RIDX(i,1,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,1,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,1,dim)]);
      assign_sum_to_pixel(&dst[dim*i], sum);
      initialize_pixel_sum(&sum);

    }




  //B Right
  for(i=1; i<dim-1; i++)
    {
      //current_pixel = dst[(dim*i)+(dim-1)];
      accumulate_sum(&sum,src[RIDX(i,dim-1,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,dim-1,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,dim-1,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,dim-2,dim)]);
      accumulate_sum(&sum,src[RIDX(i,dim-2,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,dim-2,dim)]);
      assign_sum_to_pixel(&dst[(dim*i)+(dim-1)], sum);

      initialize_pixel_sum(&sum);

    }



  //B Bottom
  for(j=1; j<dim-1; j++)
    {
      //current_pixel = dst[dim*(dim-1)+j];
      accumulate_sum(&sum,src[RIDX(dim-1,j,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-1,j-1,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-2,j-1,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-2,j,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-2,j+1,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-1,j+1,dim)]);
      assign_sum_to_pixel(&dst[dim*(dim-1)+j], sum);

      initialize_pixel_sum(&sum);
    }


 
}
/*void smooth1(int dim, pixel *src, pixel *dst)
{
  pixel_sum sum;
  //pixel current_pixel;
  initialize_pixel_sum(&sum);
  

  //A Top Left
  //current_pixel = dst[0];
  accumulate_sum(&sum,src[RIDX(0,0,dim)]);
  accumulate_sum(&sum,src[RIDX(0,1,dim)]);
  accumulate_sum(&sum,src[RIDX(1,0,dim)]);
  accumulate_sum(&sum,src[RIDX(1,1,dim)]);
  assign_sum_to_pixel(&dst[0], sum);

  
  initialize_pixel_sum(&sum);

  //A Top Right
  //current_pixel= dst[dim-1];
  accumulate_sum(&sum,src[RIDX(0,dim-1,dim)]);
  accumulate_sum(&sum, src[RIDX(0,dim-2,dim)]);
  accumulate_sum(&sum,src[RIDX(1,dim-1,dim)]);
  accumulate_sum(&sum, src[RIDX(1,dim-2,dim)]);
  assign_sum_to_pixel(&dst[dim-1], sum);

  
  initialize_pixel_sum(&sum);

  //A bottom Left
  //current_pixel = dst[dim*dim-dim];
  accumulate_sum(&sum,src[RIDX(dim-1,0,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,0,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-1,1,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,1,dim)]);
  assign_sum_to_pixel(&dst[dim*dim-dim], sum);

  
  initialize_pixel_sum(&sum);

  //A bottom right
  //current_pixel = dst[dim*dim-1];
  accumulate_sum(&sum,src[RIDX(dim-1,dim-1,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,dim-2,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-2,dim-1,dim)]);
  accumulate_sum(&sum,src[RIDX(dim-1,dim-2,dim)]);
  assign_sum_to_pixel(&dst[dim*dim-1], sum);

  
  initialize_pixel_sum(&sum);

  //B Top
  int j;
  for(j=1;j<dim-1;j++)
    {
      //current_pixel = dst[j];
      accumulate_sum(&sum,src[RIDX(0,j,dim)]);
      accumulate_sum(&sum,src[RIDX(0,j+1,dim)]);
      accumulate_sum(&sum,src[RIDX(0,j-1,dim)]);
      accumulate_sum(&sum,src[RIDX(1,j,dim)]);
      accumulate_sum(&sum,src[RIDX(1,j+1,dim)]);
      accumulate_sum(&sum,src[RIDX(1,j-1,dim)]);

      assign_sum_to_pixel(&dst[j], sum);
      initialize_pixel_sum(&sum);
    }

  initialize_pixel_sum(&sum);

  //B Left
  int i;
  for(i=1; i<dim-1;i++)
    {      //current_pixel = dst[dim*i];
      accumulate_sum(&sum,src[RIDX(i,0,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,0,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,0,dim)]);
      accumulate_sum(&sum,src[RIDX(i,1,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,1,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,1,dim)]);
      assign_sum_to_pixel(&dst[dim*i], sum);
      initialize_pixel_sum(&sum);
      
    }

 
 

  //B Right
  for(i=1; i<dim-1; i++)
    {
      //current_pixel = dst[(dim*i)+(dim-1)];
      accumulate_sum(&sum,src[RIDX(i,dim-1,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,dim-1,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,dim-1,dim)]);
      accumulate_sum(&sum,src[RIDX(i-1,dim-2,dim)]);
      accumulate_sum(&sum,src[RIDX(i,dim-2,dim)]);
      accumulate_sum(&sum,src[RIDX(i+1,dim-2,dim)]);
      assign_sum_to_pixel(&dst[(dim*i)+(dim-1)], sum);
      
      initialize_pixel_sum(&sum);
      
    }

  

  //B Bottom
  for(j=1; j<dim-1; j++)
    {
      //current_pixel = dst[dim*(dim-1)+j];
      accumulate_sum(&sum,src[RIDX(dim-1,j,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-1,j-1,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-2,j-1,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-2,j,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-2,j+1,dim)]);
      accumulate_sum(&sum,src[RIDX(dim-1,j+1,dim)]);
      assign_sum_to_pixel(&dst[dim*(dim-1)+j], sum);
      
      initialize_pixel_sum(&sum);
    }
  
  

      //C's
      for(i=1;i<dim-1;i++)
	{
	  for(j=1;j<dim-1;j++)
	    {
	      //current_pixel = dst[dim*i+j];
			accumulate_sum(&sum,src[RIDX(i-1,j-1,dim)]);
			accumulate_sum(&sum,src[RIDX(i-1,j,dim)]);
			accumulate_sum(&sum,src[RIDX(i-1,j+1,dim)]);
			accumulate_sum(&sum,src[RIDX(i,j-1,dim)]);
			accumulate_sum(&sum,src[RIDX(i,j,dim)]);
			accumulate_sum(&sum,src[RIDX(i,j+1,dim)]);
			accumulate_sum(&sum,src[RIDX(i+1,j-1,dim)]);
			accumulate_sum(&sum,src[RIDX(i+1,j,dim)]);
			accumulate_sum(&sum,src[RIDX(i+1,j+1,dim)]);
			assign_sum_to_pixel(&dst[dim*i+j], sum);
			
			initialize_pixel_sum(&sum);
	    }
	}
}*/


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
  //add_smooth_function(&smooth, smooth_descr);
    //add_smooth_function(&smooth, naive_smooth_descr);
    // add_smooth_function(&smooth7, "smooth7");
    add_smooth_function(&smooth2, "smooth2");
    /* ... Register additional test functions here */
}

