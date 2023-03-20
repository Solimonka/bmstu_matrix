#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <valarray>

#pragma once

namespace bmstu {
    template<typename T>
    class matrix {
    public:
        matrix() : data_(1), rows_(1), columns_(1) {
            if (!std::is_arithmetic_v<T>) {
                throw std::logic_error("Arithmetic error");
            }
            representation_ = {{&data_[0]}};
        }

        matrix(size_t rows, size_t columns) : data_(rows * columns), rows_(rows), columns_(columns) {
            if (!std::is_arithmetic_v<T>) {
                throw std::logic_error("Arithmetic error");
            }
            for (size_t i = 0; i < rows_; i++) {
                std::vector<T *> current_row(columns_);
                for (size_t j = 0; j < columns_; j++) {
                    current_row[j] = (&data_[i * columns_ + j]);
                }
                representation_.push_back(std::move(current_row));
            }
        }

        matrix(std::initializer_list<T> i_list, size_t rows, size_t columns) : rows_(rows), columns_(columns) {
            if (!std::is_arithmetic_v<T>) {
                throw std::logic_error("Arithmetic error");
            }
            if (i_list.size() == rows_ * columns_) {
                data_.resize(columns_ * rows_);
                std::move(i_list.begin(), i_list.end(), data_.data());
                for (size_t i = 0; i < rows_; i++) {
                    std::vector<T *> current_row(columns_);
                    for (size_t j = 0; j < columns_; j++) {
                        current_row[j] = (&data_[i * columns_ + j]);
                    }
                    representation_.push_back(std::move(current_row));
                }
            } else {
                throw std::out_of_range("No" + std::to_string(rows) + ":" + std::to_string(columns));
            }
        }

        T &operator()(size_t row, size_t column) {
            return *representation_[row][column];
        }

        T operator()(size_t row, size_t column) const {
            T copy = *representation_[row][column];
            return copy;
        }

        friend std::ostream &operator<<(std::ostream &os, const bmstu::matrix<T> &obj) {
            for (size_t i = 0; i < obj.rows_; ++i) {
                for (size_t j = 0; j < obj.columns_; ++j) {
                    os << obj(i, j) << " ";
                };
                std::cout << "\r\n";
            }
            return os;
        }

        std::vector<T *> operator[](size_t i) {
            return representation_[i];
        };

        std::vector<T *> operator[](size_t i) const {
            std::vector<T> result;
            result.resize(columns_);
            for (size_t j = 0; j < columns_; ++j) {
                result[j] = *representation_[i][j];
            }
            return result;
        };

        T det() {
            std::vector<std::vector<size_t>> res;
            if (columns_ != rows_) {
                throw std::logic_error("cannot count");
            } else {
                T result = T();
                std::vector<size_t> indexes(columns_);
                for (size_t i = 0; i < columns_; ++i) {
                    indexes[i] = i;
                }
                permute(indexes, indexes.size(), result, res);
                return result;
            }
        }

        friend matrix<T> operator*(const matrix<T> &l, const matrix<T> &r) {
            if (l.columns_ == r.rows_) {
                throw std::logic_error("No size");
            }
            matrix<T> result(l.rows_, r.columns_);
            for (int i = 0; i < l.rows_; ++i) {
                for (int j = 0; j < l.columns_; ++j) {
                    for (int k = 0; k < l.columns_; k++) {
                        result(i, j) += l(i, k) * r(k, j);
                    }
                }
            }
        }

        friend matrix<T> operator+(const matrix<T> &l, const matrix<T> &r) {
            if (l.columns_ == r.columns_ and l.rows_ == r.rows_) {
                matrix<T> result(l.rows_, r.columns_);
                for (int i = 0; i < l.rows_; ++i) {
                    for (int j = 0; j < l.columns_; ++j) {
                        result(i, j) += l(i, j) + r(i, j);
                    }
                }
                return result;
            }
        }

        friend matrix<T> operator-(const matrix<T> &l, const matrix<T> &r) {
            if (l.columns_ == r.columns_ and l.rows_ == r.rows_) {
                matrix<T> result(l.rows_, r.columns_);
                for (int i = 0; i < l.rows_; ++i) {
                    for (int j = 0; j < l.columns_; ++j) {
                        result(i, j) += l(i, j) - r(i, j);
                    }
                }
                return result;
            }
        }

        friend matrix<T> operator*(const matrix<T> &l, const T value) {
            matrix<T> result(l.rows_, l.columns_);
            for (int i = 0; i < l.rows_; ++i) {
                for (int j = 0; j < l.columns_; ++j) {
                    result(i, j) += l(i, j) * value;
                }
            }
        }

        matrix<T> get_minor(size_t row, size_t column) {
            matrix<T> result(rows_ - 1, columns_ - 1);
            size_t counter = 0;
            for (int i = 0; i < rows_; ++i) {
                for (size_t j = 0; j < columns_; j++) {
                    if (j == columns_) {
                        continue;
                    } else {
                        result.data_[counter++] = data_[i * columns_ + j];
                    }
                }
            }
            return result;
        }

        void transpose() {
            representation_ = {};
            std::swap(rows_, columns_);
            for (size_t i = 0; i < rows_; ++i) {
                std::vector<T *> current_row(columns_);
                for (size_t j = 0; j < columns_; j++) {
                    current_row[j] = (&data_[j * rows_ + i]);
                }
                representation_.push_back((std::move(current_row)));
            }
        }

        matrix<T> adj() {
            matrix<T> result(rows_, columns_);
            for (size_t i = 0; i < rows_; ++i) {
                for (size_t j = 0; j < columns_; j++) {
                    result(i, j) = std::pow(-1, i + j) + get_minor(i, j).det();
                }
            }
        }

        matrix<T> reverse() {
            auto det_s = det();
            if (det_s == 0) {
                throw std::logic_error("no");
            } else {
                T sec_det = 1 / det();
                matrix<T> result = adj();
                result.transpose();
                for (auto &item: result) {
                    item = item * sec_det;
                }
                return result;
            }
        }

    private:
        void permute(std::vector<size_t> &a, size_t pos, T &value, int &sign) {
            if (pos == 1) {
                T mrow = T(1);
                for (size_t i = 0; i < columns_; ++i) {
                    mrow *= *this->representation_[i][a[i]];
                }
                if (sign > 0) {
                    value += mrow;
                } else {
                    value += mrow * (-1);
                }
                std::cout << &a << " " << sign << std::endl;
                sign *= -1;
                return;
            }
            for (size_t i = 0; i < pos - 1; ++i) {
                permute(a, pos - 1, value, sign);
                if (pos % 2 == 0) {
                    std::swap(a[i], a[pos - 1]);
                } else {
                    std::swap(a[0], a[pos - 1]);
                }
                permute(a, pos - 1, value, sign);
            }
        }

        std::vector<T> data_;
        std::vector<std::vector<T *>> representation_;
        size_t columns_;
        size_t rows_;
    };
}
