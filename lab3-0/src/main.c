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

void destroy_vector(ARRAY_vector * vector)
{
    free( vector->array);
    vector->array = NULL;
}

void swap(int * first, int * second)
{
    int additional = * first;
    * first = * second;
    * second = additional;
}

void print_error_on_line(int line, int * flag)
{
    printf( "line: %d", line);
    * flag = 1;
}

void prepare_vector( ARRAY_vector * vector, int * flag)
{
    FILE * thread_in = fopen( "in.txt", "r");
    if (thread_in == NULL)
    {
        print_error_on_line(__LINE__, flag);
        return;
    }

    if ( fscanf(thread_in,"%d", &vector->array_len) != 1 )
    {
        print_error_on_line(__LINE__, flag );
        fclose(thread_in);
        return;
    }

    vector -> array = malloc( vector -> array_len * sizeof(int));
    if (vector->array == NULL)
    {
        print_error_on_line(__LINE__, flag );
        fclose(thread_in);
        return;
    }

    for( int index = 0; index < vector->array_len; index++)
    {
        if ( fscanf(thread_in, "%d", &vector->array[index] ) != 1 )
        {
            print_error_on_line(__LINE__, flag );
            fclose(thread_in);
            return;
        }
    }
    fclose(thread_in);
}

void make_max_heap( ARRAY_vector * vector, const int array_len, const int root_index )
{
    int max_element_index = root_index;
    const int left_index = 2 * root_index + 1,
              right_index = 2 * root_index + 2;

    if ( left_index < array_len && vector->array[left_index] > vector->array[max_element_index] )
    {
        max_element_index = left_index;
    }
    if (  right_index < array_len && vector->array[right_index] > vector->array[max_element_index])
    {
        max_element_index = right_index;
    }
    if (max_element_index != root_index)
    {
        swap(&vector->array[max_element_index], &vector->array[root_index]);

        make_max_heap(vector, array_len, max_element_index);
    }
}

void heap_sort( ARRAY_vector * vector )
{
    for( int index = vector -> array_len / 2 + 1; index >= 0; index--)
    {
        make_max_heap( vector, vector -> array_len, index);
    }

    for( int index = vector -> array_len - 1; index >= 0; index--)
    {
        swap(&vector->array[0], &vector->array[index]);

        make_max_heap(vector, index, 0);
    }
}

void print_array(const ARRAY_vector * vector, int * flag)
{
    FILE * thread_out = fopen( "out.txt", "w");
    for( int index = 0; index < vector->array_len; index++)
    {
        if (fprintf(thread_out, "%d ", vector->array[index]) == -1)
        {
            print_error_on_line(__LINE__, flag);
            break;
        }
    }
    fclose( thread_out);
}

int main(void)
{
    int flag = 0;
    ARRAY_vector vector = create_array_vector();
    prepare_vector( &vector, &flag);
    if (flag != 0)
    {
        destroy_vector( &vector);
        return 0;
    }

    if ( vector.array_len > 1)
    {
        heap_sort(&vector);
    }

    print_array( &vector, &flag);

    destroy_vector( &vector);

    return 0;
}
