#include <cstdint>
#include <cstdbool>
#include <iostream>
#include <cstring>
#include <cstdint>
#include <string>

#define RED "\033[31m"
#define RESET "\033[0m"

#define VERIFY_CONTRACT(contract, _VA_ARGS__, ...) \
if (!(contract)) {                                 \
    printf(RED "%s" RESET, _VA_ARGS__);            \
    exit(1);                                       \
}

using namespace std::literals::string_literals;


class ZeroDivision : public std::exception {
public:
    const char* what() const noexcept override;
};

class Real {
private:
    char *value;
public:
    explicit Real (const char *str_num);
    ~Real() { delete[] value; }
    char *get_value() { return value; }

    Real& operator=(const Real&);
    char operator[](uint64_t index);

    friend Real operator+(const Real& lhs, const Real& rhs);
    friend Real operator-(const Real& lhs, const Real& rhs);
    friend Real operator*(const Real& lhs, const Real& rhs);
    friend Real operator/(const Real& lhs, const Real& rhs);


    friend Real& operator+=(Real& rhs, const Real& lhs);
    friend Real& operator-=(Real& rhs, const Real& lhs);
    friend Real& operator*=(Real& rhs, const Real& lhs);
    friend Real& operator/=(Real& rhs, const Real& lhs);


    friend bool operator>(const Real& lhs, const Real& rhs);
    friend bool operator>=(const Real& lhs, const Real& rhs);
    friend bool operator<(const Real& lhs, const Real& rhs);
    friend bool operator<=(const Real& lhs, const Real& rhs);
    friend bool operator==(const Real& lhs, const Real& rhs);
    friend bool operator!=(const Real& lhs, const Real& rhs);


//    static Real pi(uint64_t precision);
    static Real pow(const Real& exponent, uint64_t degree);
};



struct Bignum {
    char *value;
    uint64_t size;
    bool isnegative;
};

struct Oper_data {
    struct Bignum *bignum_1;
    struct Bignum *bignum_2;

    uint64_t aligned_size;
    uint64_t dot_index;
};

namespace Common_functions {
    struct Bignum *CreateBignum(const char *str_num);

    void DestroyBignum(struct Bignum *bignum);

    struct Oper_data *GetRightRotation(const char *str_num_1, const char *str_num_2);

    void DestroyOper_data(struct Oper_data *data);

    char *insert_the_dot(char *str_num, uint64_t dot_ind, uint64_t num_size);

    char *delete_zeros(char *str_num, uint64_t str_num_size);

    char *CreateStr(uint64_t size);

    char *ResImpl(char *res_of_call, uint64_t size, const char *operand_1, const char *operand_2, int8_t flag);

}

namespace Comparison {
    bool operator_eq(const char *operand_1, const char *operand_2);

    bool operator_neq(const char *operand_1, const char *operand_2);

    bool operator_more(const char *operand_1, const char *operand_2);

    bool operator_less(const char *operand_1, const char *operand_2);
}

char *add_operation(const char *operand_1, const char *operand_2);

char *subtraction_operation(const char *operand_1, const char *operand_2);

namespace Product {
    char *multiplication_operation(const char *operand_1, const char *operand_2);

    char *product_of_num_and_digit(const char *str_num, char digit, uint64_t str_num_size);

    char *align_num(const char *str_num, uint64_t str_num_size, uint64_t back_ind, uint64_t aligned_num_size);

    void iterative_addition(char *operand_1, const char *operand_2, uint64_t size);
}

namespace Division {
    char *division_operation(const char *operand_1, const char *operand_2);

    struct Oper_data *GetRightRotation_division(const char *operand_1, const char *operand_2);

    char *IntegerPart(struct Oper_data *data);
}


