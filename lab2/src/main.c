#include <stdio.h>
#include <stdlib.h>

const int K_loop_not_start = -1;

typedef struct ST_vector
{
    char * pattern;
    int pattern_len;
}ST_vector;

ST_vector create_vector(void)
{
    ST_vector our_vector = {
            .pattern = NULL,
            .pattern_len = 0,
    };
    return our_vector;
}

void other_error( int line)
{
    printf( "line: %d", line);
    exit( EXIT_FAILURE);
}

void destroy_vector( ST_vector * our_vector)
{
    free(our_vector->pattern);
}

void scan_pattern( ST_vector * our_vector )
{
    char symbol;
    for( int index = 0 ; ; index++)
    {
        if ( 1 != fread(&symbol, sizeof(char), 1, stdin ) ){
            other_error( __LINE__);
        }
        if ( symbol != '\n' )
        {
            our_vector->pattern_len++;
            our_vector->pattern = realloc( our_vector->pattern, our_vector->pattern_len);
            if (our_vector -> pattern == NULL)
                other_error(__LINE__);
            our_vector->pattern[index] = symbol;
        }
        else
        {
            our_vector->pattern = realloc( our_vector->pattern, our_vector->pattern_len+1 );
            if (our_vector -> pattern == NULL)
                other_error(__LINE__);
            our_vector->pattern[ index ] = '\0';
            return;
        }
    }
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
    char sub_char;

    sub_char = our_vector->pattern[ first_necessary_index ];
    our_vector->pattern[ first_necessary_index ] = our_vector->pattern[ second_necessary_index ];
    our_vector->pattern[ second_necessary_index ] = sub_char;

    int half_of_tail = ( our_vector->pattern_len - ( first_necessary_index + 1 ) ) / 2;

    for(int index = 0; index < half_of_tail; index++  )
    {
        int front_index = first_necessary_index + 1 + index;
        int back_index = our_vector->pattern_len - 1 - index;

        sub_char = our_vector->pattern[ front_index ];
        our_vector->pattern[ front_index ] = our_vector->pattern[ back_index ];
        our_vector->pattern[ back_index ] = sub_char;
    }
}

void print_permutation(ST_vector * our_vector , int permutation_quantity )
{
    for(int iteration = 0; iteration < permutation_quantity; iteration++)
    {
        int first_necessary_index = find_first_necessary_index(our_vector );

        if ( first_necessary_index == K_loop_not_start )
        {
            return;
        }
        else
        {
            int second_necessary_index = find_second_necessary_index(our_vector, first_necessary_index );

            swap_symbols(our_vector, first_necessary_index, second_necessary_index );

            printf("%s\n", our_vector->pattern);
        }
    }
}

void finish_with_bad_input(ST_vector * our_vector )
{
    destroy_vector(our_vector );
    printf("bad input\n" );
    exit( EXIT_SUCCESS);
}


void are_all_condition_complied(ST_vector * our_vector, int count_of_scan)
{
    if ( ( count_of_scan != 1 ) || ( !is_string_correct(our_vector ) ) )
        finish_with_bad_input(our_vector );
}

int main(void)
{
    int permutations_quantity, count_of_scan;
    ST_vector vector = create_vector();
    scan_pattern(&vector );
    count_of_scan = scanf("%d", &permutations_quantity);

    are_all_condition_complied(&vector, count_of_scan );

    print_permutation(&vector, permutations_quantity);
    destroy_vector(&vector );

    return EXIT_SUCCESS;
}
