#include <stdio.h>
#include <stdlib.h>

const int K_chunk_len = 1024;
const int K_loop_not_start = -1;

typedef struct ST_vector
{
    char * pattern;
    int pattern_len;
    int permutations_quantity;
}ST_vector;

ST_vector create_vector( int memory_size )
{
    ST_vector our_vector = {
            .pattern = malloc( sizeof( char ) * memory_size ),
            .pattern_len = 0,
            .permutations_quantity = 0,
    };
    return our_vector;
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
    our_vector->pattern = NULL;
}

int scan_pattern( ST_vector * our_vector )
{
    FILE * thread_in = fopen( "in.txt", "r");
    if ( thread_in == NULL )
    {
        printf( "__LINE__ %d", __LINE__);
        return 1;
    }

    int iteration = 0;

    for ( char symbol; fread( &symbol, 1, sizeof(char), thread_in) != 0 && symbol!='\n'; iteration++)
    {
        if ( iteration>= K_chunk_len )
        {
            our_vector->pattern = realloc( our_vector->pattern, sizeof (char) * (iteration + K_chunk_len));
            if (our_vector == NULL)
            {
                printf( "__LINE__ %d", __LINE__);
                fclose( thread_in);
                return 1;
            }
            our_vector->pattern[iteration] = symbol;
        }
        else
        {
            our_vector->pattern[iteration] = symbol;
        }
    }
    our_vector -> pattern[iteration] = '\0';
    our_vector -> pattern_len = iteration;

    if ( 1 != fscanf(thread_in, "%d", &our_vector->permutations_quantity))
    {
        printf( "__LINE__ %d", __LINE__);
        fclose( thread_in);
        return 1;
    }
    fclose( thread_in);
    return 0;
}

int is_string_correct( const ST_vector * our_vector )
{
    int arr [10] = {0};

    for(int index = 0; index < our_vector->pattern_len ; index++ )
    {
        if ( '0' <= our_vector->pattern[index ] && our_vector->pattern[ index ] <= '9' )
        {
            if ( arr[our_vector->pattern[index] - '0' ] >= 1 )
            {
                return 0;
            }
            arr[our_vector->pattern[index] - '0']++;
        }
        else
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

void swap_some_symbols(ST_vector * our_vector, const int first_necessary_index, const int second_necessary_index)
{
    swap( &our_vector->pattern[ first_necessary_index ], &our_vector->pattern[ second_necessary_index ] );

    for(int front_index = first_necessary_index + 1, back_index = our_vector->pattern_len - 1; front_index < back_index; front_index++, back_index--  )
    {
        swap(&our_vector->pattern[ front_index ], &our_vector->pattern[ back_index ]);
    }
}

void print_permutation(ST_vector * vector, FILE * thread_out )
{
    for(int iteration = 0; iteration < vector->permutations_quantity; iteration++)
    {
        int first_necessary_index = find_first_necessary_index(vector );

        if ( first_necessary_index != K_loop_not_start )
        {
            int second_necessary_index = find_second_necessary_index(vector, first_necessary_index );

            swap_some_symbols(vector, first_necessary_index, second_necessary_index );

            fprintf( thread_out,"%s\n", vector->pattern);
        }
        else
        {
            return;
        }
    }
}

int main(void)
{
    ST_vector vector = create_vector(K_chunk_len );
    if ( vector.pattern == NULL)
    {
        printf( "__LINE__ %d", __LINE__);
        return 0;
    }

    if ( scan_pattern(&vector) )
    {
        destroy_vector(&vector );
        return 0;
    }

    FILE * thread_out = fopen( "out.txt", "w");
    if ( thread_out == NULL)
    {
        printf( "__LINE__ %d", __LINE__);
        destroy_vector( &vector);
        return 0;
    }

    if ( !is_string_correct( &vector ) )
    {
        fprintf( thread_out, "bad input");
        fclose( thread_out);
        destroy_vector(&vector );
        return 0;
    }

    print_permutation(&vector, thread_out);

    fclose( thread_out);
    destroy_vector(&vector );

    return 0;
}
