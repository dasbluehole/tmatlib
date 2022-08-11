/**************************************************
 tmalib usage example
 this program shows rank of matrix, 
 scalar multiplication
**************************************************/

#include "../tmatlib.h"

int main(int argc, char *argv[])
{
	matrix *m;
	
	m=create_matrix(3,3);
	/*1 2 -1 
      3 4 0 
      -1 0 -2 
    */
	set_cell(m,0,0,1);
	set_cell(m,0,1,2);
	set_cell(m,0,2,-1);
	
	set_cell(m,1,0,3);
	set_cell(m,1,1,4);
	set_cell(m,1,2,0);
	
	set_cell(m,2,0,-1);
	set_cell(m,2,1,0);
	set_cell(m,2,2,-2);
	
	printf("rank of matrix\n");
	display_matrix(m);
	printf("===============\n");
	printf("Rank = %d\n",rank_of_matrix(m));
	
	printf("multiplying by scalar\n");
	m = mat_mult_scalar(10.0,m);
	display_matrix(m);
	printf("===============\n");
	
	return(0);
}
	
