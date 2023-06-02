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
            if constexpr (!std::is_arithmetic_v<T>) {
                throw std::logic_error("you're loser");
            }
            representation_ = {{&data_[0]}};
        }

        matrix(size_t rows, size_t columns) : data_(rows * columns), rows_(rows), columns_(columns) {
            if constexpr (!std::is_arithmetic_v<T>) {
                throw std::logic_error("you're loser");
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
            if constexpr (!std::is_arithmetic_v<T>) {
                throw std::logic_error("you're loser");
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
                os << "\r\n";
            }
            return os;
        }

        std::vector<T *> operator[](size_t i) {
            return representation_[i];
        };

        std::vector<T> operator[](size_t i) const {
            std::vector<T> result;
            result.resize(columns_);
            for (size_t j = 0; j < columns_; ++j) {
                result[j] = *representation_[i][j];
            }
            return result;
        };

        T det() {
            if (columns_ != rows_) {
                throw std::logic_error("you're loser");
            } else {
                T result = T();
                std::vector<size_t> indexes(columns_);
                int sign = 1;
                for (size_t i = 0; i < columns_; ++i) {
                    indexes[i] = i;
                }
                permute(indexes, indexes.size(), result, sign);
                return result;
            }
        }


        friend matrix<T> operator*(const matrix<T> &l, const matrix<T> &r) {
            if (l.columns_ != r.rows_) {
                throw std::logic_error("you're loser");
            }
            matrix<T> result(l.rows_, r.columns_);
            for (int i = 0; i < l.rows_; ++i) {
                for (int j = 0; j < r.columns_; ++j) {
                    for (int k = 0; k < l.columns_; ++k) {
                        result(i, j) += l(i, k) * r(k, j);
                    }
                }
            }
            return result;
        }

        friend matrix<T> operator*(const matrix<T> &l, const T value) {
            matrix<T> result(l.rows_, l.columns_);
            for (int i = 0; i < l.rows_; ++i) {
                for (int j = 0; j < l.columns_; ++j) {
                    result(i, j) += l(i, j) * value;
                }
            }
            return result;
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
            throw std::logic_error("you're loser");
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
            throw std::logic_error("you're loser");
        }

        matrix<T> get_minor(size_t row, size_t column) {
            if (row < rows_ && column < columns_) {
                matrix<T> result(rows_ - 1, columns_ - 1);
                size_t row_pos = 0;
                for (size_t i = 0; i < rows_; ++i) {
                    if (i == row) {
                        continue;
                    }
                    size_t column_pos = 0;
                    for (size_t j = 0; j < columns_; ++j) {
                        if (j == column) {
                            continue;
                        }
                        result(row_pos, column_pos) = *representation_[i][j];
                        column_pos += 1;
                    }
                    row_pos += 1;
                }
                return result;
            } else {
                throw std::out_of_range("you're loser");
            }
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

        matrix<T> reverse() {
            auto det_matr = det();
            if (det_matr == 0) {
                throw std::logic_error("you're loser");
            } else {
                T revers_det = 1 / det_matr;
                matrix<T> result = adj();
                for (auto &item: result.data_) {
                    item = item * revers_det;
                }
                return result;
            }
        }

        matrix<T> adj() {
            matrix<T> result(rows_, columns_);
            for (size_t i = 0; i < rows_; ++i) {
                for (size_t j = 0; j < columns_; ++j) {
                    result(i, j) = this->get_minor(i, j).det();
                    if ((i + j) % 2) {
                        result(i, j) = -result(i, j);
                    }
                }
            }
            result.transpose();
            return result;
        }


        size_t columns() const noexcept {
            return columns_;
        }

        size_t rows() const noexcept {
            return rows_;
        }

        void unit(){
            if (rows_ != columns_){
                throw std::logic_error("lOsEr");
            } else {
                for (size_t i = 0; i < columns_; ++i){
                    for (size_t j = 0; j < columns_; ++j){
                        if (i == j){
                            data_[i * columns_ + j] = 1;
                        }else{
                            data_[i * rows_ + j] = 0;
                        }
                    }
                }
            }
        }
        
        void swap_rows(size_t a, size_t b){
            if (a > rows_ || a < 0 || b > rows_ || b < 0){
                throw std::logic_error("lOsEr");
            } else {
                std::vector<T> first = representation_[a];
                std::vector<T> second = representation_[b];
                representation_[a] = second;
                representation_[b] = first;
            }
        }
        
    private:
        void permute(std::vector<size_t> &a, size_t pos, T &value, int &sign) {
            if (pos == 1) {
                T mrow = T(1);
                for (size_t i = 0; i < columns_; ++i) {
                    mrow *= *this->representation_[i][a[i]];
                }
                value += (mrow * sign);
            } else {
                for (size_t i = 0; i < pos; ++i) {
                    permute(a, pos - 1, value, sign);
                    if (pos % 2) {
                        std::swap(a[0], a[pos - 1]);
                    } else {
                        std::swap(a[i], a[pos - 1]);
                        sign *= (-1);
                    }
                }
            }
        }

        std::vector<T> data_;
        std::vector<std::vector<T *>> representation_;
        size_t columns_;
        size_t rows_;
    };
}
