#include <stdio.h>
#include <stdlib.h>

typedef struct our_custom_array
{
    int * array;
    int array_len;
}our_custom_array;

our_custom_array create_custom_array(const int array_size )
{
    our_custom_array custom_array = {
            .array = malloc(sizeof( int ) * array_size ),
            .array_len = array_size,
    };
    return custom_array;
}

void destroy_custom_array(our_custom_array * custom_array )
{
    free(custom_array->array );
    custom_array->array = NULL;
}

int prepare_custom_array(our_custom_array * custom_array)
{
    int scanned_array_len;

    FILE * thread_in = fopen( "in.txt", "r");
    if ( thread_in == NULL)
    {
        printf( "__LINE__ %d", __LINE__);
        return 1;
    }

    if ( fscanf(thread_in,"%d", &scanned_array_len) != 1 )
    {
        printf( "__LINE__ %d", __LINE__);
        fclose( thread_in);
        return 1;
    }

    * custom_array = create_custom_array(scanned_array_len);
    if (custom_array->array == NULL)
    {
        printf( "__LINE__ %d", __LINE__);
        fclose( thread_in);
        return 1;
    }

    for (int index = 0; index < custom_array->array_len; index++)
    {
        if(fscanf( thread_in,"%d", &custom_array->array[index]) != 1)
        {
            printf( "__LINE__ %d", __LINE__);
            fclose( thread_in);
            return 1;
        }
    }
    fclose(thread_in);
    return 0;
}

void swap( int * first_element, int * second_element)
{
    int sub_element = * first_element;
    * first_element = * second_element;
    * second_element = sub_element;
}

void quick_sort(our_custom_array * custom_array, const int begin_index, const int end_index)
{
    int first_index = begin_index,
        second_index = end_index;
    const int patrition_element = custom_array->array[(begin_index + end_index) / 2 ];

    for ( ; first_index < second_index; )
    {
        for ( ; (first_index < custom_array->array_len ) && custom_array->array[first_index] < patrition_element; first_index++);
        for ( ; ( second_index >= 0 ) && patrition_element < custom_array->array[second_index] ; second_index--);

        if (first_index <= second_index)
        {
            swap(&custom_array->array[first_index], &custom_array->array[second_index]);

            first_index++;
            second_index--;
        }
    }
    if ( begin_index < second_index )
    {
        quick_sort(custom_array, begin_index, second_index);
    }
    if ( first_index < end_index )
    {
        quick_sort(custom_array, first_index, end_index);
    }
}

int print_array( const our_custom_array * vector )
{
    FILE * thread_out = fopen("out.txt", "w");
    if ( thread_out == NULL )
    {
        printf( "__LINE__ %d", __LINE__);
        return 1;
    }
    for (int index = 0; index < vector->array_len; index++)
    {
        if( fprintf(thread_out, "%d ", vector->array[index]) == -1  )
        {
            printf( "__LINE__ %d", __LINE__);
            fclose( thread_out );
            return 1;
        }
    }
    fclose( thread_out );
    return 0;
}

int main( void )
{
    our_custom_array vector;

    if ( prepare_custom_array(&vector) )
    {
        destroy_custom_array(&vector);
        return 0;
    }

    if (vector.array_len > 1)
    {
        quick_sort( &vector, 0, vector.array_len - 1);
    }

    print_array( &vector);

    destroy_custom_array(&vector);

    return 0;
}
