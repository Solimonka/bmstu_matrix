#include <gtest/gtest.h>
#include <string>
#include "bmstu_matrix.h"

TEST(GoogleTestCi, Foo) {
    ASSERT_TRUE(true);
}

TEST(ExampleTest, DmitriiLeonidovich) {
    std::string dml = "DmitriiLeonidovich";
    const char *dml_c = "DmitriiLeonidovich";
    ASSERT_STREQ(dml.c_str(), dml_c);
}

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
    bmstu::matrix a({4, 7, 1, 5, 2, 8, 6, 3, 1}, 3, 3);
    bmstu::matrix b({4, 8, 2, 6, 0, 1}, 3, 2);
    ASSERT_EQ(a.det(), 216);
    ASSERT_THROW(b.det(), std::logic_error);
}

TEST(MatrixTest, Transpose) {
    bmstu::matrix a({1, 4, 6, 8, 0, 9, 5, 3, 7}, 3, 3);
    a.transpose();
    bmstu::matrix proverka_1({1, 8, 5, 4, 0, 3, 6, 9, 7}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(a(i, j), proverka_1(i, j));
        }
    }
}

TEST(MatrixTest, Multiplication) {
    bmstu::matrix a({9, 2, 7, 2, 6, 1, 8, 3, 7}, 3, 3);
    bmstu::matrix b({1, 6, 0, 2, 6, 7, 2, 8, 5}, 3, 3);

    bmstu::matrix rezult_1 = a * b;
    bmstu::matrix proverka_1({27, 122, 49, 16, 56, 47, 28, 122, 56}, 3, 3);
    bmstu::matrix rezult_2 = a * 5;
    bmstu::matrix proverka_2({45, 10, 35, 10, 30, 5, 40, 15, 35}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_1(i, j), proverka_1(i, j));
            ASSERT_EQ(rezult_2(i, j), proverka_2(i, j));
        }
    }
    bmstu::matrix d({2, 5, 7, 3, 9, 7}, 2, 3);
    bmstu::matrix c({4, 2, 7, 0, 4, 8, 1, 2, 4, 3, 5, 0}, 3, 4);

    bmstu::matrix rezult_3 = d * c;
    bmstu::matrix proverka_3({56, 65, 54, 10, 76, 99, 65, 18}, 2, 4);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            ASSERT_EQ(rezult_3(i, j), proverka_3(i, j));
        }
    }
    ASSERT_THROW(a * d, std::logic_error);
}

TEST(MatrixTest, Addition) {
    bmstu::matrix a({5, 4, 9, 3, 6, 9, 2, 1, 6}, 3, 3);
    bmstu::matrix b({5, 8, 9, 0, 4, 3, 1, 8, 7}, 3, 3);

    bmstu::matrix rezult_1 = a + b;
    bmstu::matrix proverka_1({10, 12, 18, 3, 10, 12, 3, 9, 13}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_1(i, j), proverka_1(i, j));
        }
    }
    bmstu::matrix c({6, 8, 0, 7, 8, 4}, 2, 3);
    ASSERT_THROW(a + c, std::logic_error);
}

TEST(MatrixTest, Substraction) {
    bmstu::matrix a({4, 8, 6, 4, 2, 1, 0, 9, 7}, 3, 3);
    bmstu::matrix b({5, 9, 7, 6, 1, 4, 0, 3, 2}, 3, 3);

    bmstu::matrix rezult_1 = a - b;
    bmstu::matrix proverka_1({-1, -1, -1, -2, 1, -3, 0, 6, 5}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_1(i, j), proverka_1(i, j));
        }
    }
    bmstu::matrix c({7, 0, 1, 3, 9, 8}, 2, 3);
    ASSERT_THROW(a - c, std::logic_error);
}

TEST(MatrixTest, GetMinor) {
    bmstu::matrix a({2, 6, 9, 8, 3, 1, 0, 8, 6, 5, 0, 4}, 4, 3);
    bmstu::matrix rezult_1 = a.get_minor(1, 1);
    bmstu::matrix proverka_1({2, 9, 0, 6, 5, 4}, 3, 2);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            ASSERT_EQ(rezult_1(i, j), proverka_1(i, j));
        }
    }
    ASSERT_THROW(a.get_minor(-1, 1), std::logic_error);
    ASSERT_THROW(a.get_minor(4, 1), std::logic_error);
}

TEST(MatrixTest, Reverse) {
    bmstu::matrix a({1, -2, 1, 2, 1, -1, 3, 2, -2}, 3, 3);
    bmstu::matrix rezult_1 = a.reverse();
    bmstu::matrix proverka_1({0, 2, -1, -1, 5, -3, -1, 8, -5}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_1(i, j), proverka_1(i, j));
        }
    }
    bmstu::matrix rezult_2 = a * rezult_1;
    bmstu::matrix proverka_2({1, 0, 0, 0, 1, 0, 0, 0, 1}, 3, 3);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            ASSERT_EQ(rezult_2(i, j), proverka_2(i, j));
        }
    }

    bmstu::matrix b({4, -5, -2, 1, 7, 9, 1, -4, 3, -9, 5, 1}, 4, 3);
    ASSERT_THROW(b.reverse(), std::logic_error);
    bmstu::matrix c({1, 2, 3, 4, 5, 6, 7, 8, 9}, 3, 3);
    ASSERT_THROW(c.reverse(), std::logic_error);
}