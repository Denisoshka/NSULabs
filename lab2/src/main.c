#include <stdio.h>
#include <stdlib.h>

typedef struct ARRAY_vector{
    int * array;
    int array_len;
}ARRAY_vector;

ARRAY_vector create_array_vector(void)
{
    ARRAY_vector vector ={
            .array = NULL,
            .array_len = 0,
    };
    return vector;
}

void other_error( const char * func, int line)
{
    printf( "func: %s   line: %d", func, line);
    exit( EXIT_FAILURE);
}

void prepare_vector( ARRAY_vector * vector)
{
    FILE * thread_in = fopen( "in.txt", "r");
//    FILE * thread_in = fopen( "C:\\Users\\dinis\\Desktop\\template-lab0\\lab3-0\\test\\in.txt", "r");
    if (thread_in == NULL)
    {
    other_error( __FUNCTION__ , __LINE__);
    }

    if ( fscanf(thread_in,"%d", &vector->array_len) != 1 )
    {
        other_error( __FUNCTION__ , __LINE__);
    }

    vector -> array = malloc( vector -> array_len * sizeof(int));
    if (vector->array == NULL)
    {
        other_error(__FUNCTION__ , __LINE__);
    }

    for( int index = 0; index < vector->array_len; index++)
    {
        if ( fscanf(thread_in, "%d", &vector->array[index] ) != 1 )
        {
            other_error((char *)__FUNCTION__ , __LINE__);
        }
    }
    fclose(thread_in);
}

void make_max_heap( ARRAY_vector * vector, int array_len, int root_index )
{
    int max_element_index = root_index,
        left_index = 2 * root_index + 1,
        right_index = 2 * root_index + 2;

    if ( left_index < array_len && vector->array[left_index] > vector->array[max_element_index] )
        max_element_index = left_index;
    if (  right_index < array_len && vector->array[right_index] > vector->array[max_element_index])
        max_element_index = right_index;

    if (max_element_index != root_index)
    {
        int sub_element = vector->array[max_element_index];
        vector->array[max_element_index] = vector->array[root_index];
        vector->array[root_index] = sub_element;

        make_max_heap(vector, array_len, max_element_index);
    }
}

void heap_sort( ARRAY_vector * vector )
{
    int array_len = vector -> array_len;
    for( int index = array_len / 2 + 1; index >= 0; index--)
        make_max_heap( vector, array_len, index);

    for( int index = array_len - 1; index >= 0; index--)
    {
        int sub_element = vector->array[0];
        vector->array[0] = vector->array[index];
        vector->array[index] = sub_element;

        make_max_heap(vector, index, 0);
    }
}

void print_array(ARRAY_vector * vector)
{
    FILE * thread_out = fopen( "out.txt", "w");
//    FILE * thread_out = fopen( "C:\\Users\\dinis\\CLionProjects\\lab3-0\\out.txt", "w");
    for( int index = 0; index < vector->array_len; index++)
    {
        if (fprintf(thread_out, "%d ", vector->array[index]) == -1)
        {
            other_error(__FUNCTION__ , __LINE__);
        }
    }
    fclose( thread_out);
}

void destroy_vector(ARRAY_vector * vector)
{
    free( vector->array);
}

int main()
{
    ARRAY_vector vector = create_array_vector();
    prepare_vector( &vector);
    heap_sort( &vector);
    print_array( &vector);
    destroy_vector( &vector);

    return EXIT_SUCCESS;
}
