#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char actions_list[] = {'+', '-', '/', '*', '\0'};

const char start_brackets_list[] = { '(', '\0' };

const char end_brackets_list[] = { ')', '\0' };

const int actions_priority[] = {1, 1, 2, 2};

enum errors{
    ALL_GOOD = 0,
    SYNTAX_ERROR = -1,
    DIVISION_BY_ZERO = -2,
};
enum move{
    STAY = 0,
    MOVE_AWAY = 1,
};

void other_error(int line)
{
    printf( "line: %d", line);
    exit( EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// vector funcks
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct symbol_vector
{
    char array[1001];
    int array_len;
}symbol_vector;

typedef struct int_vector
{
    int array[1001];
    int array_len;
}int_vector;

symbol_vector create_char_stack(void)
{
    symbol_vector vector = {
            .array_len = 0,
    };
    return vector;
}

int_vector create_int_stack(void)
{
    int_vector vector = {
            .array_len = 0,
    };
    return vector;
}

int prepare_string_vector(symbol_vector *vector)
{
//    FILE * thread_in = fopen( "C:\\Users\\dinis\\CLionProjects\\lab4\\in", "r");
    FILE * thread_in = fopen( "in.txt", "r");
    if ( fscanf( thread_in,"%[^\n]s", vector->array) != 1)
    {
        return SYNTAX_ERROR;
    }
    vector->array_len = strlen( vector->array );
    fclose( thread_in );
    return ALL_GOOD;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// char stacks
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int is_empty_char_stack(symbol_vector * vector )
{
    return vector->array_len == 0;
}

void push_char_stack( symbol_vector * vector, char symbol)
{
    vector->array[ vector->array_len] = symbol;

    vector->array_len++;
}

char pop_char_stack( symbol_vector * vector)
{
    vector -> array_len--;
    return vector -> array[ vector -> array_len ];
}

char get_char_stack( symbol_vector * vector )
{
    return vector->array[ vector->array_len - 1 ];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// int stacks
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void push_int_stack( int_vector * vector, int number )
{
    vector->array[ vector->array_len ] = number;
    vector->array_len++;
}

int pop_int_stack( int_vector * vector)
{
    vector -> array_len--;
    return vector->array[ vector->array_len ];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int necessary_number( char symbol )
{
    return symbol - '0';
}

int do_action( int first_number, int second_number, const char symbol_of_action)
{
    if ( symbol_of_action == '+' )
    {
        return first_number + second_number;
    }
    if (symbol_of_action == '-')
    {
        return first_number - second_number;
    }
    if (symbol_of_action == '*')
    {
        return first_number * second_number;
    }
    if (symbol_of_action == '/')
    {
        return first_number / second_number;
    }
    return 0; //никогда не будет использоваться
}

int push_number_to_int_stack( int_vector * int_stack,symbol_vector * symbol_vector, const int start_index)
{
    char * string = symbol_vector->array;

    int string_len = symbol_vector->array_len,
        result = 0,
        index = start_index,
        end_index;

    for ( ; index < string_len; index++)
    {
        if( !( ('0' <= string[index]) && (string[index] <= '9') ) )
            break;
    }

    end_index = index;

    int back_index = index - start_index - 1;

    for ( int iteration = 0 ; back_index >= 0;  )
    {
        result += necessary_number( string[start_index + iteration] ) * (int)pow( 10, back_index );
        back_index--;
        iteration++;
    }

    push_int_stack( int_stack, result );

    return end_index;
}


int get_index_in_list( const char * where_search, char symbol)
{
    for ( int index = 0; where_search[ index ] != '\0'; index++ )
    {
        if ( where_search[index] == symbol )
            return index;
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// main count function
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int do_when_end_bracket( symbol_vector * char_stack, int_vector * int_stack ) {
    char old_action;

    int new_number,
            old_number,
            result;
    // если не встретим открыв скобку то сожрем стек и пошлем пользователя нахуй
    if (is_empty_char_stack(char_stack)) {
        return SYNTAX_ERROR;
    }

    old_action = pop_char_stack(char_stack);

    if (old_action == ')')
    {
        return SYNTAX_ERROR;
    }
    if (old_action == '(')
    {
    return MOVE_AWAY;
    }

    new_number = pop_int_stack( int_stack );
    old_number = pop_int_stack( int_stack );

    if ( old_action == '/' && new_number == 0)
    {
        return DIVISION_BY_ZERO;
    }

    result = do_action( old_number, new_number, old_action );

    push_int_stack( int_stack, result );

    return STAY;
}



int do_when_action(symbol_vector * char_stack, int_vector * int_stack, char action, int action_index)
{
    char old_action;
    int old_action_index,

            action_priority,
            old_action_priority,

    old_number,
    new_number,

            result;

    if(is_empty_char_stack( char_stack ))
    {
        push_char_stack( char_stack, action );

        return MOVE_AWAY;
    }
    else
    {
        old_action = get_char_stack( char_stack );

        if ( old_action == ')' )
        {
            return SYNTAX_ERROR;
        }

        if ( old_action == '(' )
        {
            push_char_stack( char_stack, action );
            return MOVE_AWAY;
        }

        old_action_index = get_index_in_list( actions_list, old_action );

        action_priority = actions_priority[ action_index];
        old_action_priority = actions_priority[ old_action_index ];

        if ( old_action_priority >= action_priority )
        {
            old_action = pop_char_stack( char_stack );
            if( int_stack-> array_len < 2)
            {
                return SYNTAX_ERROR;
            }
            new_number = pop_int_stack( int_stack );
            old_number = pop_int_stack( int_stack );

            if ( old_action == '/' && new_number == 0 )
            {
                return DIVISION_BY_ZERO;
            }

            result = do_action( old_number, new_number, old_action);
            push_int_stack( int_stack, result );

            return STAY;
        }
        else
        {
            push_char_stack( char_stack, action );
            return MOVE_AWAY;
        }
    }
    return 0; // neves uses
}


int final_calculations( int_vector * int_stack, symbol_vector * char_stack )
{
    int new_number,
        old_number,
        result;
    char action;

    while ( !is_empty_char_stack(char_stack) )
    {
        action = pop_char_stack( char_stack);
        if ( int_stack -> array_len < 2 )
        {
            return SYNTAX_ERROR;
        }
        if (get_index_in_list( actions_list, action ) == -1 )
        {
            return SYNTAX_ERROR;
        }

        new_number = pop_int_stack( int_stack);
        old_number = pop_int_stack( int_stack);

        if (action == '/' && new_number == 0 )
        {
            return DIVISION_BY_ZERO;
        }
        result = do_action( old_number, new_number, action );
        push_int_stack( int_stack, result);
    }
    if (  char_stack->array_len == 0 && int_stack->array_len == 1)
    {
        return ALL_GOOD;
    }
    else
    {
        return SYNTAX_ERROR;
    }
}

int count_expression(symbol_vector * string_vector, symbol_vector * char_stack, int_vector * int_stack )
{
    char * string = string_vector->array;
    int string_len = string_vector->array_len,

            action_index,
            flag = 0,

        is_digit,
        returned_result;

    char symbol;

    for (int index = 0 ; index < string_len ; )
    {
        symbol = string[index];
        is_digit = '0' <= symbol && symbol <= '9';

        if (is_digit)
        {
            index = push_number_to_int_stack( int_stack, string_vector, index);
        }
        else if ( get_index_in_list(start_brackets_list, symbol) != -1 )
        {
            if ( string[index+1] == ')')
            {
                flag = SYNTAX_ERROR;
                break;
            }
            push_char_stack(char_stack, symbol);
            index++;
        }
        else if ( get_index_in_list(end_brackets_list, symbol) != -1)
        {
            returned_result = do_when_end_bracket(char_stack, int_stack);

            if ( returned_result < 0 )
            {
                flag = returned_result;
                break;
            }

            index += returned_result;
        }
        else if ( ( action_index = get_index_in_list( actions_list, symbol ) ) != -1 )
        {
            returned_result = do_when_action(char_stack, int_stack, symbol, action_index);

            if ( returned_result < 0 )
            {
                flag = -1;
                break;
            }

            index += returned_result;
        }
        else
        {
            flag = SYNTAX_ERROR;
            break;
        }
    }

    if ( flag < 0 )
    {
        return flag;
    }

    return final_calculations( int_stack, char_stack );
}

int main(void)
{
    symbol_vector string_vector = create_char_stack();
    symbol_vector char_stack = create_char_stack();
    int_vector int_stack = create_int_stack();

    int flag = prepare_string_vector( &string_vector);

    if ( flag == SYNTAX_ERROR)
    {
        printf( "syntax error");
        return 0;
    }

    flag = count_expression( &string_vector, &char_stack, &int_stack);

    if ( flag == ALL_GOOD )
    {
        printf("%d", int_stack.array[0]);
    }
    if ( flag == SYNTAX_ERROR)
    {
        printf( "syntax error");
    }
    if ( flag == DIVISION_BY_ZERO)
    {
        printf( "division by zero");
    }
    return 0;
}
