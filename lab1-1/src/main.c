#include <stdio.h>
#include <stdlib.h>

const int K_max_pattern_len = 16,
     K_max_chunk_len = 1024,
     K_hash_base = 3;

typedef struct vector{
    unsigned char * array;
    int array_len;
    int additional;
    int hash;
}vector;

vector create_vector( const int necessary_array_len )
{
    vector blank = {
            .array = malloc( sizeof(unsigned char) * necessary_array_len ),
            .array_len = necessary_array_len,
            .additional = 0,
            .hash = 0,
    };
    return blank;
}
void destroy_vector( vector * blank )
{
    free( blank->array );
    blank->array = NULL;
}

void scan_pattern( vector * pattern, FILE * thread_in )
{
    int index = 0;
    unsigned char symbol;

    for ( ; ( 1 == fread( &symbol, sizeof(unsigned char), 1, thread_in ) ) && symbol != '\n'; index++)
    {
        pattern->array[index] = symbol;
    }
    pattern->array_len = index;
}

int get_hash(vector * blank, const vector * pattern)
{
    int degree = 1;
    for( int index  = 0 ; index < pattern->array_len; index++, degree *= K_hash_base)
    {
        blank->hash += ( blank->array[index] % K_hash_base ) * degree;
    }
    return degree/3;
}

void text_rewrite( vector * text, const vector * pattern, FILE * thread_in )
{
    int where_start_rewrite = text->additional - pattern->array_len + 1;
    int iterations_quantity = pattern->array_len - text->additional + text->array_len - 1;

    for ( int iteration = 0; iteration < iterations_quantity ; iteration++, where_start_rewrite++)
    {
        text->array[iteration] = text->array[where_start_rewrite];
    }
    text -> array_len = (int)fread( text->array + iterations_quantity, sizeof( unsigned char), text->array_len - iterations_quantity, thread_in) + iterations_quantity;
    text->additional = pattern->array_len - 1;
}

void update_hash_and_get(vector * text, const vector * pattern, const int flag )
{
    if (flag)
    {
        text->hash -= text->array[text->additional - pattern->array_len] % K_hash_base;
    }
    else
    {
        text->hash -= text->array[0] % K_hash_base;
    }
    text->hash /= K_hash_base;
    text->hash += (text->array[text->additional] % K_hash_base) * pattern->additional;
}

void search_substring(vector * text, const vector * pattern, const int * total_index, FILE * thread_out)
{
    int print_index = * total_index - pattern->array_len + 1;

    for( int pattern_work_index = 0, text_work_index = text->additional - pattern->array_len + 1; pattern_work_index < pattern->array_len; pattern_work_index++, text_work_index++)
    {
        fprintf( thread_out, "%d ", print_index + pattern_work_index);

        if( text->array[ text_work_index ] != pattern->array[ pattern_work_index ] )
        {
            break;
        }
    }
}

void rabin_carp_algorithm(const vector * pattern, FILE * thread_in)
{
    vector text = create_vector( K_max_chunk_len );
    if ( text.array == NULL)
    {
        printf("__LINE__ %d", __LINE__);
        return;
    }
    FILE * thread_out = fopen( "out.txt", "w");
    if ( thread_out == NULL)
    {
        destroy_vector( &text );
        printf("__LINE__ %d", __LINE__);
        return;
    }
    
    int total_index = pattern->array_len;
    text.additional = pattern->array_len - 1;
    text.array_len = (int)fread( text.array, sizeof (unsigned char ), K_max_chunk_len, thread_in);

    fprintf( thread_out, "%d ", pattern->hash);

    if (pattern->array_len <= text.array_len)
    {
        get_hash( &text, pattern);
    }
    while (pattern->array_len <= text.array_len)
    {
        if (pattern->hash == text.hash)
        {
            search_substring( &text, pattern, &total_index,  thread_out);
        }

        total_index++;
        text.additional++;

        if ( text.additional >= text.array_len )
        {
            text_rewrite(&text, pattern, thread_in);
            update_hash_and_get(&text, pattern, 0);
        }
        else
        {
            update_hash_and_get(&text, pattern, 1);
        }
    }
    
    fclose( thread_out);
    destroy_vector( &text);
}

int main(void)
{
    FILE * thread_in = fopen( "in.txt", "r");
    if ( thread_in == NULL )
    {
        printf("__LINE__ %d", __LINE__);
        return 0;
    }
    vector pattern = create_vector( K_max_pattern_len );
    if ( pattern.array == NULL)
    {
        fclose( thread_in );
        printf("__LINE__ %d", __LINE__);
        return 0;
    }
    

    scan_pattern( &pattern, thread_in);
    pattern.additional = get_hash( &pattern, &pattern);
    
    rabin_carp_algorithm( &pattern, thread_in );

    fclose( thread_in );
    destroy_vector( &pattern );
    
    return 0;
}
