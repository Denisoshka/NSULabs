#include <stdio.h>
#include <stdlib.h>

typedef struct ST_vector
{
    int * array;
    int array_len;
} ST_vector;

ST_vector create_vector( void )
{
    ST_vector vector = {
            .array = NULL,
            .array_len = 0,
    };
    return vector;
}

void exit_with_error(void)
{
    exit( EXIT_FAILURE );
}

void destroy_vector( ST_vector * our_vector )
{
    free( our_vector -> array );
}

void exit_with_success(ST_vector *our_vector)
{
    destroy_vector( our_vector );
    exit( EXIT_SUCCESS);
}

void fill_vector(ST_vector * our_vector)
{
    if ( scanf( "%d", &our_vector->array_len ) != 1 )
    {
        exit_with_error();
    }
    our_vector -> array = malloc( sizeof(int) * our_vector->array_len );
    if ( our_vector->array == NULL)
    {
        exit_with_error();
    }
    for (int index = 0; index < our_vector->array_len; index++)
    {
        if( scanf( "%d", &our_vector->array[ index ] ) != 1)
        {
            exit_with_error();
        }
    }
}

void quick_sort(ST_vector * our_vector, const int left_edge, const int right_edge ) {

    int first_index = left_edge, second_index = right_edge;

    int patrition_element = our_vector->array[(left_edge + right_edge) / 2];

    while (first_index <= second_index)
    {
        for( ;our_vector->array[first_index] < patrition_element; first_index++);
        for( ;our_vector->array[second_index] > patrition_element; second_index--);

        if (first_index <= second_index)
        {
            int sub_element = our_vector->array[first_index];
            our_vector->array[first_index] = our_vector->array[second_index];
            our_vector->array[second_index] = sub_element;

            first_index++;
            second_index--;
        }
    }
    if (left_edge < second_index)
    {
        quick_sort(our_vector, left_edge, second_index);
    }
    if (first_index < right_edge)
    {
        quick_sort(our_vector, first_index, right_edge );
    }

}

void print_array(ST_vector * our_vector)
{
    for (int index = 0; index < our_vector->array_len; index++)
    {
        printf( "%d ", our_vector->array[index]);
    }
}

int main(void )
{
    ST_vector vector = create_vector();
    fill_vector( &vector );

    quick_sort( &vector, 0, vector.array_len - 1 );

    print_array( &vector);

    exit_with_success( &vector );
}
