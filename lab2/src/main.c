#include <stdio.h>
#include <stdlib.h>

const char H_zero = '0', H_nine = '9';

char * scan_char( size_t * len_link )
{
    size_t string_len = 1;

    char symbol;
    char * string = malloc( sizeof(char) );

    while ((symbol = getchar()) != '\n')
    {
        string[string_len - 1] = symbol;
        string_len++;
        string = realloc(string, sizeof(char) * string_len );
    }

    string[string_len - 1] = '\0';
    *len_link = string_len - 1;
    return string;
}


int correct_string( const char * string, size_t string_len)
{
    int arr [10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for( size_t index = 0; index < string_len; index++ )
    {
        if ( H_zero <= string [index ] && string[ index ] <= H_nine )
            arr[ string[ index ] - H_zero ]++;
        else return 0;
    }
    for( size_t index = 0; index < string_len; index++ )
    {
        if( arr[ index ] > 1)
        {
            return 0;
        }
    }
    return 1;
}

size_t find_first_necessary_index( const char * string, size_t string_len, size_t * index_link )
{
    size_t first_necessary_index = 0 , proof_of_start = 0;
    for ( size_t first_index = 0; first_index < string_len - 1; first_index++ )
    {
        if ( string[ first_index ] < string[ first_index + 1])
        {
            first_necessary_index = first_index;
            proof_of_start++;
        }
    }
    *index_link = first_necessary_index;
    return ( proof_of_start  ) ? 1 : 0;
}

size_t find_second_necessary_index( const char * string, size_t string_len, size_t * index_link, size_t first_necessary_index )
{
    size_t second_necessary_index = 0;
    for ( size_t second_index = first_necessary_index + 1; second_index < string_len; second_index++)
    {
        if ( string[ first_necessary_index ] < string[ second_index ])
        {
            second_necessary_index = second_index;
        }
    }
    * index_link = second_necessary_index;
    return 0;
}

void swap_symbols(char * string, const size_t string_len, const size_t first_necessary_index, const size_t second_necessary_index)
{
    char sub_char;

    sub_char = string[ first_necessary_index ];
    string[ first_necessary_index ] = string[ second_necessary_index ];
    string[ second_necessary_index ] = sub_char;

    size_t index = 0;
    size_t half_of_tail = ( string_len - ( first_necessary_index + 1 ) ) / 2;
    size_t front_index, back_index;

    for(; index < half_of_tail; index++  )
    {
        front_index = first_necessary_index + 1 + index;
        back_index = string_len - 1 - index;

        sub_char = string[ front_index ];
        string[ front_index ] = string[ back_index ];
        string[ back_index ] = sub_char;
    }
}

void print_permutation( char * string, size_t string_len, size_t permutation_quantity)
{
    for(size_t iteration = 0; iteration < permutation_quantity; iteration++)
    {
        size_t proof_of_start = 0;
        size_t first_necessary_index = 0, second_necessary_index = 0;

        proof_of_start = find_first_necessary_index( string, string_len, &first_necessary_index );
        find_second_necessary_index( string, string_len, &second_necessary_index, first_necessary_index );

        if ( proof_of_start == 0)
        {
            return;
        }
        swap_symbols(string, string_len, first_necessary_index, second_necessary_index );

        printf( "%s\n", string);
    }
}


int main(void) {

    size_t permutations_quantity,
            main_string_len,
            count_of_scan;

    char *main_string;

    main_string = scan_char(&main_string_len);
    count_of_scan = scanf("%llu", &permutations_quantity);

    if ( !(correct_string(main_string, main_string_len) ) )
    {
        printf("bad input\n");
        return 0;
    }
    if ( ( !(correct_string(main_string, main_string_len) ) ) || ( count_of_scan != 1 ) )
    {
        printf("bad input\n");
        return 0;
    }
    print_permutation( main_string, main_string_len, permutations_quantity);

    free( main_string);
    return 0;
}
