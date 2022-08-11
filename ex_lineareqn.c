/**************************************************
 tmalib usage example
 this program shows solving 
 simultaneus linear equations,
 matrix inversion, multiplication
**************************************************/

#include "../tmatlib.h"

int main(int argc,char *argv[])
{
	printf("simultaneus equation solver\n");
	printf(" x + y + z  =  6\n 2y + 5z = -4\n 2x +5y - z  = 27\n");
	matrix *coef=create_matrix(3,3);
	if(coef == NULL)
	{
		printf("No memory\n");
		return(-1);
	}
	set_cell(coef,0,0,1.0);
	set_cell(coef,0,1,1.0);
	set_cell(coef,0,2,1.0);
	set_cell(coef,1,0,0.0);
	set_cell(coef,1,1,2.0);
	set_cell(coef,1,2,5.0);
	set_cell(coef,2,0,2.0);
	set_cell(coef,2,1,5.0);
	set_cell(coef,2,2,-1.0);
	matrix *con = create_matrix(3,1);
	if(con ==NULL)
	{
		printf("no memory\n");
		return(-1);
	}
	set_cell(con,0,0,6.0);
	set_cell(con,1,0,-4.0);
	set_cell(con,2,0,27.0);
	matrix *icoef = inverse_gj(coef);
	matrix *res = mat_mult(icoef,con);
	display_matrix(res);
	delete_matrix(res);
	delete_matrix(icoef);
	delete_matrix(con);
	delete_matrix(coef);
	
	
	return(0);
}
