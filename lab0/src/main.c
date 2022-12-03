#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int K_base_numbers[16] = {0, 1, 2,  3,  4,  5,  6,  7,
                                8, 9, 10, 11, 12, 13, 14, 15};
const int K_max_input_number_len = 13;
const int K_max_after_dot_numbers = 12;

const char *K_all_symbols = "0123456789abcdef";

typedef struct our_custom_array {
  char *array;
  int number_len;
  int dot_index;
  int fractional;
} our_custom_array;

our_custom_array make_custom_array(int array_len) {
  our_custom_array custom_array = {
      .array = malloc(sizeof(char) * array_len),
      .dot_index = 0,
      .number_len = 0,
      .fractional = 0,
  };
  return custom_array;
}

void destroy_custom_array(our_custom_array *custom_array) {
  free(custom_array->array);
  custom_array->array = NULL;
}

char *our_realloc(char *array, const int new_memory_size) {
  char *old_pointer = array;
  if ((array = realloc(array, new_memory_size)) == NULL){
    free(old_pointer);
    return NULL;
  }
  return array;
}

int scan_number(our_custom_array *custom_array, FILE *thread_in) {
  if (fscanf(thread_in, "%13s", custom_array->array) != 1) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }
  custom_array->number_len = (int)strlen(custom_array->array);
  return 0;
}

void make_lower_char(our_custom_array *custom_array) {
  for (int index = 0; index < (custom_array->number_len); index++) {
    custom_array->array[index] = (char)tolower(custom_array->array[index]);
  }
}

void update_dot_index(our_custom_array *custom_array) {
  int index = 0;
  for (; index < custom_array->number_len; index++) {
    if (custom_array->array[index] == '.') {
      custom_array->dot_index = index;
      return;
    }
  }
  custom_array->dot_index = index;
}

int get_necessary_number(const char symbol) {
  if (('0' <= symbol) && (symbol <= '9')) {
    return K_base_numbers[symbol - '0'];
  } else {
    return K_base_numbers[symbol - 'a' + 10];
  }
}

int is_symbol_less_base(const char symbol, const int K_base_from) {
  return (symbol <= '9') ? (('0' <= symbol) && (symbol < '0' + K_base_from)) : (('a' <= symbol) && (symbol < 'a' + K_base_from - 10));
}

int is_valid_char(const our_custom_array *custom_array, const int K_base_from) {
  if (custom_array->array[0] == '\0') {
    return 0;
  }

  if ((custom_array->dot_index == 0) || (custom_array->dot_index == custom_array->number_len - 1)) {
    return 0;
  }

  for (int index = 0; index < custom_array->dot_index; index++) {
    if (!is_symbol_less_base(custom_array->array[index], K_base_from)) {
      return 0;
    }
  }

  for (int index = custom_array->dot_index + 1; index < custom_array->number_len; index++) {
    if (!is_symbol_less_base(custom_array->array[index], K_base_from)) {
      return 0;
    }
  }

  return 1;
}

int is_valid_bases(const int K_base_from, const int K_base_to) {
  return (2 <= K_base_from && K_base_from <= 16 && 2 <= K_base_to && K_base_to <= 16);
}

void is_fractional(our_custom_array *custom_array) {
  for (int index = custom_array->dot_index + 1; index < custom_array->number_len; index++) {
    if (custom_array->array[index] != '0') {
      custom_array->fractional = 1;
      return;
    }
  }
  custom_array->fractional = 0;
}

long long int powlli(const long long int number, const int degree) {
  long long int result = 1;
  for (int iteration = 0; iteration < degree; iteration++, result *= number){
    ;
  }
  return result;
}

long long int convert_from_char_non_fractional_part(const our_custom_array *custom_array, const int K_base_from) {
  long long int non_fractional_result = 0;

  for (int front_index = 0, additional_index = custom_array->dot_index - 1; front_index < custom_array->dot_index; front_index++, additional_index--) {
    non_fractional_result += get_necessary_number(custom_array->array[front_index]) * powlli(K_base_from, additional_index);
  }
  return non_fractional_result;
}

double convert_from_char_fractional_part(const our_custom_array *custom_array, const int K_base_from) {
  double fractional_result = 0;

  for (int index = custom_array->dot_index + 1, degree = -1; index < custom_array->number_len; index++, degree--) {
    fractional_result += get_necessary_number(custom_array->array[index]) * pow(K_base_from, degree);
  }
  return fractional_result;
}

int convert_to_char(our_custom_array *custom_array, long long int non_fractional_part, double fractional_part, const int non_fractional_part_len, const int K_base_to) {
  const int result_len = (custom_array->fractional) ? (non_fractional_part_len + 1 + K_max_after_dot_numbers) : non_fractional_part_len;

  custom_array->array = our_realloc(custom_array->array, (result_len + 1) * (int)sizeof(char));
  if (custom_array->array == NULL){
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }

  for (int back_index = non_fractional_part_len - 1; back_index >= 0; back_index--) {
    custom_array->array[back_index] = K_all_symbols[non_fractional_part % K_base_to];
    non_fractional_part /= K_base_to;
  }

  custom_array->array[non_fractional_part_len] = '.';

  for (int index = non_fractional_part_len + 1; index < result_len; index++) {
    fractional_part *= K_base_to;
    custom_array->array[index] = K_all_symbols[(int)fractional_part];
    fractional_part -= (int)fractional_part;
  }

  custom_array->array[result_len] = '\0';
  return 0;
}

int get_non_fractional_part_len(const long long int non_fractional_result, const int K_base_to) {
  int non_fractional_result_len = 0;
  do {
    non_fractional_result_len++;
  } while (non_fractional_result >= powlli(K_base_to, non_fractional_result_len));

  return non_fractional_result_len;
}

int convert_number(our_custom_array *custom_array, const int K_base_from, const int K_base_to) {
  const long long int non_fractional_part = convert_from_char_non_fractional_part(custom_array, K_base_from);
  return convert_to_char(custom_array, non_fractional_part,convert_from_char_fractional_part(custom_array, K_base_from),get_non_fractional_part_len(non_fractional_part, K_base_to), K_base_to);
}

int print_converted_number(const our_custom_array *custom_array, int flag) {
  FILE *thread_out = fopen("out.txt", "w");
  if (thread_out == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 1;
  }

  if (flag) {
    fprintf(thread_out, "bad input\n");
  } else {
    fprintf(thread_out, "%s\n", custom_array->array);
  }

  fclose(thread_out);
  return 0;
}

int main(void) {
  int base_from, base_to;

  FILE *thread_in = fopen("in.txt", "r");
  if (thread_in == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    return 0;
  }

  if (2 != fscanf(thread_in, "%d%d", &base_from, &base_to)) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    fclose(thread_in);
    return 0;
  }

  our_custom_array custom_array = make_custom_array(K_max_input_number_len + 1);
  if (custom_array.array == NULL) {
    fprintf(stderr, "__LINE__ %d\n", __LINE__);
    fclose(thread_in);
    return 0;
  }

  if (scan_number(&custom_array, thread_in)) {
    destroy_custom_array(&custom_array);
    fclose(thread_in);
    return 0;
  }

  fclose(thread_in);

  make_lower_char(&custom_array);
  update_dot_index(&custom_array);
  is_fractional(&custom_array);

  if (is_valid_bases(base_from, base_to) && is_valid_char(&custom_array, base_from)) {
    if (convert_number(&custom_array, base_from, base_to)) {
      destroy_custom_array(&custom_array);
      return 0;
    }
    print_converted_number(&custom_array, 0);
  } else {
    print_converted_number(&custom_array, 1);
  }

  destroy_custom_array(&custom_array);

  return 0;
}

