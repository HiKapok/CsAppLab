/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "study",              /* Team name */

    "kapok",     /* First member full name */
    "bovik@nowhere.edu",  /* First member email address */

    "kapok",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
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

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    //naive_rotate(dim, src, dst);
	
//	9.4
	int i,j;
	int pos = dim*dim-dim;
	for(i = 0;i < dim-7;i+=8){
		pixel * temp = &dst[pos];
		pos-=(dim<<3);
		for(j = 0;j < dim;j++){		
			int tempindex=RIDX(j,i,dim);
			temp[j]=src[tempindex++];
			temp[j-dim]=src[tempindex++];
			temp[j-(dim<<1)]=src[tempindex++];
			temp[j-dim-(dim<<1)]=src[tempindex++];
			temp[j-(dim<<2)]=src[tempindex++];
			temp[j-dim-(dim<<2)]=src[tempindex++];
			temp[j-(dim<<1)-(dim<<2)]=src[tempindex++];
			temp[j-(dim<<1)-(dim<<2)-dim]=src[tempindex];
		}
	}	
    for (i-=8; i < dim; i++){
		pixel * temp = &dst[RIDX(dim-1-i,0,dim)];
		for (j = 0; j < dim; j++)
	    temp[j] = src[RIDX(j,i,dim)];
	}
	/*9.2	
	int i,j;
	for(i = 0;i < dim-3;i+=4){
		pixel * temp = &dst[RIDX(dim-1-i,0,dim)];
		for(j = 0;j < dim;j++){		
			int tempindex=RIDX(j,i,dim);
			temp[j]=src[tempindex++];
			temp[j-dim]=src[tempindex++];
			temp[j-(dim<<1)]=src[tempindex++];
			temp[j-dim-(dim<<1)]=src[tempindex];
		}
	}*/	
	/*7.9
	int i,j;
	for(i = 0;i < dim-1;i+=2){
		pixel * temp = &dst[RIDX(dim-1-i,0,dim)];
		for(j = 0;j < dim;j++){		
			temp[j]=src[RIDX(j,i,dim)];
			temp[j-dim]=src[RIDX(j,i+1,dim)];
		}
	}*/
	/*7.3	
	int i,j;
	for(i = 0;i < dim;i++){
		pixel * temp = &dst[RIDX(dim-1-i,0,dim)];
		for(j = 0;j < dim;j++){
			
			temp[j]=src[RIDX(j,i,dim)];
		}
	}*/
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
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
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
 */
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
   //naive_smooth(dim, src, dst);
int i,j,k;
	pixel temp;
	pixel temp1;
	pixel temp2;
	temp.red=0;
	temp.green=0;
	temp.blue=0;
	for(i=1;i<dim-2;i+=2){
		pixel * first = &src[i*dim-dim];
		pixel * second = &src[i*dim];
		pixel * third = &src[i*dim+dim];
		pixel * four = &src[i*dim+(dim<<1)];
		pixel * dst1 = &dst[i*dim];
		pixel * dst2 = &dst[i*dim+dim];
		for(j=1;j<dim-2;j+=2){
			temp1.red = second[j+1].red + second[j].red;
			temp1.green = second[j+1].green + second[j].green;
			temp1.blue = second[j+1].blue + second[j].blue;
			temp.red = temp1.red + (third[j+1].red + third[j].red);
			temp.green = temp1.green + (third[j+1].green + third[j].green);
			temp.blue = temp1.blue + (third[j+1].blue + third[j].blue);
			
			temp1.red = first[j+1].red + first[j].red;
			temp1.green = first[j+1].green + first[j].green;
			temp1.blue = first[j+1].blue + first[j].blue;
			
			dst2[j+2].red = second[j-1].red + third[j-1].red;
			dst2[j+2].green = second[j-1].green + third[j-1].green;
			dst2[j+2].blue = second[j-1].blue + third[j-1].blue;
			
			dst1[j+2].red = second[j+2].red + third[j+2].red;
			dst1[j+2].green = second[j+2].green + third[j+2].green;
			dst1[j+2].blue = second[j+2].blue + third[j+2].blue;
					
			temp2.red = four[j+1].red + four[j].red;
			temp2.green = four[j+1].green + four[j].green;
			temp2.blue = four[j+1].blue + four[j].blue;
			
			dst1[j].red = (temp.red + temp1.red + first[j-1].red + dst2[j+2].red)/9;
			dst1[j].green = (temp.green + temp1.green + first[j-1].green + dst2[j+2].green)/9;
			dst1[j].blue = (temp.blue + temp1.blue + first[j-1].blue + dst2[j+2].blue)/9;
			
			dst1[j+1].red = (temp.red + temp1.red + first[j+2].red+ dst1[j+2].red)/9;
			dst1[j+1].green = (temp.green + temp1.green + first[j+2].green+ dst1[j+2].green)/9;
			dst1[j+1].blue = (temp.blue + temp1.blue + first[j+2].blue+ dst1[j+2].blue)/9;
			
			dst2[j].red = (temp.red + temp2.red + four[j-1].red + dst2[j+2].red)/9;
			dst2[j].green = (temp.green + temp2.green + four[j-1].green + dst2[j+2].green)/9;
			dst2[j].blue = (temp.blue + temp2.blue + four[j-1].blue + dst2[j+2].blue)/9;
			   
			dst2[j+1].red = (temp.red + temp2.red + four[j+2].red+ dst1[j+2].red)/9;
			dst2[j+1].green = (temp.green + temp2.green + four[j+2].green+ dst1[j+2].green)/9;
			dst2[j+1].blue = (temp.blue + temp2.blue + four[j+2].blue+ dst1[j+2].blue)/9;
		}
	}
	
	for(j=1;j<dim-1;j++){
		dst[j].red = ((src[j].red + src[j+1].red + src[j-1].red) + (src[j+dim].red + src[j+dim+1].red + src[j+dim-1].red))/6;
		dst[j].green = ((src[j].green + src[j+1].green + src[j-1].green) + (src[j+dim].green + src[j+dim+1].green + src[j+dim-1].green))/6;
		dst[j].blue = ((src[j].blue + src[j+1].blue + src[j-1].blue) + (src[j+dim].blue + src[j+dim+1].blue + src[j+dim-1].blue))/6;
	}
	pixel * first = &src[(dim-2)*dim];
	pixel * second = &src[(dim-1)*dim];
	pixel * dst1 = &dst[(dim-1)*dim];
	
	for(j=1;j<dim-1;j++){
		dst1[j].red = ((first[j].red + first[j+1].red + first[j-1].red) + (second[j].red + second[j+1].red + second[j-1].red))/6;
		dst1[j].green = ((first[j].green + first[j+1].green + first[j-1].green) + (second[j].green + second[j+1].green + second[j-1].green))/6;
		dst1[j].blue = ((first[j].blue + first[j+1].blue + first[j-1].blue) + (second[j].blue + second[j+1].blue + second[j-1].blue))/6;
	}
	
	dst1 = dst;
	pixel * src1 = src;
	for(j=1;j<dim-1;j++){
		dst1 += dim;
		dst1[0].red = ( (src1[0].red+(src1[1].red)) + (src1[dim].red+(src1[dim+1].red)) + (src1[(dim<<1)].red+(src1[(dim<<1)+1].red)))/6;
		dst1[0].green = ( (src1[0].green+(src1[1].green)) + (src1[dim].green+(src1[dim+1].green)) + (src1[(dim<<1)].green+(src1[(dim<<1)+1].green)))/6;
		dst1[0].blue = ( (src1[0].blue+(src1[1].blue)) + (src1[dim].blue+(src1[dim+1].blue)) + (src1[(dim<<1)].blue+(src1[(dim<<1)+1].blue)))/6;
		src1 += dim;
	}
	dst1 = dst+dim-1;
	src1 = src+dim-2;
	for(j=1;j<dim-1;j++){
		dst1 += dim;
		dst1[0].red = ( (src1[1].red+(src1[0].red)) + (src1[dim+1].red+(src1[dim].red)) + (src1[(dim<<1)+1].red+(src1[(dim<<1)].red)))/6;
		dst1[0].green = ( (src1[1].green+(src1[0].green)) + (src1[dim+1].green+(src1[dim].green)) + (src1[(dim<<1)+1].green+(src1[(dim<<1)].green)))/6;
		dst1[0].blue = ( (src1[1].blue+(src1[0].blue)) + (src1[dim+1].blue+(src1[dim].blue)) + (src1[(dim<<1)+1].blue+(src1[(dim<<1)].blue)))/6;
		src1 += dim;
	}
	
	dst[0].red=( (src[0].red+src[1].red) + (src[dim].red+src[1+dim].red) )/4;
	dst[0].green=( (src[0].green+src[1].green) + (src[dim].green+src[1+dim].green) )/4;
	dst[0].blue=( (src[0].blue+src[1].blue) + (src[dim].blue+src[1+dim].blue) )/4;
	
	dst[dim-1].red=( (src[dim-1].red+src[dim-2].red) + (src[(dim<<1)-1].red+src[(dim<<1)-2].red) )/4;
	dst[dim-1].green=( (src[dim-1].green+src[dim-2].green) + (src[(dim<<1)-1].green+src[(dim<<1)-2].green) )/4;
	dst[dim-1].blue=( (src[dim-1].blue+src[dim-2].blue) + (src[(dim<<1)-1].blue+src[(dim<<1)-2].blue) )/4;
	
	dst[(dim-1)*dim].red=( (src[(dim-1)*dim].red+src[(dim-1)*dim+1].red) + (src[(dim-2)*dim].red+src[1+(dim-2)*dim].red) )/4;
	dst[(dim-1)*dim].green=( (src[(dim-1)*dim].green+src[(dim-1)*dim+1].green) + (src[(dim-2)*dim].green+src[1+(dim-2)*dim].green) )/4;
	dst[(dim-1)*dim].blue=( (src[(dim-1)*dim].blue+src[(dim-1)*dim+1].blue) + (src[(dim-2)*dim].blue+src[1+(dim-2)*dim].blue) )/4;
	
	dst[dim*dim-1].red=( (src[dim*dim-1].red+src[dim*dim-2].red) + (src[dim*dim-dim-1].red+src[dim*dim-dim-2].red) )/4;
	dst[dim*dim-1].green=( (src[dim*dim-1].green+src[dim*dim-2].green) + (src[dim*dim-dim-1].green+src[dim*dim-dim-2].green) )/4;
	dst[dim*dim-1].blue=( (src[dim*dim-1].blue+src[dim*dim-2].blue) + (src[dim*dim-dim-1].blue+src[dim*dim-dim-2].blue) )/4;
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

