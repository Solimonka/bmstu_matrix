#include <gtest/gtest.h>
#include <string>
#include "bmstu_matrix.h"

TEST(MatrixTest, BasicConstructor) {
    bmstu::matrix<int> arr2d;
    ASSERT_EQ(arr2d(0, 0), 0);
    ASSERT_THROW(bmstu::matrix<std::string> arr2d_, std::logic_error);
}

TEST(MatrixTest, ConstructorWithParametr) {
    bmstu::matrix<int> arr2d(2, 3);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(arr2d(i, j), 0);
        }
    }
    ASSERT_THROW(bmstu::matrix<std::string> arr2d_(10, 890), std::logic_error);
}

TEST(MatrixTest, InitializerList) {
    bmstu::matrix<int> arr2d({1, 2, 3, 11, 22, 33}, 2, 3);
    ASSERT_EQ(arr2d(0, 0), 1);
    ASSERT_EQ(arr2d(0, 1), 2);
    ASSERT_EQ(arr2d(0, 2), 3);
    ASSERT_EQ(arr2d(1, 0), 11);
    ASSERT_EQ(arr2d(1, 1), 22);
    ASSERT_EQ(arr2d(1, 2), 33);
    ASSERT_THROW(bmstu::matrix<std::string> arr2d({"ya", "ne", "vse", "spisivau", "u", "malchikov"}, 2, 3),
                 std::logic_error);
    ASSERT_THROW(bmstu::matrix<char> arr2d_({'a', 'b', 'c', 'd', 'e'}, 2, 3), std::out_of_range);
}

TEST(MatrixTest, Output) {
    bmstu::matrix<int> arr2d({1, 2, 3, 11, 22, 33}, 2, 3);
    std::stringstream s;
    s << arr2d;
    ASSERT_STREQ(s.str().c_str(), "1 2 3 \r\n11 22 33 \r\n");
}

TEST(MatrixTest, Subscript) {
    bmstu::matrix<int> arr2d({1, 2, 3, 11, 22, 33}, 2, 3);
    std::vector a = arr2d[1];
    std::vector b({11, 22, 33});
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(*a[i], b[i]);
    }
}

TEST(MatrixTest, Determinant) {
    bmstu::matrix arr2d({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    bmstu::matrix matritsa({1, 2, 3, 11, 22, 33}, 3, 2);
    ASSERT_THROW(matritsa.det(), std::logic_error);
    ASSERT_EQ(arr2d.det(), 0);
}

TEST(MatrixTest, Multiplication) {
    bmstu::matrix arr2d({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    bmstu::matrix matritsa({1, 2, 3, 11, 22, 33}, 3, 2);
    bmstu::matrix rezult_mult_matrix = arr2d * matritsa;
    bmstu::matrix solution_mult_matrix({73, 123, 803, 1353, 8103, 13653}, 3, 2);
    bmstu::matrix rezult_mult_number = arr2d * 2;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            ASSERT_EQ(rezult_mult_matrix(i, j), solution_mult_matrix(i, j));
            ASSERT_EQ(rezult_mult_number(i, j), arr2d(i, j)*2);
        }
    }
    ASSERT_THROW(matritsa * arr2d, std::logic_error);
}

TEST(MatrixTest, Addition) {
    bmstu::matrix arr2d({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    bmstu::matrix matritsa({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    bmstu::matrix rezult_add = arr2d + matritsa;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_add(i, j), arr2d(i, j)*2);
        }
    }
    bmstu::matrix fail({1, 2, 3}, 1, 3);
    ASSERT_THROW(arr2d + fail, std::logic_error);
}

TEST(MatrixTest, Substraction) {
    bmstu::matrix arr2d({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    bmstu::matrix matritsa({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    bmstu::matrix rezult_substr = arr2d - matritsa;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_substr(i, j), 0);
        }
    }
    bmstu::matrix fail({1, 2, 3}, 1, 3);
    ASSERT_THROW(arr2d - fail, std::logic_error);
}

TEST(MatrixTest, GetMinor) {
    bmstu::matrix arr2d({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    bmstu::matrix rezult_minor = arr2d.get_minor(2, 1);
    bmstu::matrix solution({1, 3, 11, 33}, 2, 2);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            ASSERT_EQ(rezult_minor(i, j), solution(i, j));
        }
    }
    ASSERT_THROW(arr2d.get_minor(-1, 7), std::logic_error);
    ASSERT_THROW(arr2d.get_minor(6, 1), std::logic_error);
}


TEST(MatrixTest, Transpose) {
    bmstu::matrix arr2d({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    arr2d.transpose();
    bmstu::matrix rezult_trans({1, 11, 111, 2, 22, 222, 3, 33, 333}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(arr2d(i, j), rezult_trans(i, j));
        }
    }
}

TEST(MatrixTest, Reverse) {
    bmstu::matrix arr2d({2, 5, 7, 6, 3, 4, 5, -2, -3}, 3, 3);
    bmstu::matrix rezult_reverse = arr2d.reverse();
    bmstu::matrix solution({1, -1, 1, -38, 41, -34, 27, -29, 24}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_reverse(i, j), solution(i, j));
        }
    }
    bmstu::matrix fail({1, 2, 3, 11, 22, 33, 111, 222, 333}, 3, 3);
    ASSERT_THROW(fail.reverse(), std::logic_error);
}

TEST(MatrixTest, Unit) {
    bmstu::matrix arr2d({2, 5, 7, 6, 3, 4, 5, -2, -3}, 3, 3);
    bmstu::matrix result({1, 0, 0, 0, 0, 1, 0, 1, 0}, 3, 3);
    arr2d.unit();
    arr2d.swap_rows(1, 2);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(arr2d(i, j), result(i, j));
        }
    }
}
