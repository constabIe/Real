#include "../include/Real.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>

#define MAX(a, b) ((a > b) ? a : b)
#define PRECISION 100

#define RED "\033[31m"
#define RESET "\033[0m"

#define VERIFY_CONTRACT(contract, _VA_ARGS__, ...) \
if (!(contract)) {                                 \
    printf(RED "%s" RESET, _VA_ARGS__);            \
    exit(1);                                       \
}                                                  \

using namespace Common_functions;
using namespace Division;
using namespace Product;
using namespace Comparison;


struct Bignum *Common_functions::CreateBignum(const char *str_num) {
    uint64_t size = strlen(str_num);
    auto new_bignum = (struct Bignum *) malloc(sizeof(struct Bignum));

    bool flag = str_num[0] == '-';

    if (flag) {
        new_bignum->value = (char *) malloc(size * sizeof(char));
        VERIFY_CONTRACT(new_bignum->value != nullptr, "\n[CreateBignum] Unable to allocate memory\n")
        new_bignum->value[size - 1] = '\0';

        for (uint64_t i = 1; i < size; ++i) {
            new_bignum->value[i - 1] = str_num[i];
        }

        new_bignum->size = size - 1;
        new_bignum->isnegative = true;
    } else {
        new_bignum->value = (char *) malloc((size + 1) * sizeof(char));
        VERIFY_CONTRACT(new_bignum->value != nullptr, "\n[CreateBignum] Unable to allocate memory\n")
        new_bignum->value[size] = '\0';

        strcpy(new_bignum->value, str_num);

        new_bignum->size = size;
        new_bignum->isnegative = false;
    }

    return new_bignum;
}
void Common_functions::DestroyBignum(struct Bignum *bignum) {
    free(bignum->value);
    free(bignum);
}

struct Oper_data *Common_functions::GetRightRotation(const char *str_num_1, const char *str_num_2) {
    auto bignum_1 = Common_functions::CreateBignum(str_num_1);
    auto bignum_2 = Common_functions::CreateBignum(str_num_2);

    bignum_1->value = Common_functions::delete_zeros(bignum_1->value, bignum_1->size);
    bignum_2->value = Common_functions::delete_zeros(bignum_2->value, bignum_2->size);

    bignum_1->size = strlen(bignum_1->value);
    bignum_2->size = strlen(bignum_2->value);

    uint64_t dot_ind_1 = bignum_1->size,
            dot_ind_2 = bignum_2->size;

    for (uint64_t i = 0; i < bignum_1->size; ++i) {
        if (bignum_1->value[i] == '.') {
            dot_ind_1 = i;
            break;
        }
    }
    for (uint64_t i = 0; i < bignum_2->size; ++i) {
        if (bignum_2->value[i] == '.') {
            dot_ind_2 = i;
            break;
        }
    }

    uint64_t max_dot_ind = MAX(dot_ind_1, dot_ind_2);

    char *operand_1 = (char *) malloc(max_dot_ind * sizeof(char)),
            *operand_2 = (char *) malloc(max_dot_ind * sizeof(char));
    VERIFY_CONTRACT(operand_1 != nullptr && operand_2 != nullptr, "\n[GetRightRotation] Unable to allocate memory\n")

    memset(operand_1, '0', max_dot_ind);
    memset(operand_2, '0', max_dot_ind);

    uint64_t ind;

    ind = dot_ind_1 - 1;
    for (uint64_t i = max_dot_ind - 1; i >= max_dot_ind - dot_ind_1 && i != UINT64_MAX; --i) {
        operand_1[i] = bignum_1->value[ind];
        if (ind - 1 != UINT64_MAX) {
            ind--;
        }
    }
    ind = dot_ind_2 - 1;
    for (uint64_t i = max_dot_ind - 1; i >= max_dot_ind - dot_ind_2 && i != UINT64_MAX; --i) {
        operand_2[i] = bignum_2->value[ind];
        if (ind - 1 != UINT64_MAX) {
            ind--;
        }
    }

    uint64_t size_1_right = 0;
    uint64_t size_2_right = 0;
    for (uint64_t i = dot_ind_1 + 1; i < bignum_1->size; ++i) {
        size_1_right += 1;
    }
    for (uint64_t i = dot_ind_2 + 1; i < bignum_2->size; ++i) {
        size_2_right += 1;
    }
    uint64_t max_size_right = MAX(size_1_right, size_2_right);

    operand_1 = (char *) realloc(operand_1, max_dot_ind + ((max_size_right + 1) * sizeof(char)));
    operand_2 = (char *) realloc(operand_2, max_dot_ind + ((max_size_right + 1) * sizeof(char)));
    VERIFY_CONTRACT(operand_1 != nullptr && operand_2 != nullptr, "\n[GetRightRotation] Unable to allocate memory\n")

    operand_1[max_size_right + max_dot_ind] = '\0';
    operand_2[max_size_right + max_dot_ind] = '\0';

    for (uint64_t i = max_dot_ind; i < max_dot_ind + max_size_right; ++i) {
        operand_1[i] = '0';
        operand_2[i] = '0';
    }

    ind = dot_ind_1 + 1;
    for (uint64_t i = max_dot_ind; i <= max_dot_ind + size_1_right - 1; ++i) {
        operand_1[i] = bignum_1->value[ind];
        ind++;
    }

    ind = dot_ind_2 + 1;
    for (uint64_t i = max_dot_ind; i <= max_dot_ind + size_2_right - 1; ++i) {
        operand_2[i] = bignum_2->value[ind];
        ind++;
    }

    auto data = (struct Oper_data *) malloc(sizeof(struct Oper_data));

    data->bignum_1 = (struct Bignum *) malloc(sizeof(struct Bignum));
    VERIFY_CONTRACT(data->bignum_1 != nullptr, "\n[GetRightRotation] Unable to allocate memory\n")
    data->bignum_1->value = operand_1;
    data->bignum_1->size = 0;
    data->bignum_1->isnegative = bignum_1->isnegative;

    data->bignum_2 = (struct Bignum *) malloc(sizeof(struct Bignum));
    VERIFY_CONTRACT(data->bignum_2 != nullptr, "\n[GetRightRotation] Unable to allocate memory\n")
    data->bignum_2->value = operand_2;
    data->bignum_2->size = 0;
    data->bignum_2->isnegative = bignum_2->isnegative;

    data->aligned_size = max_size_right + max_dot_ind;
    data->dot_index = max_dot_ind;

    Common_functions::DestroyBignum(bignum_1);
    Common_functions::DestroyBignum(bignum_2);

    return data;
}
void Common_functions::DestroyOper_data(struct Oper_data *data) {
    free(data->bignum_1->value);
    free(data->bignum_2->value);

    free(data->bignum_1);
    free(data->bignum_2);

    free(data);
}

char *Common_functions::insert_the_dot(char *str_num, uint64_t dot_ind, uint64_t num_size) {
    char *res = nullptr;

    if (dot_ind == num_size) {
        res = CreateStr(num_size);
        strcpy(res, str_num);
    } else {
        res = CreateStr(num_size + 1);
        strcpy(res, str_num);

        for (uint64_t i = dot_ind; i < num_size; ++i) {
            char ptr = str_num[i + 1];

            res[i + 1] = str_num[i];
            res[i + 2] = ptr;
        }

        res[dot_ind] = '.';
    }

    free(str_num);

    return res;
}
char *Common_functions::delete_zeros(char *str_num, uint64_t str_num_size) {
    uint64_t dot_ind = str_num_size;

    for (uint64_t i = 0; i < str_num_size; ++i) {
        if (str_num[i] == '.') {
            dot_ind = i;
            break;
        }
    }

    uint64_t right_ind = 0;
    uint64_t left_ind = str_num_size - 1;

    for (uint64_t i = 0; i < dot_ind - 1; ++i) {
        if (str_num[i] != '0') {
            break;
        } else {
            right_ind = i + 1;
        }
    }

    for (uint64_t i = str_num_size - 1; i >= dot_ind + 1 ; --i) {
        if (str_num[i] != '0') {
            break;
        } else{
            left_ind = i - 1;
        }
    }

    char *res = CreateStr((left_ind - right_ind + 1 == 0) ? 1 : left_ind - right_ind + 1);

    uint64_t ind_res = 0;
    for (uint64_t i = right_ind; i <= left_ind; ++i) {
        res[ind_res++] = str_num[i];
    }

    free(str_num);

    return res;
}
char *Common_functions::CreateStr(uint64_t size) {
    char *res = (char *) malloc((size + 1) * sizeof(char));
    VERIFY_CONTRACT(res != nullptr, "\n[CreateStr] Unable to allocate memory\n")
    res[size] = '\0';

    memset(res, '0', size);

    return res;
}
char *Common_functions::ResImpl(char *res_of_call, uint64_t size, const char *operand_1, const char *operand_2, int8_t flag) {
    char *res = nullptr;

    char sign = '+';
    if (flag == 1) {
        if (operand_1[0] == '-' && operand_2[0] == '-') {
            sign = '-';
        }
    } else if (flag == 2) {
        if ((operand_1[0] != '-' && operand_2[0] == '-') ||
            (operand_1[0] == '-' && operand_2[0] != '-'))  {
            sign = '-';
        }
    } else if (flag == 3) {
        if ((operand_1[0] == '-' && operand_2[0] == '-' && Comparison::operator_less(operand_1, operand_2)) ||
            (operand_1[0] != '-' && operand_2[0] != '-' && Comparison::operator_less(operand_1, operand_2))) {
            sign = '-';
        }
    }

    if (sign == '+') {
        res = Common_functions::CreateStr(size);
        strcpy(res, res_of_call);
    } else {
        res = Common_functions::CreateStr(size + 1);
        res[0] = '-';
        for (uint64_t i = 1; i < size + 1; ++i) {
            res[i] = res_of_call[i - 1];
        }
    }

    free(res_of_call);

    return res;
}


bool Comparison::operator_eq(const char *operand_1, const char *operand_2) {
    struct Oper_data *data = Common_functions::GetRightRotation(operand_1, operand_2);

    bool flag = false;
    if (data->bignum_1->isnegative == data->bignum_2->isnegative) {
        for (uint64_t i = 0; i < data->aligned_size; ++i) {
            if (data->bignum_1->value[i] == data->bignum_2->value[i]) {
                flag = true;
            } else {
                flag = false;
                break;
            }
        }
    }

    Common_functions::DestroyOper_data(data);

    return flag;
}
bool Comparison::operator_neq(const char *operand_1, const char *operand_2) {
    struct Oper_data *data = Common_functions::GetRightRotation(operand_1, operand_2);

    bool flag = false;
    if (data->bignum_1->isnegative == data->bignum_2->isnegative) {
        for (uint64_t i = 0; i < data->aligned_size; ++i) {
            if (data->bignum_1->value[i] == data->bignum_2->value[i]) {
                flag = false;
            } else {
                flag = true;
                break;
            }
        }
    }

    Common_functions::DestroyOper_data(data);

    return flag;
}
bool Comparison::operator_more(const char *operand_1, const char *operand_2) {
    struct Oper_data *data = Common_functions::GetRightRotation(operand_1, operand_2);

    bool flag = false;
    if (data->bignum_1->isnegative == data->bignum_2->isnegative) {
        if (!data->bignum_1->isnegative) {
            for (uint64_t i = 0; i < data->aligned_size; ++i) {
                if (data->bignum_1->value[i] == data->bignum_2->value[i]) {
                    continue;
                } else if (data->bignum_1->value[i] < data->bignum_2->value[i]) {
                    break;
                } else if ((data->bignum_1->value[i] > data->bignum_2->value[i])) {
                    flag = true;
                    break;
                }
            }
        } else if (data->bignum_1->isnegative) {
            for (uint64_t i = 0; i < data->aligned_size; ++i) {
                if (data->bignum_1->value[i] == data->bignum_2->value[i]) {
                    continue;
                } else if (data->bignum_1->value[i] < data->bignum_2->value[i]) {
                    flag = true;
                    break;
                } else if ((data->bignum_1->value[i] > data->bignum_2->value[i])) {
                    break;
                }
            }
        }
    } else if (!data->bignum_1->isnegative && data->bignum_2->isnegative) {
        flag = true;
    } else if (data->bignum_1->isnegative && !data->bignum_2->isnegative) {
        flag = false;
    }

    Common_functions::DestroyOper_data(data);

    return flag;
}
bool Comparison::operator_less(const char *operand_1, const char *operand_2) {
    struct Oper_data *data = Common_functions::GetRightRotation(operand_1, operand_2);

    bool flag = false;
    if (data->bignum_1->isnegative == data->bignum_2->isnegative) {
        if (!data->bignum_1->isnegative) {
            for (uint64_t i = 0; i < data->aligned_size; ++i) {
                if (data->bignum_1->value[i] == data->bignum_2->value[i]) {
                    continue;
                } else if (data->bignum_1->value[i] < data->bignum_2->value[i]) {
                    flag = true;
                    break;
                } else if ((data->bignum_1->value[i] > data->bignum_2->value[i])) {
                    break;
                }
            }
        } else if (data->bignum_1->isnegative) {
            for (uint64_t i = 0; i < data->aligned_size; ++i) {
                if (data->bignum_1->value[i] == data->bignum_2->value[i]) {
                    continue;
                } else if (data->bignum_1->value[i] < data->bignum_2->value[i]) {
                    break;
                } else if ((data->bignum_1->value[i] > data->bignum_2->value[i])) {
                    flag = true;
                    break;
                }
            }
        }
    } else if (!data->bignum_1->isnegative &&  data->bignum_2->isnegative) {
        flag = true;
    } else if (data->bignum_1->isnegative && !data->bignum_2->isnegative) {
        flag = false;
    }

    Common_functions::DestroyOper_data(data);

    return flag;
}


char *add_operation(const char *operand_1, const char *operand_2) {
    if (Comparison::operator_eq(operand_1, "0") && Comparison::operator_eq(operand_2, "0")) {
        char *res = Common_functions::CreateStr(1);

        return res;
    }

    if (operand_1[0] == '-' && operand_2[0] != '-') {
        uint64_t size_operand_2 = strlen(operand_2);
        char *dummy_operand_2 = Common_functions::CreateStr(size_operand_2 + 1);
        for (uint64_t i = 1; i < size_operand_2 + 1; ++i) {
            dummy_operand_2[i] = operand_2[i - 1];
        }
        dummy_operand_2[0] = '-';

        char *res = subtraction_operation(operand_1, dummy_operand_2);

        free(dummy_operand_2);

        return res;
    } else if (operand_1[0] != '-' && operand_2[0] == '-') {
        uint64_t size_operand_2 = strlen(operand_2);
        char *dummy_operand_2 = Common_functions::CreateStr(size_operand_2 - 1);
        for (uint64_t i = 0; i < size_operand_2; ++i) {
            dummy_operand_2[i] = operand_2[i + 1];
        }

        char *res = subtraction_operation(operand_1, dummy_operand_2);

        free(dummy_operand_2);

        return res;
    }

    auto data = Common_functions::GetRightRotation(operand_1, operand_2);

    char *help_str = Common_functions::CreateStr(data->aligned_size + 1);
    char *res_of_add = Common_functions::CreateStr(data->aligned_size + 1);

    int8_t colsum = 0;
    for (uint64_t i = data->aligned_size - 1; i != UINT64_MAX; --i) {
        colsum = (data->bignum_1->value[i] - '0') + (data->bignum_2->value[i] - '0') + (help_str[i + 1] - '0');

        help_str[i] = ((colsum - (colsum % 10)) / 10) + '0';
        res_of_add[i + 1] = (colsum % 10) + '0';
    }
    res_of_add[0] = ((colsum - (colsum % 10)) / 10) + '0';

    res_of_add = Common_functions::insert_the_dot(res_of_add, data->dot_index + 1, data->aligned_size + 1);
    res_of_add = Common_functions::delete_zeros(res_of_add, data->aligned_size + 2);
    res_of_add = Common_functions::ResImpl(res_of_add, strlen(res_of_add), operand_1, operand_2, 1);

    free(help_str);
    Common_functions::DestroyOper_data(data);

    return res_of_add;
}
char *subtraction_operation(const char *operand_1, const char *operand_2) {
    if (Comparison::operator_eq(operand_1, operand_2)) {
        char *res = Common_functions::CreateStr(1);

        return res;
    }

    if (operand_1[0] == '-' && operand_2[0] != '-') {
        uint64_t size_operand_2 = strlen(operand_2);
        char *dummy_operand_2 = Common_functions::CreateStr(size_operand_2 + 1);
        for (uint64_t i = 1; i < size_operand_2 + 1; ++i) {
            dummy_operand_2[i] = operand_2[i - 1];
        }
        dummy_operand_2[0] = '-';

        char *res = add_operation(operand_1, dummy_operand_2);

        free(dummy_operand_2);

        return res;
    } else if (operand_1[0] != '-' && operand_2[0] == '-') {
        uint64_t size_operand_2 = strlen(operand_2);
        char *dummy_operand_2 = Common_functions::CreateStr(size_operand_2 - 1);
        for (uint64_t i = 0; i < size_operand_2; ++i) {
            dummy_operand_2[i] = operand_2[i + 1];
        }

        char *res = add_operation(operand_1, dummy_operand_2);

        free(dummy_operand_2);

        return res;
    }

    auto bignum_1 = Common_functions::CreateBignum(operand_1);
    auto bignum_2 = Common_functions::CreateBignum(operand_2);

    bool flag = Comparison::operator_more(bignum_1->value, bignum_2->value);

    Common_functions::DestroyBignum(bignum_1);
    Common_functions::DestroyBignum(bignum_2);


    auto data = (flag) ? Common_functions::GetRightRotation(operand_1, operand_2) : Common_functions::GetRightRotation(operand_2, operand_1);

    char *dummy_operand_1 = data->bignum_1->value;
    char *help_str = Common_functions::CreateStr(data->aligned_size);
    char *res_of_subtraction = Common_functions::CreateStr(data->aligned_size);

    int8_t substr = 0;
    for (uint64_t i = data->aligned_size - 1; i != UINT64_MAX ; --i) {
        substr = (help_str[i] - '0') + (dummy_operand_1[i] - '0') - (data->bignum_2->value[i] - '0');
        if (substr < 0) {
            for (uint64_t j = i - 1; j != UINT64_MAX ; --j) {
                if (dummy_operand_1[i] == '0') {
                    dummy_operand_1[i] = '9';
                }
                if (dummy_operand_1[i] != '0') {
                    dummy_operand_1[i] -= 1;
                    break;
                }
            }
            substr += 10;
        }
        res_of_subtraction[i] = (substr % 10) + '0';
        if (i - 1 != UINT64_MAX) {
            help_str[i - 1] = ((substr - (substr % 10)) / 10) + '0';
        }
    }
    res_of_subtraction = Common_functions::insert_the_dot(res_of_subtraction, data->dot_index, data->aligned_size);
    res_of_subtraction = Common_functions::delete_zeros(res_of_subtraction, data->aligned_size + 1);
    res_of_subtraction = Common_functions::ResImpl(res_of_subtraction, strlen(res_of_subtraction), operand_1, operand_2, 3);

    free(help_str);
    free(dummy_operand_1);

    return res_of_subtraction;
}


char *Product::multiplication_operation(const char *operand_1, const char *operand_2) {
    char *res_of_multiplication = nullptr;

    if (Comparison::operator_eq(operand_1, "0") || Comparison::operator_eq(operand_2, "0")) {
        res_of_multiplication = Common_functions::CreateStr(1);

        return res_of_multiplication;
    }

    auto data = Common_functions::GetRightRotation(operand_1, operand_2);

    res_of_multiplication = Common_functions::CreateStr(2 * data->aligned_size);
    uint64_t back_ind = 2 * data->aligned_size - 1;

    char *product_num_digit = nullptr;
    char *aligned_num = nullptr;

    for (uint64_t i = data->aligned_size - 1; i != UINT64_MAX; --i) {
        if (data->bignum_2->value[i] != '0') {
            product_num_digit = Product::product_of_num_and_digit(data->bignum_1->value, data->bignum_2->value[i], data->aligned_size);
            aligned_num = Product::align_num(product_num_digit, data->aligned_size + 1, back_ind, 2 * data->aligned_size);
            Product::iterative_addition(res_of_multiplication, aligned_num, 2 * data->aligned_size);

            free(product_num_digit);
            free(aligned_num);
        }
        back_ind--;
    }

    res_of_multiplication = Common_functions::insert_the_dot(res_of_multiplication, 2 * data->dot_index, 2 * data->aligned_size);
    res_of_multiplication = Common_functions::delete_zeros(res_of_multiplication, 2 * data->aligned_size + 1);
    res_of_multiplication = Common_functions::ResImpl(res_of_multiplication, strlen(res_of_multiplication), operand_1, operand_2, 2);

    return res_of_multiplication;
}
char *Product::product_of_num_and_digit(const char *str_num, char digit, uint64_t str_num_size) {
    char *res = Common_functions::CreateStr(str_num_size + 1);
    char *interim_calculations = Common_functions::CreateStr(str_num_size + 1);

    int digits_product = 0;
    for (uint64_t i = str_num_size - 1; i != UINT64_MAX ; --i) {
        digits_product = (interim_calculations[i + 1] - '0') + (str_num[i] - '0') * (digit - '0');

        interim_calculations[i] = ((digits_product - digits_product % 10) / 10) + '0';
        digits_product = digits_product % 10;

        res[i + 1] = digits_product + '0';
    }

    res[0] = interim_calculations[0];

    free(interim_calculations);

    return res;
}
char *Product::align_num(const char *str_num, uint64_t str_num_size, uint64_t back_ind, uint64_t aligned_num_size) {
    char *res = Common_functions::CreateStr(aligned_num_size);

    uint64_t ind = str_num_size - 1;
    for (uint64_t i = back_ind; i != UINT64_MAX; --i) {
        if (ind != UINT64_MAX) {
            res[i] = str_num[ind];
            ind--;
        } else {
            break;
        }
    }

    return res;
}
void Product::iterative_addition(char *operand_1, const char *operand_2, uint64_t size) {
    char *add = add_operation(operand_1, operand_2);
    memset(operand_1, '0', size);
    uint64_t ind = size - 1;
    for (uint64_t i = strlen(add) - 1; i != UINT64_MAX; --i) {
        operand_1[ind--] = add[i];
    }
    free(add);
}


char *Division::division_operation(const char *operand_1, const char *operand_2) {
    if (operator_eq(operand_2, "0")) {
        throw ZeroDivision {};
    }

    char *res_of_division = nullptr;
    auto data = GetRightRotation_division(operand_1, operand_2);

    char *integer_part = IntegerPart(data);
    size_t size_integer_part = strlen(integer_part);

    res_of_division = CreateStr(size_integer_part + 1);
    strcpy(res_of_division, integer_part);

    res_of_division[size_integer_part] = '.';
    size_t size_res_of_division = size_integer_part + 1;


    char *interim_product;
    bool break_flag = false;
    for (uint64_t i = size_integer_part + 1; i < size_integer_part + 1 + PRECISION; ++i) {
        res_of_division = (char *) realloc(res_of_division, size_res_of_division + 2);
        res_of_division[size_res_of_division+1] = '\0';
        size_res_of_division++;

        if (!break_flag) {
            for (char j = '0'; j <= '9'; ++j) {
                res_of_division[i] = (char) j;
                interim_product = multiplication_operation(data->bignum_2->value, res_of_division);

                if (operator_more(interim_product, data->bignum_1->value)) {
                    if (j != '0') {
                        res_of_division[i] = j - 1;
                    }

                    free(interim_product);
                    break;
                }

                if (operator_eq(interim_product, data->bignum_1->value)) {
                    break_flag = true;

                    free(interim_product);
                    break;
                }

                free(interim_product);
            }
        } else {
            break;
        }
    }

    res_of_division = delete_zeros(res_of_division, size_res_of_division);
    res_of_division = Common_functions::ResImpl(res_of_division, strlen(res_of_division), operand_1, operand_2, 2);

    free(integer_part);

    return res_of_division;
}
struct Oper_data *Division::GetRightRotation_division(const char *str_num_1, const char *str_num_2) {
    auto bignum_1 = Common_functions::CreateBignum(str_num_1);
    auto bignum_2 = Common_functions::CreateBignum(str_num_2);

    bignum_1->value = Common_functions::delete_zeros(bignum_1->value, bignum_1->size);
    bignum_2->value = Common_functions::delete_zeros(bignum_2->value, bignum_2->size);

    bignum_1->size = strlen(bignum_1->value);
    bignum_2->size = strlen(bignum_2->value);

    uint64_t dot_ind_1 = bignum_1->size,
            dot_ind_2 = bignum_2->size;


    for (uint64_t i = 0; i < bignum_1->size; ++i) {
        if (bignum_1->value[i] == '.') {
            dot_ind_1 = i;
            break;
        }
    }

    for (uint64_t i = 0; i < bignum_2->size; ++i) {
        if (bignum_2->value[i] == '.') {
            dot_ind_2 = i;
            break;
        }
    }

    uint64_t shift = (bignum_2->size == dot_ind_2) ? 0 : (bignum_2->size - dot_ind_2 - 1);
    uint64_t capacity = (shift - ((bignum_1->size == dot_ind_1) ? 0 : bignum_1->size - dot_ind_1 - 1) > UINT32_MAX) ? 0 :
                        shift - ((bignum_1->size == dot_ind_1) ? 0 : bignum_1->size - dot_ind_1 - 1);

    bool dot_flag_str_num_1 = bignum_1->size != dot_ind_1,
            dot_flag_str_num_2 = bignum_2->size != dot_ind_2;


    char *dummy_operand_1 = Common_functions::CreateStr(bignum_1->size - dot_flag_str_num_1 + capacity);
    char *dummy_operand_2 = Common_functions::CreateStr(bignum_2->size - dot_flag_str_num_2);

    uint64_t ind = 0;
    for (uint64_t i = 0; i < bignum_1->size; ++i) {
        if (bignum_1->value[i] != '.') {
            dummy_operand_1[ind++] = bignum_1->value[i];
        }
    }

    ind = 0;
    for (uint64_t i = 0; i < bignum_2->size; ++i) {
        if (bignum_2->value[i] != '.') {
            dummy_operand_2[ind++] = bignum_2->value[i];
        }
    }


    auto data = (struct Oper_data *) malloc(sizeof(struct Oper_data));


    data->bignum_1 = (struct Bignum *) malloc(sizeof(struct Bignum));

    bool flag_zero_1 = dummy_operand_1[0] == '0';
    data->bignum_1->value = (flag_zero_1) ? Common_functions::delete_zeros(dummy_operand_1,
                                                                           bignum_1->size - dot_flag_str_num_1 + capacity) : dummy_operand_1;

    data->bignum_1->size = (flag_zero_1) ? bignum_1->size - dot_flag_str_num_1 + capacity - 1 : bignum_1->size - dot_flag_str_num_1 + capacity;
    if (Comparison::operator_eq(data->bignum_1->value, "0")) {
        data->bignum_1->size = 1;
    }

    data->bignum_1->isnegative = bignum_1->isnegative;


    data->bignum_2 = (struct Bignum *) malloc(sizeof(struct Bignum));

    bool flag_zero_2 = dummy_operand_2[0] == '0';
    data->bignum_2->value = (flag_zero_2) ? Common_functions::delete_zeros(dummy_operand_2,
                                                                           bignum_2->size - dot_flag_str_num_2) : dummy_operand_2;

    data->bignum_2->size = (flag_zero_2) ? bignum_2->size - dot_flag_str_num_2 - 1 : bignum_2->size - dot_flag_str_num_2;
    if (Comparison::operator_eq(data->bignum_2->value, "0")) {
        data->bignum_2->size = 1;
    }

    data->bignum_2->isnegative = bignum_2->isnegative;


    data->aligned_size = 0;
    data->dot_index = (flag_zero_1) ? dot_ind_1 + shift - 1: dot_ind_1 + shift;

    char *op_1 = CreateStr(data->bignum_1->size + 1);
    strncpy(op_1, data->bignum_1->value, data->dot_index);
    op_1[data->dot_index] = '.';
    for (uint64_t i = data->dot_index; i < data->bignum_1->size; ++i) {
        op_1[i + 1] = data->bignum_1->value[i];
    }

    free(data->bignum_1->value);
    data->bignum_1->value = op_1;

    Common_functions::DestroyBignum(bignum_1);
    Common_functions::DestroyBignum(bignum_2);

    return data;
}
char *Division::IntegerPart(struct Oper_data *data) {
    uint64_t digits_quantity = data->dot_index - data->bignum_2->size;

    char *res = nullptr;
    size_t size_res;
    char *interim_products;

    if (digits_quantity == 0 || digits_quantity > UINT32_MAX) {
        res = Common_functions::CreateStr(1);
        size_res = 1;
    } else {
        res = Common_functions::CreateStr(digits_quantity);
        size_res = digits_quantity;

        interim_products = multiplication_operation(res, data->bignum_2->value);
        for (uint64_t i = size_res; i >= 1; --i) {
            if (operator_less(interim_products, data->bignum_2->value)) {
                size_res = i;
                break;
            } else {
                res = (char *) realloc(res, (i + 1) * sizeof(char));
            }
        }
    }

    bool break_flag = false;
    for (uint64_t i = 0; i < size_res; ++i) {
        if (!break_flag) {
            for (char j = '0'; j <= '9'; ++j) {
                res[i] = j;
                interim_products = multiplication_operation(data->bignum_2->value, res);

                if (operator_more(interim_products, data->bignum_1->value)) {
                    if (j != '0') {
                        res[i] = j - 1;
                    }

                    free(interim_products);
                    break;
                }

                if (operator_eq(interim_products, data->bignum_1->value)) {
                    break_flag = true;

                    free(interim_products);
                    break;
                }

                free(interim_products);
            }
        } else {
            break;
        }
    }

    return res;
}


Real::Real(const char *str_num) {
    size_t size = strlen(str_num);
    value = new char[size + 1];
    strcpy(value, str_num);
}

Real &Real::Real::operator=(const Real &other) {
    if (this != &other) {
        if (value != nullptr) {
            delete[] value;
        }
        if (other.value != nullptr) {
            value = new char[strlen(other.value) + 1];
            strcpy(value, other.value);
        } else {
            value = nullptr;
        }
    }
    return *this;
}
char Real::operator[](uint64_t index) {
    return this->value[index];
}


Real operator+(const Real& lhs, const Real& rhs) {
    Real res("1");
    free(res.value);

    res.value = add_operation(lhs.value, rhs.value);

    return res;
}
Real operator-(const Real& lhs, const Real& rhs) {
    Real res("1");
    free(res.value);

    res.value = subtraction_operation(lhs.value, rhs.value);

    return res;
}
Real operator*(const Real& lhs, const Real& rhs) {
    Real res("1");
    free(res.value);

    res.value = Product::multiplication_operation(lhs.value, rhs.value);

    return res;
}
Real operator/(const Real& lhs, const Real& rhs) {
    Real res("1");
    free(res.value);

    res.value = Division::division_operation(lhs.value, rhs.value);

    return res;
}


Real& operator+=(Real& lhs, const Real& rhs) {
    char *res = add_operation(lhs.value, rhs.value);
    free(lhs.value);
    lhs.value = res;
    return lhs;
}
Real& operator-=(Real& lhs, const Real& rhs) {
    char *res = subtraction_operation(lhs.value, rhs.value);
    free(lhs.value);
    lhs.value = res;
    return lhs;
}
Real& operator*=(Real& lhs, const Real& rhs) {
    char *res = multiplication_operation(lhs.value, rhs.value);
    free(lhs.value);
    lhs.value = res;
    return lhs;
}
Real& operator/=(Real& lhs, const Real& rhs) {
    char *res = division_operation(lhs.value, rhs.value);
    free(lhs.value);
    lhs.value = res;
    return lhs;
}


bool operator>(const Real& lhs, const Real& rhs) {
    return operator_more(lhs.value, rhs.value);
}
bool operator>=(const Real& lhs, const Real& rhs) {
    return operator_eq(lhs.value, rhs.value) || operator_more(lhs.value, rhs.value);
}
bool operator<(const Real& lhs, const Real& rhs) {
    return operator_less(lhs.value, rhs.value);
}
bool operator<=(const Real& lhs, const Real& rhs) {
    return operator_eq(lhs.value, rhs.value) || operator_less(lhs.value, rhs.value);
}
bool operator==(const Real& lhs, const Real& rhs) {
    return operator_eq(lhs.value, rhs.value);
}
bool operator!=(const Real& lhs, const Real& rhs) {
    return operator_neq(lhs.value, rhs.value);
}

Real Real::pow(const Real &exponent, uint64_t degree) {
    Real res(exponent.value);

    if (degree == 0) {
        res = Real("1");
        return res;
    }

    for (uint64_t i = 0; i < degree - 1; ++i) {
        res *= exponent;
    }

    return res;
}


const char* ZeroDivision::what() const noexcept {
    return "unpredictable result with zero divisor";
}



