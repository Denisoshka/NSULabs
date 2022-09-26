#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int K_base_numbers[16] = {0, 1, 2, 3,
                                4, 5, 6, 7,
                                8, 9, 10, 11,
                                12, 13, 14, 15};
const char K_dot = '.', K_zero = '0', K_nine = '9', K_min_letter = 'a';

const char * K_all_symbols = "0123456789abcdef";

int Valid_Bases(const int K_base1, const int K_base2);
int Valid_Char(const char * K_operand, const int K_base_from, const size_t operand_len);
size_t Dot_Index(const char * K_where_search);
int Fractional( const char * K_operand );
int necessary_number(const char symbol);

double To_Decimal( const int K_number_base1, const int K_is_fractional, const size_t K_operand_len,  const char * K_operand );

char * From_Decimal( double decimal, const int BASE_TO, const int K_is_fractional );
char * Lower_Char(const char * K_operand, const size_t K_operand_len);



char * Lower_Char(const char * K_operand, const size_t K_operand_len)
{
    char * new_char = calloc( K_operand_len, sizeof(char) );

    for ( size_t index = 0; index < K_operand_len; index++)
    {
        new_char[ index ] = (char)tolower( K_operand[ index ] );
    }
    return new_char;
}


int necessary_number(const char symbol){
    int in_range_2_9 = (K_zero <= symbol && symbol <= K_nine );

    if ( in_range_2_9 )
    {
        return K_base_numbers[ symbol - K_zero ];
    }else
    {
        return K_base_numbers[ symbol - K_min_letter + 10];
    }
}


int Symbol_less_base(const char symbol, const int K_base_from)
{
    int symbol_in_range_2_base_from_9 = ( ( K_zero <= symbol ) && ( symbol < K_zero + K_base_from ) );
    int symbol_in_range_a_base_from_f = ( ( K_min_letter <= symbol ) && ( symbol < K_min_letter + K_base_from - 10 ) );
    int condition = ( symbol <= K_nine ) ? symbol_in_range_2_base_from_9 : symbol_in_range_a_base_from_f;

    return condition;
}

//в функции мы проверяемм на то что в записи числа нет цифр больше базы, посторонние элементы проверяем при переводе числа
int Valid_Char(const char * K_operand, const int K_base_from, const size_t K_operand_len)
{
    size_t dot_index = K_operand_len;

    if ( strrchr(K_operand, K_dot ) != NULL )
    {
        dot_index = Dot_Index(K_operand);
        if ( ( dot_index == 0 ) || ( dot_index == K_operand_len - 1 ) )
        {
            return 0;
        }
    }
    for( size_t index = 0 ; index < dot_index; index++ )
    {
        if ( !Symbol_less_base( K_operand[ index ] , K_base_from ) )
        {
            return 0;
        }
    }
    for( size_t index = dot_index + 1 ; index < K_operand_len; index++ )
    {
        if ( !Symbol_less_base( K_operand[ index ], K_base_from ) )
        {
            return 0;
        }
    }
    return 1;
}

int Valid_Bases(const int K_base1, const int K_base2)
{
    return ( 2 <= K_base1 && K_base1 <= 16 && 2 <= K_base2 && K_base2<= 16 );
}

size_t Dot_Index(const char * K_where_search)
{
    size_t ind=0;
    for( ; K_where_search[ ind ] != K_dot ; ind++ )
        ;
    return ind;
}
int Fractional( const char *K_operand )
{
    if (strrchr(K_operand, K_dot) != NULL)
    {
        for ( size_t ind = Dot_Index( K_operand )+1 ; K_operand[ ind ] != '\0'; ind++)
        {
            if ( K_operand[ ind ] != K_zero)
            {
                return 1;
            }
        }
    }
    return 0;
}

double To_Decimal( const int K_number_base1, const int K_is_fractional, const size_t K_operand_len,  const char * K_operand )
{
    double result = 0;
    const size_t K_dot_index = ( strrchr(K_operand, K_dot ) != NULL ) ?  (int) Dot_Index(K_operand) : (int) K_operand_len;

    for ( size_t index = 0; index < K_dot_index; index++ )
    {
        result += necessary_number( K_operand[index] ) * pow(K_number_base1,  (int)K_dot_index - 1 - (int)index );
    }
    if ( K_is_fractional )
    {
        for ( size_t index = K_dot_index + 1; index < K_operand_len; index++ )
        {
            result += necessary_number( K_operand[index] ) * pow(K_number_base1, (int)K_dot_index - (int)index );
        }
    }
    return result;
}

char * From_Decimal( double decimal, const int BASE_TO, const int K_is_fractional )
{
    const int AFTER_POINT = 12;
    int int_size = 0;
    long long int int_decimal = ( long long int )decimal;
    double fractional_part = decimal - ( double )int_decimal;

    do{
        int_size++;
    }while( ( int )int_decimal >= pow(BASE_TO, int_size ) );

    const int decimal_size = ( K_is_fractional ) ? int_size + 1 + AFTER_POINT : int_size;
    char * result = calloc(decimal_size, sizeof( char ) );

    for( int ind = int_size-1; ind >= 0; ind-- )
    {
        result[ ind ] = K_all_symbols[ int_decimal % BASE_TO ];
        int_decimal /= BASE_TO;
    }
    if ( K_is_fractional ) {

        result[ int_size ] = K_dot;

        for ( int index = int_size + 1; index < decimal_size; index++ )
        {
            fractional_part *= BASE_TO;
            result[ index ] = K_all_symbols[ (int) fractional_part ];
            fractional_part -= (int) fractional_part;
        }
    }

    return result;
}

int main( void )
{
    int base_from, base_to, count_scan_systems, count_scan_number;
    double decimal_result;
    char main_operand[ 14 ];

    count_scan_systems = scanf("%d%d", &base_from, &base_to );
    count_scan_number = scanf("%13s", main_operand );

    if ( ( count_scan_systems != 2 ) || ( count_scan_number != 1 ))
    {
        printf("bad input\n" );
        return 0;
    }

    const size_t K_main_operand_len = strlen( main_operand );
    const int K_is_fractional = Fractional( main_operand );
    char * lower_main_operand = Lower_Char( main_operand, K_main_operand_len );

//блок с проверкой на соответствие баз и соответствия чисел числа базе
    if ( !Valid_Bases(base_from, base_to ) || !Valid_Char(lower_main_operand, base_from, K_main_operand_len) )
    {
        printf("bad input\n" );
        return 0;
    }
//блок проверок закончен

    char *char_result;
// здесь будем делать проверку на дробную часть
    decimal_result = To_Decimal( base_from, K_is_fractional, K_main_operand_len, lower_main_operand );
    char_result = From_Decimal( decimal_result, base_to, K_is_fractional );

    printf("%s\n", char_result );
    free(lower_main_operand);
    free( char_result );
    return 0;
}
