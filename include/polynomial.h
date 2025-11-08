#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include <cstdint>
#include <assert.h>
#include <stdexcept>

// NOTE: we can perform the unsafe addition and subtraction 
// cuz uint64_max > q we use for Kyber schemes

class Polynomial {
private:
    std::vector<uint64_t> coefficients;
    uint64_t N;
    uint64_t q;

    bool use_schoolbook;
public:
    Polynomial(std::vector<uint64_t> coeffs, uint64_t modulus_q, bool schoolbook = true)
        : coefficients(coeffs), N(coeffs.size()), q(modulus_q) {

            assert(schoolbook); // NTT not supported yet
        }

    Polynomial operator+(const Polynomial& other) const {
        if (N != other.N) {
            throw std::invalid_argument("Polynomials must have the same degree");
        }
        if (q != other.q) {
            throw std::invalid_argument("Polynomials must have the same modulus");
        }
        std::vector<uint64_t> result_coeffs(N);
        for (uint64_t i = 0; i < N; ++i) {
            result_coeffs[i] = (coefficients[i] + other.coefficients[i]) % q;
        }
        return Polynomial(result_coeffs, q);
    }
    Polynomial operator-(const Polynomial& other) const {
        if (N != other.N) {
            throw std::invalid_argument("Polynomials must have the same degree");
        }
        if (q != other.q) {
            throw std::invalid_argument("Polynomials must have the same modulus");
        }
        std::vector<uint64_t> result_coeffs(N);
        for (uint64_t i = 0; i < N; ++i) {
            result_coeffs[i] = (coefficients[i] + q - other.coefficients[i]) % q;
        }
        return Polynomial(result_coeffs, q);
    }

    Polynomial operator*(const Polynomial& other) const {
        if (N != other.N) {
            throw std::invalid_argument("Polynomials must have the same degree");
        }
        if (q != other.q) {
            throw std::invalid_argument("Polynomials must have the same modulus");
        }
        std::vector<uint64_t> result_coeffs(N, 0);
        for (uint64_t i = 0; i < N; ++i) {
            for (uint64_t j = 0; j < N; ++j) {
                uint64_t product = (coefficients[i] * other.coefficients[j]) % q;
                if (i + j < N) {
                    result_coeffs[i + j] = (result_coeffs[i + j] + product) % q;
                } else {
                    // Negacyclic: X^N = -1, so subtract the term
                    result_coeffs[i + j - N] = (result_coeffs[i + j - N] + q - product) % q;
                }
            }
        }
        return Polynomial(result_coeffs, q);
    }

    // Accessor methods
    uint64_t degree() const { return N; }

    const std::vector<uint64_t>& coeffs() const { return coefficients; }

    // Scalar multiplication (supports negative scalars via modular arithmetic)
    Polynomial multiply_scalar(int64_t scalar) const {
        std::vector<uint64_t> result_coeffs(N);
        // Convert negative scalar to positive equivalent: ((n % q) + q) % q
        int64_t mod_scalar = ((scalar % static_cast<int64_t>(q)) + static_cast<int64_t>(q)) % static_cast<int64_t>(q);
        uint64_t positive_scalar = static_cast<uint64_t>(mod_scalar);

        for (uint64_t i = 0; i < N; ++i) {
            result_coeffs[i] = (coefficients[i] * positive_scalar) % q;
        }
        return Polynomial(result_coeffs, q);
    }

};

#endif