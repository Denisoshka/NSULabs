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
int Valid_Char(const char * K_operand, const int K_base_from, int operand_len);
int Dot_Index(const char * K_where_search);
int Fractional( const char * K_operand );
int Necessary_Number(const char symbol);

double To_Decimal( const int K_number_base1, const int K_is_fractional, const int K_operand_len,  const char * K_operand );

char * From_Decimal( double decimal, const int BASE_TO, const int K_is_fractional );
char * Lower_Char(const char * K_operand, const int K_operand_len);



char * Lower_Char(const char * K_operand, const int K_operand_len)
{
    char * new_char = calloc( K_operand_len, sizeof(char) );

    for ( int index = 0; index < K_operand_len; index++)
    {
        new_char[ index ] = tolower( K_operand[ index ] );
    }
    return new_char;
}


int Necessary_Number(const char symbol){
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
int Valid_Char(const char * K_operand, const int K_base_from, const int K_operand_len)
{
    int dot_index = K_operand_len;

    if ( strrchr(K_operand, K_dot ) != NULL )
    {
        dot_index = Dot_Index(K_operand);

        if ( ( dot_index == 0 ) || ( dot_index == K_operand_len - 1 ) )
        {
            return 0;
        }
    }
    for(int index = 0 ; index < dot_index; index++ )
    {
        if ( !Symbol_less_base( K_operand[ index ] , K_base_from ) )
        {
            return 0;
        }
    }
    for(int index = dot_index + 1 ; index < K_operand_len; index++ )
    {
        if ( !Symbol_less_base( K_operand[ index ], K_base_from ) )
        {
            return 0;
        }
    }
    return 1;
}

int Valid_Bases(const int K_base1,  int K_base2)
{
    return ( 2 <= K_base1 && K_base1 <= 16 && 2 <= K_base2 && K_base2<= 16 );
}

int Dot_Index(const char * K_where_search)
{
    int ind=0;
    for( ; K_where_search[ ind ] != K_dot ; ind++ )
        ;
    return ind;
}
int Fractional( const char *K_operand )
{
    if (strrchr(K_operand, K_dot) != NULL)
    {
        for (int ind = Dot_Index( K_operand )+1 ; K_operand[ ind ] != '\0'; ind++)
        {
            if ( K_operand[ ind ] != K_zero)
            {
                return 1;
            }
        }
    }
    return 0;
}

double To_Decimal( const int K_number_base1, const int K_is_fractional, const int K_operand_len,  const char * K_operand )
{
    double result = 0;
    const int K_dot_index = ( strrchr(K_operand, K_dot ) != NULL ) ?  Dot_Index(K_operand) : K_operand_len;

    for ( int index = 0; index < K_dot_index; index++ )
    {
        result += Necessary_Number( K_operand[index] ) * pow(K_number_base1, K_dot_index - 1 - index );
    }
    if ( K_is_fractional )
    {
        for ( int index = K_dot_index + 1; index < K_operand_len; index++ )
        {
            result += Necessary_Number( K_operand[index] ) * pow(K_number_base1, K_dot_index - index );
        }
    }
    return result;
}

char * From_Decimal( double decimal, const int BASE_TO, const int K_is_fractional )
{
    const int AFTER_POINT = 12;
    int int_size = 0;
    long long int int_decimal = decimal;
    double fractional_part = decimal - int_decimal;

    do{
        int_size++;
    }while( int_decimal >= pow(BASE_TO, int_size ) );

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
    int base_from, base_to, count_scan;
    double decimal_result;
    char main_operand[ 14 ];

    count_scan = scanf("%d%d", &base_from, &base_to );
    scanf("%13s", main_operand );

    if ( count_scan != 2 )
    {
        printf("bad input" );
        return 0;
    }

    const int K_main_operand_len = strlen( main_operand );
    const int K_is_fractional = Fractional( main_operand );
    char * lower_main_operand = Lower_Char( main_operand, K_main_operand_len );

//блок с проверкой на соответствие баз и соответствия чисел числа базе
    if ( !Valid_Bases(base_from, base_to ) || !Valid_Char(lower_main_operand, base_from, K_main_operand_len) )
    {
        printf("bad input" );
        return 0;
    }
//блок проверок закончен

    char *char_result;
// здесь будем делать проверку на дробную часть
    decimal_result = To_Decimal( base_from, K_is_fractional, K_main_operand_len, lower_main_operand );
    char_result = From_Decimal( decimal_result, base_to, K_is_fractional );

    printf("%s", char_result );

    free( char_result );
    return 0;
}
