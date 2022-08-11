/**************************************************
 tmalib usage example
 this program shows how to create, display and 
 delete the matrix
**************************************************/
#include "../tmatlib.h"
int main(int argc, char *argv[])
{
    // create a matrix of 3x3 dimension
    matrix *mm = create_matrix(3,3);
    if(mm ==NULL)
    {
	fprintf(stderr,"Unable to create matrix");
	exit(EXIT_FAILURE);
    }
    printf("matrix created at  %X\n",(unsigned)mm);
    // use set_cell to populate the matrix
    set_cell(mm,0,0,0.0);
    set_cell(mm,0,1,0.1);
    set_cell(mm,0,2,0.2);
    set_cell(mm,1,0,1.0);
    set_cell(mm,1,1,1.1);
    set_cell(mm,1,2,1.2);
    set_cell(mm,2,0,2.0);
    set_cell(mm,2,1,2.1);
    set_cell(mm,2,2,2.2);
    //display it using display_matrix()
    display_matrix(mm);
    // done with it delete it from memory
    delete_matrix(mm);
    return(0);
}        
