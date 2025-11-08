
#include "../include/polynomial.h"


///////////////////////////////
// Polynomial Vector
///////////////////////////////

PolynomialVector PolynomialVector::operator+(const PolynomialVector& other) const {
    if (k != other.k) {
        throw std::invalid_argument("PolynomialVectors must have the same size");
    }

    std::vector<Polynomial> result_polys;
    result_polys.reserve(k);

    for (size_t i = 0; i < this->k; i++) {
        result_polys.push_back(this->polynomials[i] + other.polynomials[i]);
    }

    return PolynomialVector(result_polys);
}

PolynomialVector PolynomialVector::operator-(const PolynomialVector& other) const {
    if (k != other.k) {
        throw std::invalid_argument("PolynomialVectors must have the same size");
    }

    std::vector<Polynomial> result_polys;
    result_polys.reserve(k);

    for (size_t i = 0; i < this->k; i++) {
        result_polys.push_back(this->polynomials[i] - other.polynomials[i]);
    }

    return PolynomialVector(result_polys);
}

Polynomial PolynomialVector::inner_product(const PolynomialVector& other) const {
    if (k != other.k) {
        throw std::invalid_argument("PolynomialVectors must have the same size");
    }

    Polynomial result = Polynomial(std::vector<uint64_t>(this->polynomials[0].get_N(), 0), this->polynomials[0].get_q());

    for (size_t i = 0; i < this->k; i++) {
        result = result + (this->polynomials[i] * other.polynomials[i]);
    }

    return result;
}

///////////////////////////////
// Polynomial Matrix
///////////////////////////////
PolynomialMatrix PolynomialMatrix::transpose() const {
    std::vector<std::vector<Polynomial>> transposed_matrix(cols, std::vector<Polynomial>());

    for (size_t j = 0; j < cols; j++) {
        transposed_matrix[j].reserve(rows);
        for (size_t i = 0; i < rows; i++) {
            transposed_matrix[j].push_back(matrix[i][j]);
        }
    }

    return PolynomialMatrix(transposed_matrix);
}

PolynomialVector PolynomialMatrix::matrix_vector_mul(const PolynomialVector& vec) const {
    if (cols != vec.size()) {
        throw std::invalid_argument("Matrix columns must match vector size");
    }
    if (vec.get_is_vertical() == false) {
        throw std::invalid_argument("Input vector must be a vertical vector");
    }

    std::vector<Polynomial> result_polys(rows, Polynomial(std::vector<uint64_t>(matrix[0][0].get_N(), 0), matrix[0][0].get_q()));

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            result_polys[i] = result_polys[i] + (matrix[i][j] * vec.get_polynomials()[j]);
        }
    }

    
    return PolynomialVector(result_polys, true);
}