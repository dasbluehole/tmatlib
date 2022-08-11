/**************************************************
 tmalib usage example
 this program shows check equality of 2 matrices
**************************************************/
#include "../tmatlib.h"
int main(int argc, char *argv[])
{
	matrix *m,*m1;
	m = create_matrix(2,1);
	m1 = create_matrix(2,1);
	set_cell(m,0,0,1.0);
	set_cell(m,1,0,2.0);
	set_cell(m1,0,0,1.0);
	set_cell(m1,1,0,2.0);
	printf("Equality Check\n");
	if(is_equal(m,m1))
	{
		printf("matrices are same\n");
	}
	else
		printf("matrices are different\n");
	
	display_matrix(m);
	display_matrix(m1);
	return(0);
}
