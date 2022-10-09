#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int K_base_numbers[16] = {0, 1, 2, 3,
                                4, 5, 6, 7,
                                8, 9, 10, 11,
                                12, 13, 14, 15};
const char K_dot = '.', K_zero = '0', K_nine = '9', K_min_letter = 'a', K_end_of_string = '\0';
const int K_max_input_number_len = 13;
const int K_max_after_dot_numbers = 12;
const int K_true = 1, K_false = 0;
typedef struct ST_vector
{
    char * Char_number;
    int number_len;
    int dot_index;
    int fractional;
}ST_vector;

ST_vector Make_Vector(void)
{
    ST_vector our_vector = {
            .Char_number = NULL,
            .dot_index = 0,
            .number_len = 0,
            .fractional = 0,
    };
    return our_vector;
}

const char * K_all_symbols = "0123456789abcdef";

void Scan_Number( ST_vector * our_vector)
{
    char symbol;
    if (fread(&symbol, sizeof(char), 1, stdin) != 1)
    {
//        здесь должен быть код для того что бы что то сделать с возвращаемым значением
    }

    if ( symbol == '\n' ) //  как я понял после scanf указатель на чтение строки застывает на \n и этот костыль избавляет нас от этой проблемы
    {
        for (int index = 0; index < K_max_input_number_len; index++) {

            if(fread(&symbol, sizeof(char), 1, stdin) != 1)
            {
//              здесь должен быть код для того что бы что то сделать с возвращаемым значением
            }
            if (symbol != '\n') {
                our_vector->number_len++;
                our_vector->Char_number = realloc(our_vector->Char_number,sizeof(char) * ((our_vector->number_len) + 1));
                our_vector->Char_number[index] = symbol;
            } else {
                our_vector->Char_number[index] = K_end_of_string;
                return;
            }
        }
    }
}

void Destroy_Vector( ST_vector * our_vector )
{
    free( our_vector->Char_number);
}

void Lower_Char(ST_vector * our_vector)
{
    for ( int index = 0; index < ( our_vector->number_len ) ; index++)
    {
        our_vector -> Char_number[ index ] = (char)tolower( our_vector->Char_number[ index ] );
    }
}

void Dot_Index( ST_vector * our_vector )
{
    int index=0;
    for( ; index < our_vector->number_len; index++ )
    {
        if ( our_vector->Char_number[index] == K_dot ){
            our_vector->dot_index = index;
            return;
        }
    }
    our_vector->dot_index = index;
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

int Symbol_Less_Base(const char symbol, const int K_base_from)
{
    int symbol_in_range_2_base_from_9 = ( ( K_zero <= symbol ) && ( symbol < K_zero + K_base_from ) );
    int symbol_in_range_a_base_from_f = ( ( K_min_letter <= symbol ) && ( symbol < K_min_letter + K_base_from - 10 ) );
    int condition = ( symbol <= K_nine ) ? symbol_in_range_2_base_from_9 : symbol_in_range_a_base_from_f;

    return condition;
}

size_t Valid_Char(ST_vector * our_vector, const int K_base_from)
{

    if ( ( our_vector->dot_index == 0 ) || ( our_vector -> dot_index == our_vector -> number_len - 1 ) )
    {
        return K_false;
    }

    for( int index = 0 ; index < our_vector -> dot_index; index++ )
    {
        if ( !Symbol_Less_Base( our_vector -> Char_number[ index ] , K_base_from ) )
        {
            return K_false;
        }
    }
    for( int index = our_vector -> dot_index + 1 ; index < our_vector -> number_len; index++ )
    {
        if ( !Symbol_Less_Base( our_vector -> Char_number[ index ], K_base_from ) )
        {
            return K_false;
        }
    }
    return K_true;
}

size_t Valid_Bases(const int K_base1, const int K_base2)
{
    return ( 2 <= K_base1 && K_base1 <= 16 && 2 <= K_base2 && K_base2<= 16 );
}
void Fractional( ST_vector * our_vector )
{
        for ( int index = our_vector -> dot_index + 1 ; index < our_vector->number_len; index++)
        {
            if ( our_vector-> Char_number[ index ] != K_zero)
            {
                our_vector->fractional = K_true;
                return;
            }
        }

    our_vector->fractional = K_false;
}

void Prepare_String( ST_vector * our_vector)
{
    Lower_Char( our_vector);
    Dot_Index( our_vector );
    Fractional( our_vector);
}


double From_Char_To_Double( ST_vector * our_vector, const int K_number_base1)
{
    double result = 0;
    int additional_index;

    for ( int front_index = 0; front_index < our_vector->dot_index; front_index++ )
    {
        additional_index = our_vector -> dot_index - 1 - front_index;
        result += Necessary_Number( our_vector->Char_number[front_index] ) * pow(K_number_base1,  (int)additional_index ) ;
    }

    for ( int index = our_vector->dot_index + 1; index < our_vector->number_len; index++ )
    {
        additional_index = our_vector->dot_index - index;
        result += Necessary_Number( our_vector->Char_number[index] ) * pow(K_number_base1, additional_index );
    }
    return result;
}

void From_Double_To_Char( ST_vector * our_vector, double decimal,  const int K_base_to )
{
    int result_non_fractional_part_len = 0;
    long int int_part_of_decimal = ( long int )decimal;
    double fractional_part_of_decimal = decimal - ( double )int_part_of_decimal;

    do{
        result_non_fractional_part_len++;
    }while( int_part_of_decimal >= (long int)pow(K_base_to, result_non_fractional_part_len ) );

    int result_len = result_non_fractional_part_len + 1 + K_max_after_dot_numbers;
    our_vector->Char_number = realloc( our_vector->Char_number, (result_len + 1) * sizeof(char));

    for( int back_index = result_non_fractional_part_len - 1; back_index >= 0; back_index-- ) {
        our_vector->Char_number[back_index] = K_all_symbols[int_part_of_decimal % K_base_to];
        int_part_of_decimal /= K_base_to;
    }

    our_vector->Char_number[result_non_fractional_part_len] = K_dot;

    for ( int index = result_non_fractional_part_len + 1; index < result_len; index++ )
    {
        fractional_part_of_decimal *= K_base_to;
        our_vector->Char_number[ index ] = K_all_symbols[ (int)fractional_part_of_decimal ];
        fractional_part_of_decimal -= (int)fractional_part_of_decimal;
    }

    our_vector->Char_number[ result_len ] = K_end_of_string;
}

long long int From_Char_To_Int( ST_vector * our_vector, const int K_number_base_from)
{
    long long int result = 0;
    
    for ( int front_index = 0; front_index < our_vector->dot_index; front_index++ )
    {
        int back_index = our_vector->dot_index - 1 - front_index;
        /*back_index - отвечает за то в какую степень будем возводить число
          front_index - отвечает за расположение числа в строке */
        result += (long long int)Necessary_Number( our_vector->Char_number[ front_index ] ) * (long long int)pow(K_number_base_from, back_index ) ;
    }
    return result;
}

void From_Int_To_Char( ST_vector * our_vector, long long int int_number, const long long int K_base_to )
{
    int result_len = 0;

    do{
        result_len++;
    }while( int_number >= (long long int)pow((double)K_base_to, (double)result_len ) );

    our_vector->Char_number = realloc( our_vector->Char_number, ( result_len + 1 ) * sizeof(char) );

    for(int back_index = result_len - 1; back_index >= 0 ; back_index-- )
    {
        our_vector->Char_number[ back_index ] = K_all_symbols[ int_number % K_base_to ];
        int_number /= K_base_to;
    }
    our_vector->Char_number[ result_len ] = K_end_of_string;
}


int main( void )
{
    int base_from, base_to, count_of_scan_systems;
    count_of_scan_systems = scanf("%d%d", &base_from, &base_to);

    ST_vector main_vector = Make_Vector();
    Scan_Number( &main_vector );

    Prepare_String( &main_vector);

    if (main_vector.Char_number[0] == K_end_of_string ||
    !Valid_Bases(base_from, base_to) ||
    !Valid_Char(&main_vector, base_from) ||
    (count_of_scan_systems != 2)
    )
    {
        printf("bad input\n");
        Destroy_Vector( &main_vector);
        return 0;
    }

    if ( main_vector.fractional )
    {
        double result = From_Char_To_Double( &main_vector, base_from );
        From_Double_To_Char( &main_vector, result, base_to);
        printf("%s\n", main_vector.Char_number);
    }
    else
    {
        long long int result = From_Char_To_Int( &main_vector, base_from);
        From_Int_To_Char( &main_vector, result, base_to);
        printf( "%s\n", main_vector.Char_number);
    }
    Destroy_Vector( &main_vector );
    return 0;
}
