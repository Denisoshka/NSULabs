#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int K_max_string_len = 1024;
const int K_loop_not_start = -1;
const int K_other_error = -3;

typedef struct ST_vector
{
    char * pattern;
    int pattern_len;
    int permutations_quantity;
}ST_vector;

ST_vector create_vector(void)
{
    ST_vector our_vector = {
            .pattern = NULL,
            .pattern_len = 0,
            .permutations_quantity = 0,
    };
    return our_vector;
}

void other_error( int line, int * flag, int error )
{
    printf( "line: %d", line);
    * flag = error;
}

void swap(char * first, char * second)
{
    char additional = * first;
    * first = * second;
    * second =  additional;
}

void destroy_vector( ST_vector * our_vector)
{
    free(our_vector->pattern);
}

void scan_pattern( ST_vector * our_vector, int * flag )
{
    FILE * thread_in = fopen( "C:\\Users\\dinis\\Desktop\\template-lab0\\lab2\\test\\in.txt", "r");
    if (thread_in == NULL)
    {
        other_error(__LINE__, flag,K_other_error);
        return;
    }

    our_vector->pattern = malloc( sizeof(char) * K_max_string_len);
    if (our_vector -> pattern == NULL)
    {
        fclose( thread_in);
        other_error(__LINE__, flag,K_other_error);
        return;
    }

    if ( 0 == fscanf(thread_in , "%1023[^\n]s", our_vector->pattern))
    {
        fclose( thread_in);
        other_error(__LINE__, flag,K_other_error);
        return;
    }

    our_vector -> pattern_len = (int)strlen(our_vector->pattern);

    if ( 1 != fscanf(thread_in, "%d", &our_vector->permutations_quantity))
    {
        fclose( thread_in);
        other_error(__LINE__, flag,K_other_error);
        return;
    }
    fclose( thread_in );
}

int is_string_correct( const ST_vector * our_vector )
{
    int arr [10] = {0};

    for(int index = 0; index < our_vector->pattern_len ; index++ )
    {
        if ( '0' <= our_vector->pattern[index ] && our_vector->pattern[ index ] <= '9' )
        {
            arr[our_vector->pattern[index] - '0']++;
        }
        else
        {
            return 0;
        }
    }
    for( int index = 0; index < our_vector->pattern_len; index++ )
    {
        if( arr[ index ] > 1)
        {
            return 0;
        }
    }
    return 1;
}

int find_first_necessary_index(const ST_vector * our_vector )
{
    int first_necessary_index = K_loop_not_start;

    for ( int first_index = 0; first_index < our_vector->pattern_len - 1; first_index++ )
    {
        if ( our_vector->pattern[ first_index ] < our_vector->pattern[ first_index + 1])
        {
            first_necessary_index = first_index;
        }
    }
    return first_necessary_index;
}

int find_second_necessary_index(const ST_vector * our_vector, const int first_necessary_index )
{
    int second_necessary_index = 0;

    for ( int second_index = first_necessary_index + 1; second_index < our_vector->pattern_len; second_index++)
    {
        if ( our_vector->pattern[ first_necessary_index ] < our_vector->pattern[ second_index ])
        {
            second_necessary_index = second_index;
        }
    }
    return second_necessary_index;
}

void swap_symbols(ST_vector * our_vector, const int first_necessary_index, const int second_necessary_index)
{
    swap( &our_vector->pattern[ first_necessary_index ], &our_vector->pattern[ second_necessary_index ] );

    int half_of_tail = ( our_vector->pattern_len - ( first_necessary_index + 1 ) ) / 2;

    for(int index = 0; index < half_of_tail; index++  )
    {
        int front_index = first_necessary_index + 1 + index,
            back_index = our_vector->pattern_len - 1 - index;

        swap(&our_vector->pattern[ front_index ], &our_vector->pattern[ back_index ]);
    }
}

void print_permutation(ST_vector * vector )
{
    for(int iteration = 0; iteration < vector->permutations_quantity; iteration++)
    {
        int first_necessary_index = find_first_necessary_index(vector );

        if ( first_necessary_index != K_loop_not_start )
        {
            int second_necessary_index = find_second_necessary_index(vector, first_necessary_index );

            swap_symbols(vector, first_necessary_index, second_necessary_index );

            printf("%s\n", vector->pattern);
        }
        else
        {
            return;
        }
    }
}

int main(void)
{

    int flag = 0;
    ST_vector vector = create_vector();

    scan_pattern(&vector, &flag );
    if ( flag != 0)
    {
        destroy_vector(&vector );
        return 0;
    }

    if ( !is_string_correct( &vector ) )
    {
        printf( "bad input");
        destroy_vector(&vector );
        return 0;
    }

    print_permutation(&vector);

    destroy_vector(&vector );
    return 0;
}
