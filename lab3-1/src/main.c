#include <stdio.h>
#include <stdlib.h>

void other_error(int line)
{
    printf( "line: %d", line);
    exit( EXIT_FAILURE);
}

typedef struct ARRAY_vector
{
    int * array;
    int array_len;
}ARRAY_vector;

ARRAY_vector create_vector(void)
{
    ARRAY_vector vector = {
            .array = NULL,
            .array_len = 0,
    };
    return vector;
}

void destroy_vector( ARRAY_vector * vector )
{
    free( vector->array );
}

void prepare_vector( ARRAY_vector * vector)
{

    if ( scanf("%d", &vector->array_len) != 1 )
    {
        other_error(__LINE__);
    }

    vector -> array = malloc( vector->array_len * sizeof( int ) );

    if ( vector->array == NULL)
    {
        other_error(__LINE__);
    }

    for (int index = 0; index < vector->array_len; index++)
    {
        if( scanf( "%d", &vector->array[index]) != 1)
        {
            other_error(__LINE__);
        }
    }
}

void swap_elements( int * first_element, int * second_element)
{
    int sub_element = * first_element;
    * first_element = * second_element;
    * second_element = sub_element;
}

void quick_sort(ARRAY_vector * vector, int begin_index, int end_index)
{
    int first_index = begin_index;
    int second_index = end_index;

    int patrition_element = vector->array[ (begin_index + end_index) / 2 ];

    for ( ; first_index < second_index; )
    {
        for ( ; ( first_index < vector->array_len ) && vector->array[first_index] < patrition_element; first_index++);
        for ( ; ( second_index >= 0 ) && patrition_element < vector->array[second_index] ; second_index--);

        if (first_index <= second_index)
        {
            swap_elements( &vector->array[first_index], &vector->array[second_index]);

            first_index++;
            second_index--;
        }
    }
    if ( begin_index < second_index )
    {
        quick_sort(vector, begin_index, second_index);
    }
    if ( first_index < end_index )
    {
        quick_sort(vector, first_index, end_index);
    }
}

void print_array( const ARRAY_vector * vector )
{
    for (int index = 0; index < vector->array_len; index++)
    {
        printf( "%d ", vector->array[index]);
    }
}

int main( void )
{
    ARRAY_vector vector = create_vector();
    prepare_vector( &vector);
    if (vector.array_len > 1)
    {
        quick_sort( &vector, 0, vector.array_len - 1);
    }
    print_array( &vector);
    destroy_vector( &vector);

    return 0;
}

