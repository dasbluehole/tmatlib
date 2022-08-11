/**************************************************
 tmalib usage example
 this program shows rank of matrix, 
 scalar multiplication
**************************************************/

#include "../tmatlib.h"

int main(int argc, char *argv[])
{
	matrix *m, *m1, *m2;
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
	set_cell(m,2,2,0);
	//---------------------
	m1 = create_matrix(3,3);
	set_cell(m1,0,0,-1);
	set_cell(m1,0,1,2);
	set_cell(m1,0,2,1);
	
	set_cell(m1,1,0,-3);
	set_cell(m1,1,1,4);
	set_cell(m1,1,2,0);
	
	set_cell(m1,2,0,-1);
	set_cell(m1,2,1,0);
	set_cell(m1,2,2,-1);
	printf("addition of matrices\n");
	display_matrix(m);
	printf("===============\n");
	display_matrix(m1);
	printf("===============\n");
	m2= add_matrix(m,m1);
	display_matrix(m2);
	printf("===============\n");

	printf("subtraction of matrices\n");
	display_matrix(m);
	printf("===============\n");
	display_matrix(m1);
	printf("===============\n");
	m2= subtract_matrix(m,m1);
	display_matrix(m2);
	printf("===============\n");
	delete_matrix(m);
	delete_matrix(m1);
	delete_matrix(m2);
	printf("===============\n");
	
	return(0);
}
	
