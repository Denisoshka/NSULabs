#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int K_base_numbers[16] = {0, 1, 2, 3,
                                4, 5, 6, 7,
                                8, 9, 10, 11,
                                12, 13, 14, 15};
const int K_max_input_number_len = 13;
const int K_max_after_dot_numbers = 12;

typedef struct ST_vector
{
    char * char_number;
    int number_len;
    int dot_index;
    int fractional;
}ST_vector;

ST_vector make_vector(void)
{
    ST_vector our_vector = {
            .char_number = NULL,
            .dot_index = 0,
            .number_len = 0,
            .fractional = 0,
    };
    return our_vector;
}

const char * K_all_symbols = "0123456789abcdef";

void print_error_on_line( const int line, int * flag )
{
    printf( "line: %d", line);
    * flag = 1;
}

void scan_number(ST_vector * vector, FILE * thread_in, int * flag)
{
    if( fscanf(thread_in, "%13s", vector->char_number) != 1 )
    {
        print_error_on_line(__LINE__, flag);
        return;
    }

    vector -> number_len = (int)strlen( vector->char_number);
}

void destroy_vector( ST_vector * our_vector )
{
    free( our_vector->char_number);
    our_vector->char_number = NULL;
}

void make_lower_char(ST_vector * our_vector)
{
    for ( int index = 0; index < ( our_vector->number_len ) ; index++)
    {
        our_vector -> char_number[ index ] = (char)tolower( our_vector->char_number[ index ] );
    }
}

void get_dot_index( ST_vector * our_vector )
{
    int index=0;
    for( ; index < our_vector->number_len; index++ )
    {
        if ( our_vector->char_number[index] == '.' ){
            our_vector->dot_index = index;
            return;
        }
    }
    our_vector->dot_index = index;
}

int get_necessary_number( const char symbol )
{
    if ( ( '0' <= symbol) && (symbol <= '9' ) )
    {
        return K_base_numbers[ symbol - '0' ];
    }else
    {
        return K_base_numbers[ symbol - 'a' + 10];
    }
}

int is_symbol_less_base( const char symbol, const int K_base_from )
{
    int symbol_in_range_2_base_from = ( ( '0' <= symbol ) && ( symbol < '0' + K_base_from ) );
    int symbol_in_range_a_base_from = ( ( 'a' <= symbol ) && ( symbol < 'a' + K_base_from - 10 ) );
    int condition = ( symbol <= '9' ) ? symbol_in_range_2_base_from : symbol_in_range_a_base_from;

    return condition;
}

int is_valid_char(const ST_vector * vector, const int K_base_from )
{
    if ( vector->char_number[0] == '\0' )
    {
        return 0;
    }

    if ((vector -> dot_index == 0 ) || (vector -> dot_index == vector -> number_len - 1 ) )
    {
        return 0;
    }

    for(int index = 0 ; index < vector -> dot_index; index++ )
    {
        if ( !is_symbol_less_base(vector -> char_number[ index ] , K_base_from ) )
        {
            return 0;
        }
    }

    for(int index = vector -> dot_index + 1 ; index < vector -> number_len; index++ )
    {
        if ( !is_symbol_less_base(vector -> char_number[ index ], K_base_from ) )
        {
            return 0;
        }
    }

    return 1;
}

int is_valid_bases( const int K_base1, const int K_base2 )
{
    return ( 2 <= K_base1 && K_base1 <= 16 && 2 <= K_base2 && K_base2<= 16 );
}

void is_fractional( ST_vector * our_vector )
{
    for ( int index = our_vector -> dot_index + 1 ; index < our_vector->number_len; index++)
    {
        if ( our_vector-> char_number[ index ] != '0')
        {
            our_vector->fractional = 1;
            return;
        }
    }
    our_vector->fractional = 0;
}

long long int powlli( const long long int number, const int degree)
{
    long long int result = 1;
    for (int iteration = 0; iteration < degree; iteration++, result *= number);
    return result;
}

long long int convert_from_char_non_fractional_part(const ST_vector * our_vector, const int K_base_from)
{
    long long int non_fractional_result = 0;

    for ( int front_index = 0, additional_index = our_vector -> dot_index - 1; front_index < our_vector->dot_index; front_index++, additional_index-- )
    {
        non_fractional_result += get_necessary_number( our_vector->char_number[front_index] ) * powlli(K_base_from, additional_index ) ;
    }
    return non_fractional_result;
}

double convert_from_char_fractional_part( const ST_vector * our_vector, const int K_base_from)
{
    double fractional_result = 0;

    for( int index = our_vector->dot_index + 1, degree = -1; index < our_vector->number_len; index++, degree-- )
    {
        fractional_result += get_necessary_number( our_vector->char_number[index] ) * pow(K_base_from, degree);
    }
    return fractional_result;
}

void convert_to_char(ST_vector * vector, long long int non_fractional_part, double fractional_part, int non_fractional_part_len, const int K_base_to )
{
    int result_len = ( vector->fractional ) ? ( non_fractional_part_len + 1 + K_max_after_dot_numbers ) : non_fractional_part_len;

    vector->char_number = realloc(vector->char_number, (result_len + 1) * sizeof(char));
    if (vector->char_number == NULL )
    {
        printf( "__LINE__ %d", __LINE__);
        return;
    }

    for( int back_index = non_fractional_part_len - 1; back_index >= 0; back_index-- )
    {
        vector->char_number[back_index] = K_all_symbols[non_fractional_part % K_base_to];
        non_fractional_part /= K_base_to;
    }

    if ( !vector->fractional)
    {
        vector->char_number[ result_len ] = '\0';
        return;
    }

    vector->char_number[non_fractional_part_len] = '.';

    for ( int index = non_fractional_part_len + 1; index < result_len; index++ )
    {
        fractional_part *= K_base_to;
        vector->char_number[ index ] = K_all_symbols[ (int)fractional_part ];
        fractional_part -= (int)fractional_part;
    }

    vector->char_number[ result_len ] = '\0';
}

int get_non_fractional_part_len( const long long int non_fractional_result, int K_base_to)
{
    int non_fractional_result_len = 0;
    do{
        non_fractional_result_len++;
    }while( non_fractional_result >= powlli(K_base_to, non_fractional_result_len ) );

    return non_fractional_result_len;
}

void convert_number(ST_vector *vector, const int K_base_from, const int K_base_to )
{
    long long int non_fractional_part = convert_from_char_non_fractional_part(vector, K_base_from);
    double fractional_part = convert_from_char_fractional_part(vector, K_base_from );
    int non_fractional_part_len = get_non_fractional_part_len( non_fractional_part, K_base_to);

    convert_to_char(vector, non_fractional_part, fractional_part, non_fractional_part_len, K_base_to );
}

void print_converted_number( const ST_vector *vector)
{
    FILE * thread_out = fopen( "out.txt", "w");
    if ( thread_out == NULL )
    {
        printf( "__LINE__ %d", __LINE__);
        return;
    }
    fprintf(thread_out, "%s\n", vector->char_number );
    fclose( thread_out );
}

int main(void )
{
    int flag= 0, base_from, base_to;

    FILE * thread_in = fopen( "in.txt", "r");
    if( thread_in == NULL)
    {
        printf( "__LINE__ %d", __LINE__);
        return 0;
    }

    if( 2 != fscanf(thread_in,"%d%d", &base_from, &base_to) )
    {
        printf("bad input\n" );
        fclose( thread_in );
        return 0;
    }

    ST_vector vector = make_vector();
    vector.char_number = malloc( sizeof(char) * (K_max_input_number_len + 1));
    if (vector.char_number == NULL)
    {
        printf( "__LINE__ %d", __LINE__);
        fclose( thread_in);
        return 0;
    }

    scan_number( &vector, thread_in, &flag );
    if ( flag )
    {
        destroy_vector( &vector);
        fclose( thread_in);
        return 0;
    }

    fclose( thread_in);

    make_lower_char( &vector );
    get_dot_index( &vector );
    is_fractional( &vector);

    if ( is_valid_bases(base_from, base_to) && is_valid_char( &vector, base_from))
    {
        convert_number(&vector, base_from, base_to);
        print_converted_number( &vector );
    }
    else
    {
        printf("bad input\n" );
    }

    destroy_vector( &vector);

    return 0;
}
