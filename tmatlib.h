#ifndef __TMAT_LIB__
#define __TMAT_LIB__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <json-c/json.h>
#ifndef __bool__
#define __bool__
	typedef enum bool{false,true} bool;
#endif
typedef bool bool;

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
/* we define our matrix as a structure
 * the structure contains information and the data for the matrix
 * rows = number of rows
 * cols = number of columns
 * data is a pointer to double , it is a single dimentional array,
 * which contains 2d matrix in 1d array
 */ 
typedef struct matrix{
	int rows; // number of rows
	int cols; // number of coulmns
	double *data; // pointer to data array
}matrix;
/* create_matrix() creates the matrix with all data set to zeros
 * input number of rows(rows) number of columns
 * returns a pointer to created matrix on success else NULL
 */
matrix *create_matrix(int rows, int columns)
{
	if(rows <= 0 || columns <=0)
	{
		printf("Error: Invalid rows or columns value\n");
		return (NULL);
	}
	matrix *m = (matrix*)malloc(sizeof(matrix));
	if(m == NULL)
	{
		perror("unable to allocate memory for matrix frame");
		return(NULL);
	}
	// success
	//allocate memory  for data
	m->data = (double *)malloc(rows*columns*sizeof(double));
	if (m->data == NULL)
	{
		perror("unable to allocate memory for matrix data");
		free(m);
		return(NULL);
	}
	//success
	m->rows =rows;
	m->cols= columns;
	bzero(m->data,rows*columns*sizeof(double));
	return(m);
}
/* is_valid() check if matrix is valid
 * input pointer to matrix
 * output boolean
 * validates if memory is allocated for data and the structure,
 * and row and column number are non negative
 */
bool is_valid(matrix *m)
{
	if(!m || !m->data)
		return(false);
	if(m->rows <= 0 || m->cols <=0)
		return(false);
	return(true);
}
/* matrix_from_file() constructs a matrix from a file
 * input file pointer
 * formated as follows
 * firstline number of rows as integer number of column as integer
 * data are stored from next line separated by tab "\t"
 * output created matrix on success, else NULL
 */
matrix *matrix_from_file(FILE *f)
{
	unsigned int r=0,c=0;
	//read matrix dimention  
	fscanf(f,"%d",&r);
	fscanf(f,"%d",&c);
	//create matrix;
	matrix *res = create_matrix(r,c);
	// read elements and put
	for(int i=0; i<r*c; i++)
	{
		fscanf(f,"%lf\t",&res->data[i]);
	}
	return(res);
}
/* matrix_to_file
 * input pointer to matrix, file pointer
 * output on success 1, else -1
 */ 
int matrix_to_file(matrix *m, FILE *fp)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid Matrix\n");
		return(-1);
	}
	if(fp==NULL)
	{
		printf("Error: please provide a valid file pointer\n");
		return(-1);
	}
	fprintf(fp,"%d %d\n",m->rows,m->cols);
	/*for(int i=0; i< m->rows*m->cols; i++)
	{
		fprintf(fp,"%lf\t",m->data[i]);
	}*/
	for(int i = 0;i<m->rows;i++)
	{
		for(int j = 0; j<m->cols; j++)
		{
			fprintf(fp,"%-5.4f ",m->data[m->cols*i+j]);
			//if(j<c-1)
			//printf("\t");
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");
	return(1);
}
/* matrix_to_json() makes a json string out of a matrix structure
 * input pointer to matrix struct
 * output a string formated as json
 */
char *matrix_to_json(matrix *mat)
{
	/* json representation of 2-d matrix
	{
		rows: mat->rows,
		cols: mat->cols,
		mat:[x,y,z,x1,y1,z1,...]
	}
	* 1+8+2+8+2+6+2*rows*col-1+1
	*/
	if(!is_valid(mat))
	{
		printf("Error: Invalid Matrix\n");
		return(NULL);
	}
	int js_sz = 27+2*mat->cols*mat->rows*sizeof(double);
	char *js_str=(char *)malloc((size_t)js_sz);
	if(js_str == NULL)
	{
		printf("Error: insufficient memory\n");
		return(NULL);
	}
	bzero(js_str,js_sz);
	strcat(js_str,"{\n");
	strcat(js_str,"\"rows\":");
	char istr[10];
	sprintf(istr,"%d",mat->rows);
	strcat(js_str,istr);
	strcat(js_str,",\n");
	strcat(js_str,"\"cols\":");
	sprintf(istr,"%d",mat->cols);
	strcat(js_str,istr);
	strcat(js_str,",\n");
	strcat(js_str,"\"mat\":[");
	for(int i = 0; i<mat->rows*mat->cols; i++)
	{
		if(i>=mat->rows*mat->cols-1)
		{
			sprintf(istr,"%f",mat->data[i]);
		}
		else
			sprintf(istr,"%f,",mat->data[i]);
			
		strcat(js_str,istr);
		//printf("%s\n", js_str);
	}
	strcat(js_str,"]\n}");
	return(js_str);
} 
/* eye_matrix() create an identity matrix 
 * input dimension as integer
 * returns identity matrix of order dimension x dimension
 */
matrix *eye_matrix(int dimension)
{
	matrix *res = create_matrix(dimension,dimension);
	if(!res)
		return(NULL);
	for(int i=0;i<dimension;i++)
	{
		for(int j=0;j<dimension;j++)
		{
			if(i==j)
			{
				res->data[dimension*i+j]=1;
				break;
			}
		}
	}
	return (res);
}
/* set_cell() puts a value into matrix cell denoted by row and col
 * input pointer to matrix, row as integer, column as integer, value as double
 * returns nothing
 */ 
void set_cell(matrix *m,int row,int col,double val)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return;
	}
	if(row<m->rows && col<m->cols)
	{
		m->data[m->cols*row+col]=val;
	}
	else
	{
		printf("Error: dimension out of bound\n");
		return;
	}
}
/* get_cell() gets a cell value from matrix
 * input pointer to matrix, row as integer, col as integer
 * returns double value stored in the cell denoted by row and col on success.
 * else 123.456e10 on error
 */
double get_cell(matrix *m,int row,int col)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return(123.456e10);
	}
	if(row<m->rows && col<m->cols)
	{
		return (m->data[m->cols*row+col]);
	}
	else{
		printf("Error: dimension out of bound\n");
		return(123.456e10);
	}
}
/* display_matrix() displays a matrix in a tabular form"prety print"
 * input pointer to matrix
 * returns nothing
 * outputs matrix in tabular form onto stdout
 */
void display_matrix(matrix *m)
{
	if(!m)
	{
		printf("Error: Invalid or un initialized matrix\n");
		return;
	}
	int r = m->rows;
	int c = m->cols;
	for (int i = 0;i<r;i++)
	{
		printf("[ ");
		for(int j = 0; j<c; j++)
		{
			printf("%-5.4f ",m->data[c*i+j]);
			//if(j<c-1)
			//printf("\t");
		}
		printf("]\n");
	}
	printf("\n");
}
/* delete_matrix() removes matrix from memory
 * input pointer to matrix
 * returns nothing
 */ 
void delete_matrix(matrix *m)
{
	if(m!=NULL)
	{
		if(m->data != NULL)
			free(m->data);
		free(m);
	}
	else
		printf("Warning: marix is already deleted or not initialized\n");
}
/* is_equal() check if 2 matrices are equal i.e element by element both matrices 
 * are same
 * input pointer to 2 matrices
 * returns booleaan true if same else false
 */
bool is_equal(matrix *m, matrix *m1)
{
	if(!is_valid(m) || !is_valid(m1))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return(false);
	}
	bool ret_val=true;
	if(m->rows == m1->rows && m->cols == m1->cols)
	{
		for(int i = 0; i< m->rows*m->cols; i++){
			if(m->data[i]!=m1->data[i])
			{
				ret_val=false;
				break;
			}
		}
	}
	else
	{
		ret_val = false;
	}
	return(ret_val);
}
/* is_square() checks if it is a square matrix
 * input pointer to  a matrix
 * returns true if rows =  columns else false
 */
bool is_square(matrix *m)
{
	return(m->rows==m->cols);
}
/* adds 2 matrices and return resultant matrix
 * input pointer to first matrix and second matrix
 * output pointer to summed matrix
 */ 
matrix *add_matrix(matrix *m1,matrix *m2)
{
	if(!is_valid(m1) || !is_valid(m1))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return(NULL);
	}
	if(m1->rows != m2->rows || m1->cols != m2->cols)
	{
		printf("Error: Order of input matrices are different\n");
		return(NULL);
	}
	matrix *res = create_matrix(m1->rows,m1->cols);
	if(!res)
		return(NULL);
	for(int i=0; i<m1->rows*m1->cols; i++)
	{
		res->data[i] = m1->data[i]+m2->data[i];
	}
	return(res);
}
/* subtracts first matrix from second return resultant matrix
 * input pointer to operand matrices
 * output pointer to result matrix
 */
matrix *subtract_matrix(matrix *m1,matrix *m2)
{
	if(!is_valid(m1) || !is_valid(m1))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return(NULL);
	}
	if(m1->rows != m2->rows || m1->cols != m2->cols)
	{
		printf("Error: Order of input matrices are different\n");
		return(NULL);
	}
	matrix *res = create_matrix(m1->rows,m1->cols);
	if(!res)
		return(NULL);
	for(int i=0; i<m1->rows*m1->cols; i++)
	{
		res->data[i] = m1->data[i]-m2->data[i];
	}
	return(res);
}
/* adds r1 and r2 and put in r1 (r1=r1+r2)
 * input pointer to matrix, row numbers to be added
 */
void add_rows(matrix *m, int r1, int r2)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return;
	}
	if(!(r1<m->rows) || !(r2<m->rows))
	{
		printf("Error: Invalid rows\n");
		return;
	}
	for( int c=0; c<m->cols; c++)
	{
		m->data[m->cols*r1+c] += m->data[m->cols*r2+c];
	} 
}
/* multiply all elements in a row r with factor
 * input pointer to matrix, row number, factor
 */
void row_mult_scalar(matrix *m, int r, double factor)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return;
	}
	if(!(r<m->rows))
	{
		printf("Error: Invalid rows\n");
		return;
	}
	for(int c = 0; c<m->cols; c++)
	{
		m->data[m->cols*r+c] *= factor;
	}
}
/* multiply a matrix by a scalar quantity
 * input scalar , pointer to matrix
 * output pointer to result matrix
 */  
matrix *mat_mult_scalar(double scl, matrix *m)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid or un initialized matrix\n");
		return(NULL);
	}
	for(int i = 0; i< m->rows * m->cols;i++)
	{
		m->data[i] *= scl;
	}
	return (m);
}
/* multiply 2 matrices
 * input pointer to 2 matrices
 * output pointer to result matrix
 */
matrix *mat_mult(matrix *m1, matrix *m2)
{
	if(!is_valid(m1) || !is_valid(m2))
	{
		printf("Error: invalid matrices\n");
		return(NULL);
	}
	if(m1->cols != m2->rows)
	{
		printf("Error: number of colulmns of first must be number of rows in second matrix\n");
		return(NULL);
	}
	matrix *res = create_matrix(m1->rows,m2->cols);
	if(res == NULL)
		return(NULL);
	
	int r = m1->rows;
	int c = m2->cols;
	for(int i=0; i<r; i++)
	{
		for(int j=0; j<c; j++)
		{
			for( int k =0; k<m1->cols; k++)
			{
				res->data[res->cols*i+j] += m1->data[m1->cols*i+k]*m2->data[m2->cols*k+j]; 
			}
		}
	}
	return (res);
}
/* returns the clone
 * input pointer to matrix to be cloned
 * output pointer to clone matrix
 */ 
matrix *copy_matrix(matrix *m)
{
	matrix *clone=create_matrix(m->rows,m->cols);
	for(int i=0; i<clone->rows*clone->cols; i++)
	{
		clone->data[i] = m->data[i];
	}
	return(clone);
}
/* transpose a matrix
 * input pointer to a matrix
 * output pointer to transposed matrix
 */ 
matrix *transpose(matrix *m)
{
	if(!is_valid(m))
	{
		printf("Error: invalid matrix\n");
		return(NULL);
	}
	matrix *res = create_matrix(m->cols,m->rows);
	if(is_valid(res))
	{
		for(int r=0; r<m->rows; r++)
		{
			for(int c=0; c<m->cols; c++)
			{
				res->data[res->cols*c+r]=m->data[m->cols*r+c];
			}
		}
		return(res);
	}
	printf("Error: invalid matrix\n");
	return(NULL);
}
/* swap 2 rows of matrix
 * input 2 row numbers which are to be swapped, and a pointer to matrix
 */ 
void swap_rows(int r1, int r2, matrix *m)
{
	if(!is_valid(m))
	{
		printf("Error: invalid matrix\n");
		return;
	}
	if(r1<0 || r1>m->rows || r2<0 || r2>m->rows || r1==r2)
		return; // do nothing
	int r1r,r2r;
	int i;
	int temp;
	for(i=0;i<m->cols;i++)
	{
		r1r = r1 * m->cols + i;
		r2r = r2 * m->cols + i;
		temp = m->data[r1r];
		m->data[r1r] = m->data[r2r];
		m->data[r2r] = temp;
	}
}
/* swap 2 cols of a matrix
 * input 2 column numbers to be swaped and pointer to matrix
 */ 
void swap_cols(int c1, int c2, matrix *m)
{
	if(c1 < 0 || c1>m->cols || c2<0 || c2>m->cols || c1 == c2)
		return;
	int c1r,c2r;
	int i;
	int temp;
	for(i = 0; i < m->rows; i++)
	{
		c1r = c1 + m->cols * i;
		c2r = c2 + m->cols * i;
		temp = m->data[c1r];
		m->data[c1r] = m->data[c2r];
		m->data[c2r] = temp;
	}
}
/* reduce row1 by row2 * factor (r2 = r2-r1*fact)
 * input pointer to matrix, row numbers and factor
 */ 
void reduce_row(matrix *m, int r1, int r2,double fact)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid matrix\n");
		return;
	}
	if(!(r1<m->rows) || !(r2<m->cols))
	{
		printf("Error: Invaalid rows\n");
		return;
	}
	for(int c = 0; c<m->cols; c++)
	{
		m->data[m->cols*r2+c] -= m->data[m->cols*r1+c]*fact;
	}
}
/* determinant of a matrix
 * input pointer to matrix
 * output double value of equivalent determinant
 */ 
double determinant(matrix* m)
{
	double det=1.0;
	if(!is_square(m))
	{
		printf("Error: Not a square matrix\n");
		return(0.0);
	}
	// destructive hence copy m first
	matrix *clone = copy_matrix(m);
	if(!clone)
		return(0.0);
	for(int r = 0; r<clone->rows; r++)
	{
		for(int i = r+1; i<clone->rows; i++)
		{
			if(clone->data[clone->cols*r+r]==0)
				continue;
			double fact = clone->data[clone->cols*i+r]/clone->data[clone->cols*r+r];
			reduce_row(clone,r,i,fact);
			//display_matrix(clone);
		}
	}
	//display_matrix(clone);
	// lower triangle
	for(int i =0; i<clone->cols; i++)
	{
		det *= clone->data[clone->cols*i+i];
	}
	delete_matrix(clone);
	return (det);
}
/* Gaussean reduction
 * input pointer to matrix to be reduced
 * output reduced matrix a upper triangle matrix
 */ 
matrix *gauss_reduction(matrix *m)
{
	if(!is_valid(m))
	{
		printf("Error: Invalid matrix\n");
		return(NULL);
	}
	matrix *clone = copy_matrix(m);
	// try to make lower triangle 0
	for(int r = 0; r<clone->rows; r++)
	{
		//check rows bellow and reduce
		for(int i=r+1; i<clone->rows; i++)
		{
			if(get_cell(clone,r,r)==0.0)
				continue;
			double fact = clone->data[clone->cols*i+r]/clone->data[clone->cols*r+r];
			reduce_row(clone,r,i,fact);
		}
	}
	return(clone);
}
/* determinant afeter gauss reduction 
 * input pointer to matrix
 * output double value of determinant after reduction
 */ 
double determinant_g(matrix *m)
{
	matrix *temp = gauss_reduction(m);
	double val=1.0;
	for(int r=0; r<temp->rows; r++)
	{
		val *= temp->data[temp->cols*r+r]; 
	}
	delete_matrix(temp);
	return(val);
}
/* Inverse of matrix
 * input pointer to matrix whose inverse is intended
 * output pointer to inverted matrix
 */ 
matrix *inverse_gj(matrix *m)
{
	if(!is_square(m))
	{
		printf("Error: need a squaare matrix\n");
		return(NULL);
	}
	// apply gauss_reduction first
	matrix *inv = eye_matrix(m->rows);
	matrix *clone = copy_matrix(m);
	// try to make lower triangle 0
	for(int r = 0; r<clone->rows; r++)
	{
		//check rows bellow and reduce
		for(int i=r+1; i<clone->rows; i++)
		{
			if(get_cell(clone,r,r)==0.0)
				continue;
			double fact = clone->data[clone->cols*i+r]/clone->data[clone->cols*r+r];
			reduce_row(clone,r,i,fact);
			reduce_row(inv,r,i,fact);
		}
	}
	//try upper triangular
	for(int r = clone->rows-1; r>=0; r--)
	{
		for(int i = r-1; i>=0; i--)
		{
			if(get_cell(clone,r,r)==0.0)
				continue;
			double fact = clone->data[clone->cols*i+r]/clone->data[clone->cols*r+r];
			reduce_row(clone,r,i,fact);
			reduce_row(inv,r,i,fact);
		}
	}
	display_matrix(clone);
	// normalize
	for( int j = 0; j<clone->cols; j++)
	{
		if(get_cell(clone,j,j)==0)
			continue;
		double mult= 1.0/get_cell(clone,j,j);
		row_mult_scalar(clone,j,mult);
		row_mult_scalar(inv,j,mult);
	}
	display_matrix(clone);
	return(inv);
}
// rank of matrix
// A(mxn) is a matrix with distinct m rows and n columns
// row rank of A is <= m
// column rank of is <=n
// rank of A <= min(m,n)
/* rank_of_matrix
 * input pointer to matrix
 * output rank
 */
int rank_of_matrix(matrix *m)
{
	int num_z=0, rank=0;
	if(!m)
		return -1;
	matrix *temp = gauss_reduction(m);
	// check for 0 rows
	for(int i = 0; i<temp->rows;i++)
	{
		for(int j =0; j<temp->cols; j++)
		{
			if(get_cell(temp,i,j)!=0)
				break;
			num_z++;
		}
		if(num_z == temp->cols) 
			rank--;
	}
	rank += MIN(m->rows,m->cols);
	return(rank);
}
/* get_row
 * input pointer to matrix, row number
 * output pointer to a single row matrix extracted from input matrix
 */ 
matrix *get_row(matrix *m, int r)
{
	if(!is_valid(m))
		return(NULL);
	if(r>=m->rows || r<0)
		return(NULL);
	matrix *res = create_matrix(1,m->cols);
	if(!res)
		return(NULL);
	for(int i =0; i<res->cols;i++)
	{
		res->data[res->cols*0+i]=m->data[m->cols*r+i];
	}
	return(res);
}
/* get_col gets a column matrix/vector from input
 * input pointer to matrix, column number
 * output pointer to a single column matrix
 */ 
matrix *get_col(matrix *m, int c)
{
	if(!is_valid(m))
		return(NULL);
	if(c<0 || c>=m->cols)
		return(NULL);
	matrix *res = create_matrix(m->rows,1);
	if(!res)
		return(NULL);
	for(int i= 0; i<res->rows; i++)
	{
		res->data[res->cols*i+0]= m->data[m->cols*i+c];
	}
	return(res);
}
#endif /* __TMAT_LIB__ */
