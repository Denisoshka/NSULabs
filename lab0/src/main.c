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
const int K_other_error = 1;

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

void other_error( int line, int * flag, int error )
{
    printf( "line: %d", line);
    * flag = error;
}

void scan_number(ST_vector * vector, int * flag)
{
    FILE * thread_in = fopen( "C:\\Users\\dinis\\Desktop\\template-lab0\\lab0\\test\\in.txt", "r");
    if( thread_in == NULL)
    {
        other_error( __LINE__, flag, K_other_error);
        return;
    }

    vector -> char_number = malloc( (K_max_input_number_len + 1) * sizeof(char) );

    if (vector -> char_number == NULL)
    {
        other_error( __LINE__, flag, K_other_error);
        return;
    }

    if( fscanf(thread_in, "%13s", vector->char_number) != 1 )
    {
        other_error(__LINE__, flag, K_other_error);
        return;
    }

    vector -> number_len = (int)strlen( vector->char_number);

    fclose( thread_in);
}

void destroy_vector( ST_vector * our_vector )
{
    free( our_vector->char_number);
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

int return_necessary_number( const char symbol )
{
    int in_range_2_9 = ( '0' <= symbol) && (symbol <= '9' );

    if ( in_range_2_9 )
    {
        return K_base_numbers[ symbol - '0' ];
    }else
    {
        return K_base_numbers[ symbol - 'a' + 10];
    }
}

int is_symbol_less_base( const char symbol, const int K_base_from )
{
    int symbol_in_range_2_base_from_9 = ( ( '0' <= symbol ) && ( symbol < '0' + K_base_from ) );
    int symbol_in_range_a_base_from_f = ( ( 'a' <= symbol ) && ( symbol < 'a' + K_base_from - 10 ) );
    int condition = ( symbol <= '9' ) ? symbol_in_range_2_base_from_9 : symbol_in_range_a_base_from_f;

    return condition;
}

int is_valid_char( const ST_vector * our_vector, const int K_base_from )
{
    if ( ( our_vector -> dot_index == 0 ) || ( our_vector -> dot_index == our_vector -> number_len - 1 ) )
    {
        return 0;
    }

    for( int index = 0 ; index < our_vector -> dot_index; index++ )
    {
        if ( !is_symbol_less_base( our_vector -> char_number[ index ] , K_base_from ) )
        {
            return 0;
        }
    }

    for( int index = our_vector -> dot_index + 1 ; index < our_vector -> number_len; index++ )
    {
        if ( !is_symbol_less_base( our_vector -> char_number[ index ], K_base_from ) )
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


void prepare_vector( ST_vector * our_vector)
{
    make_lower_char( our_vector);
    get_dot_index( our_vector );
    is_fractional( our_vector);
}


double convert_from_char_to_double(ST_vector * our_vector, const int K_number_base1)
{
    double result = 0;
    int additional_index;

    for ( int front_index = 0; front_index < our_vector->dot_index; front_index++ )
    {
        additional_index = our_vector -> dot_index - 1 - front_index;
        result += return_necessary_number( our_vector->char_number[front_index] ) * pow(K_number_base1,  (int)additional_index ) ;
    }

    for ( int index = our_vector->dot_index + 1; index < our_vector->number_len; index++ )
    {
        additional_index = our_vector->dot_index - index;
        result += return_necessary_number( our_vector->char_number[index] ) * pow(K_number_base1, additional_index );
    }
    return result;
}


void convert_from_double_to_char(ST_vector * our_vector, double decimal, const int K_base_to )
{
    int result_non_fractional_part_len = 0;
    long int int_part_of_decimal = ( long int )decimal;
    double fractional_part_of_decimal = decimal - ( double )int_part_of_decimal;

    do{
        result_non_fractional_part_len++;
    }while( int_part_of_decimal >= (long int)pow(K_base_to, result_non_fractional_part_len ) );

    int result_len = result_non_fractional_part_len + 1 + K_max_after_dot_numbers;

    our_vector->char_number = realloc( our_vector->char_number, (result_len + 1) * sizeof(char));

    for( int back_index = result_non_fractional_part_len - 1; back_index >= 0; back_index-- )
    {
        our_vector->char_number[back_index] = K_all_symbols[int_part_of_decimal % K_base_to];
        int_part_of_decimal /= K_base_to;
    }

    our_vector->char_number[result_non_fractional_part_len] = '.';

    for ( int index = result_non_fractional_part_len + 1; index < result_len; index++ )
    {
        fractional_part_of_decimal *= K_base_to;
        our_vector->char_number[ index ] = K_all_symbols[ (int)fractional_part_of_decimal ];
        fractional_part_of_decimal -= (int)fractional_part_of_decimal;
    }

    our_vector->char_number[ result_len ] = '\0';
}


long long int convert_from_char_to_int( ST_vector * our_vector, const int K_number_base_from)
{
    long long int result = 0;

    for ( int front_index = 0; front_index < our_vector->dot_index; front_index++ )
    {
        int back_index = our_vector->dot_index - 1 - front_index;
        
        result += (long long int)return_necessary_number( our_vector->char_number[ front_index ] ) * (long long int)pow(K_number_base_from, back_index ) ;
    }
    return result;
}


void convert_from_int_to_char( ST_vector * our_vector, long long int int_number, const long long int K_base_to )
{
    int result_len = 0;

    do{
        result_len++;
    }while( int_number >= (long long int)pow((double)K_base_to, (double)result_len ) );

    our_vector->char_number = realloc( our_vector->char_number, ( result_len + 1 ) * sizeof(char) );

    for(int back_index = result_len - 1; back_index >= 0 ; back_index-- )
    {
        our_vector->char_number[ back_index ] = K_all_symbols[ int_number % K_base_to ];
        int_number /= K_base_to;
    }
    our_vector->char_number[ result_len ] = '\0';
}


void print_converted_number(ST_vector *our_vector, const int K_base_from, const int K_base_to, int * flag )
{
    if ( our_vector->fractional )
    {
        double result = convert_from_char_to_double(our_vector, K_base_from );
        convert_from_double_to_char(our_vector, result, K_base_to );
    }
    else
    {
        long long int result = convert_from_char_to_int( our_vector, K_base_from );
        convert_from_int_to_char( our_vector, result, K_base_to );
    }

    FILE * thread_out = fopen("C:\\Users\\dinis\\Desktop\\template-lab0\\lab0\\test\\out.txt", "w");
    if ( thread_out == NULL )
    {
        other_error( __LINE__, flag, K_other_error);
        return;
    }
    fprintf(thread_out, "%s\n", our_vector->char_number );
    fclose( thread_out );
}


int are_all_conditions_complied( ST_vector *our_vector, const int K_count_of_scan,
                                  const int K_base_to, const int K_base_from)
{
    if ( our_vector->char_number[0] == '\0' )
    {
        return 0;
    }

    if ( !is_valid_bases(K_base_from, K_base_to) )
    {
        return 0;
    }

    if ( !is_valid_char( our_vector, K_base_from) )
    {
        return 0;
    }

    if ( K_count_of_scan != 2)
    {
        return 0;
    }
    return 1;
}

int main(void )
{
    int flag= 0,
        base_from,
        base_to,
        count_of_scan_systems;

    count_of_scan_systems = scanf("%d%d", &base_from, &base_to);

    ST_vector main_vector = make_vector();

    scan_number( &main_vector, &flag );
    if ( flag!=0)
    {
        destroy_vector( &main_vector);
        return 0;
    }

    prepare_vector( &main_vector);

    if ( are_all_conditions_complied( &main_vector, count_of_scan_systems, base_to, base_from ) )
    {
        print_converted_number(&main_vector, base_from, base_to, &flag);
        if ( flag != 0)
        {
            return 0;
        }
    }
    else
    {
        printf("bad input\n" );
    }

    destroy_vector( &main_vector);

    return 0;
}
