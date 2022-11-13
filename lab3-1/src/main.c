#include <stdio.h>
#include <stdlib.h>

void print_error_on_line(int line, int * flag )
{
    printf( "__LINE__ %d", line);
    * flag = 1;
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

void prepare_vector( ARRAY_vector * vector, int * flag)
{
    FILE * thread_in = fopen( "in.txt", "r");
    if ( thread_in == NULL)
    {
        print_error_on_line(__LINE__, flag);
        return;
    }

    if ( fscanf(thread_in,"%d", &vector->array_len) != 1 )
    {
        print_error_on_line(__LINE__, flag);
        fclose( thread_in);
        return;
    }

    vector -> array = malloc( vector->array_len * sizeof( int ) );
    if ( vector->array == NULL)
    {
        print_error_on_line(__LINE__, flag);
        fclose( thread_in);
        return;
    }

    for (int index = 0; index < vector->array_len; index++)
    {
        if( fscanf( thread_in,"%d", &vector->array[index]) != 1)
        {
            print_error_on_line(__LINE__, flag);
            fclose( thread_in);
            return;
        }
    }
    fclose(thread_in);
}

void swap_elements( int * first_element, int * second_element)
{
    int sub_element = * first_element;
    * first_element = * second_element;
    * second_element = sub_element;
}

void quick_sort(ARRAY_vector * vector, const int begin_index, const int end_index)
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
    FILE * thread_out = fopen("out.txt", "w");
    for (int index = 0; index < vector->array_len; index++)
    {
        if( -1 == fprintf(thread_out, "%d ", vector->array[index]))
        {
            printf( "__LINE__ %d", __LINE__);
            break;
        }
    }
    fclose( thread_out );
}

int main( void )
{
    int flag = 0;
    
    ARRAY_vector vector = create_vector();
    prepare_vector( &vector, &flag);
    if (flag != 0)
    {
        destroy_vector( &vector);
        return 0;
    }
    
    if (vector.array_len > 1)
    {
        quick_sort( &vector, 0, vector.array_len - 1);
    }

    print_array( &vector);

    destroy_vector( &vector);
    
    return 0;
}
