#include <stdio.h>
#include <stdlib.h>

const int K_max_pattern_len = 16,
          K_max_chunk_len = 1024,
          K_hash_base = 3;

typedef struct our_custom_array{
    unsigned char * array;
    int array_len;
    int additional;
    int hash;
}our_custom_array;

our_custom_array create_custom_array( const int necessary_array_len )
{
    our_custom_array blank = {
            .array = malloc( sizeof(unsigned char) * necessary_array_len ),
            .array_len = necessary_array_len,
            .additional = 0,
            .hash = 0,
    };
    return blank;
}
void destroy_custom_array(our_custom_array * blank )
{
    free( blank->array );
    blank->array = NULL;
}

int scan_pattern(our_custom_array * pattern, FILE * stream_in )
{
    int index = 0;
    unsigned char symbol;

    for ( ; ; index++)
    {
        if( 1 != fread(&symbol, sizeof(unsigned char), 1, stream_in ) )
        {
            fprintf( stderr, "__LINE__ %d\n", __LINE__);
            return 1;
        }

        if ( symbol == '\n' )
        {
            break;
        }
        pattern->array[index] = symbol;
    }
    pattern->array_len = index;
    return 0;
}

int get_hash(our_custom_array * blank, const our_custom_array * pattern)
{
    int degree = 1;
    for( int index  = 0 ; index < pattern->array_len; index++, degree *= K_hash_base)
    {
        blank->hash += ( blank->array[index] % K_hash_base ) * degree;
    }
    return degree/3;
}

void text_rewrite(our_custom_array * text, const our_custom_array * pattern, FILE * stream_in )
{
    int where_start_rewrite = text->additional - pattern->array_len + 1;
    const int iterations_quantity = pattern->array_len - text->additional + text->array_len - 1;

    for ( int iteration = 0; iteration < iterations_quantity ; iteration++, where_start_rewrite++)
    {
        text->array[iteration] = text->array[where_start_rewrite];
    }
    text -> array_len = (int)fread( text->array + iterations_quantity, sizeof( unsigned char), text->array_len - iterations_quantity, stream_in) + iterations_quantity;
    text->additional = pattern->array_len - 1;
}

void update_hash_and_get(our_custom_array * text, const our_custom_array * pattern, const int flag )
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

void search_substring(our_custom_array * text, const our_custom_array * pattern, const int * total_index, FILE * stream_out)
{
    const int print_index = * total_index - pattern->array_len + 1;

    for( int pattern_work_index = 0, text_work_index = text->additional - pattern->array_len + 1; pattern_work_index < pattern->array_len; pattern_work_index++, text_work_index++)
    {
        fprintf(stream_out, "%d ", print_index + pattern_work_index);

        if( text->array[ text_work_index ] != pattern->array[ pattern_work_index ] )
        {
            break;
        }
    }
}

int rabin_carp_algorithm(const our_custom_array * pattern, FILE * stream_in)
{
    our_custom_array text = create_custom_array(K_max_chunk_len);
    if ( text.array == NULL)
    {
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 1;
    }

    FILE * stream_out = fopen("out.txt", "w");
    if (stream_out == NULL)
    {
        destroy_custom_array(&text);
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 1;
    }

    int total_index = pattern->array_len;
    text.additional = pattern->array_len - 1;
    text.array_len = (int)fread(text.array, sizeof (unsigned char ), K_max_chunk_len, stream_in);

    fprintf(stream_out, "%d ", pattern->hash);

    if (pattern->array_len <= text.array_len)
    {
        get_hash( &text, pattern);
    }

    while (pattern->array_len <= text.array_len)
    {
        if (pattern->hash == text.hash)
        {
            search_substring(&text, pattern, &total_index, stream_out);
        }

        total_index++;
        text.additional++;

        if ( text.additional >= text.array_len )
        {
            text_rewrite(&text, pattern, stream_in);
            update_hash_and_get(&text, pattern, 0);
        }
        else
        {
            update_hash_and_get(&text, pattern, 1);
        }
    }

    fclose(stream_out);
    destroy_custom_array(&text);

    return 0;
}

int main(void)
{
    FILE * stream_in = fopen("in.txt", "r");
    if (stream_in == NULL )
    {
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 0;
    }

    our_custom_array pattern = create_custom_array(K_max_pattern_len);
    if ( pattern.array == NULL)
    {
        fclose(stream_in );
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 0;
    }

    if ( scan_pattern(&pattern, stream_in) )
    {
        fclose(stream_in );
        destroy_custom_array( &pattern );
        fprintf( stderr, "__LINE__ %d\n", __LINE__);
        return 0;
    }
    pattern.additional = get_hash( &pattern, &pattern);

    rabin_carp_algorithm(&pattern, stream_in );

    fclose(stream_in );
    destroy_custom_array(&pattern);

    return 0;
}
