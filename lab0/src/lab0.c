#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

const int BASE_NUMBERS[16] = {0, 1, 2, 3,
                              4, 5, 6, 7,
                              8, 9, 10, 11,
                              12, 13, 14, 15};
const char * ALL_SYMBOLS = "0123456789abcdef";

//блок проверок
//в функции мы проверяемм на то что в записи числа нет цифр больше базы, посторонние элементы проверяем при переводе числа
int Valid_Char(const char * OPERAND, const int BASE_FROM){
    const char * IND;
    for(IND = OPERAND; *IND!='\0'; IND++) {
        if ((*IND >= '0' + BASE_FROM) && (*IND != '.') && ((2 <= BASE_FROM) && (BASE_FROM <= 9))) {
            return 0;
        } else if (((tolower(*IND) >= 'a' + BASE_FROM - 10)) && ((10 <= BASE_FROM) && (*IND != '.') && (BASE_FROM <= 16))) {
            return 0;
        }
    }
    return 1;
}

int Valid_Bases(const int BASE1, int BASE2){
    return ( 2 <= BASE1 && BASE1<= 16 && 2 <= BASE2 && BASE2<= 16 );
}

int Dot_Index(const char * WHERE_SEARCH){
    int ind=0;
    for( ; *(WHERE_SEARCH+ind)!= '.' ; ind++ )
        ;
    return ind;
}
int Fractional(const char *OPERAND){
    if (strrchr(OPERAND, '.') != NULL) {
        for (int ind = Dot_Index( OPERAND )+1 ; OPERAND[ ind ] != '\0'; ind++) {
            if ( OPERAND[ ind ] != '0') {
                return 1;
            }
        }
    }
    return 0;
}

long double To_Decimal( const int NUMBER_BASE1, const int IS_FRACTIONAL, const char * OPERAND ) {

    long double result = 0;
    int dot_ind = strlen(OPERAND );
    const int OPERAND_LEN = dot_ind;
    if ( strrchr(OPERAND, '.' ) != NULL ){
        dot_ind = Dot_Index(OPERAND );
        if ( ( dot_ind == 0 ) || ( dot_ind == OPERAND_LEN - 1 ) ) return -1000;
    }
    for ( int ind = 0; ind < dot_ind; ind++ ) {
        if ( '0' <= OPERAND[ ind ] && OPERAND[ ind ] <= '9' ) {
            result += BASE_NUMBERS[ OPERAND[ ind ] - '0' ] * pow(NUMBER_BASE1, dot_ind - 1 - ind );
        } else if ( ('a' <= tolower( OPERAND[ ind ]) && tolower( OPERAND[ ind ]) <= 'f' ) ) {
            result += BASE_NUMBERS[ tolower( OPERAND[ ind ]) - 'a' + 10 ] * pow(NUMBER_BASE1, dot_ind - 1 - ind );
        } else {
            return -1000;
        }
    }
    if ( IS_FRACTIONAL ){
        for ( int ind = dot_ind + 1; ind < OPERAND_LEN; ind++ ) {
            if ( '0' <= OPERAND[ ind ] && OPERAND[ ind ] <= '9' ) {
                result += BASE_NUMBERS[ OPERAND[ ind ] - '0' ] * pow(NUMBER_BASE1, dot_ind-ind );
            } else if ( ( 'a' <= tolower( OPERAND[ ind ] ) && tolower( OPERAND[ ind ] ) <= 'f' ) ) {
                result += BASE_NUMBERS[ tolower( OPERAND[ ind ] ) - 'a' + 10 ] * pow(NUMBER_BASE1, dot_ind-ind );
            } else {
                return -1000;
            }
        }
    }
    return result;
}

char * From_Decimal( long double decimal, const int BASE_TO, const int IS_FRACTIONAL ){
    const int AFTER_POINT = 12;
    int int_size = 0;
    long long int int_decimal = (long long int)decimal;
    long double fractional_part = 0;

    if ( IS_FRACTIONAL ) {
        fractional_part = decimal - int_decimal;
    }
    do{
        int_size++;
    }while( int_decimal >= pow(BASE_TO, int_size ) );

    int decimal_size = ( IS_FRACTIONAL ) ? int_size + 1 + AFTER_POINT : int_size;
    char * result = calloc(decimal_size, sizeof( char ) );

    for( int ind = int_size-1; ind>=0; ind-- ){
        result[ ind ] = ALL_SYMBOLS[ int_decimal % BASE_TO ];
        int_decimal /= BASE_TO;
    }
    if ( IS_FRACTIONAL ) {
        result[ int_size ] = '.';
        for ( int ind = int_size + 1; ind < decimal_size; ind++ ) {
            fractional_part *= BASE_TO;
            result[ ind ] = ALL_SYMBOLS[ (int)fractional_part ];
            fractional_part -= (int)fractional_part;
        }
        return result;
    }
    else{
        return result;
    }
}

int main( void ) {
    int base_from, base_to, is_fractional, count_scan;
    char main_operand[ 14 ]={0};
    long double decimal_result;

    count_scan = scanf("%d%d", &base_from, &base_to );
    scanf("%s", main_operand );
    if ( count_scan != 2 ){
        printf("bad input" );
        return 0;
    }
//блок с проверкой на соответствие баз и соответствия чисел числа базе
    if ( !Valid_Bases(base_from, base_to ) || !Valid_Char(main_operand, base_from ) ){
        printf("bad input" );
        return 0;
    }
//блок проверок закончен

    char *char_result;
// здесь будем делать проверку на дробную часть
    is_fractional = Fractional(main_operand );
    decimal_result = To_Decimal(base_from, is_fractional, main_operand );
    char_result=From_Decimal(decimal_result, base_to, is_fractional );
    if ( decimal_result == -1000 ) {
        printf("bad input" ); }
    else {
        printf("%s", char_result );
    }
    free( char_result );
    return 0;
}
