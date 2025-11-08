#include "../include/kyber.h"
#include <random>

// Thread-local random number generator
// Using thread_local to avoid global state issues
static thread_local std::random_device rd;
static thread_local std::mt19937_64 rng(rd());

Polynomial Kyber::sample_uniform_poly() const {
    std::uniform_int_distribution<uint64_t> dist(0, q - 1);
    std::vector<uint64_t> coeffs(n);

    for (uint64_t i = 0; i < n; ++i) {
        coeffs[i] = dist(rng);
    }

    return Polynomial(coeffs, q);
}

Polynomial Kyber::sample_small_poly(int bound) const {
    // Sample coefficients uniformly from [-bound, bound]
    std::uniform_int_distribution<int> dist(-bound, bound);
    std::vector<uint64_t> coeffs(n);

    for (uint64_t i = 0; i < n; ++i) {
        int val = dist(rng);
        // Convert to unsigned by adding q if negative
        coeffs[i] = (val >= 0) ? static_cast<uint64_t>(val) : (q + val);
    }

    return Polynomial(coeffs, q);
}

PolynomialMatrix Kyber::sample_uniform_matrix() const {
    std::vector<std::vector<Polynomial>> matrix;
    matrix.reserve(k);

    for (uint64_t i = 0; i < k; ++i) {
        std::vector<Polynomial> row;
        row.reserve(k);
        for (uint64_t j = 0; j < k; ++j) {
            row.push_back(sample_uniform_poly());
        }
        matrix.push_back(std::move(row));
    }

    return PolynomialMatrix(matrix);
}

PolynomialVector Kyber::sample_small_vector(int bound) const {
    std::vector<Polynomial> polys;
    polys.reserve(k);

    for (uint64_t i = 0; i < k; ++i) {
        polys.push_back(sample_small_poly(bound));
    }

    return PolynomialVector(polys, true);  // true = vertical vector
}

std::pair<Kyber::PublicKey, Kyber::SecretKey> Kyber::keygen(int error_bound) const {
    PolynomialMatrix A = sample_uniform_matrix();

    PolynomialVector s = sample_small_vector(error_bound);

    PolynomialVector e = sample_small_vector(error_bound);

    PolynomialVector As = A.matrix_vector_mul(s);
    PolynomialVector t = As + e;

    PublicKey pk{A, t};
    SecretKey sk{s};

    return {pk, sk};
}

Kyber::Ciphertext Kyber::encrypt(const Kyber::PublicKey& pk, const Polynomial& message, int error_bound) const {
    PolynomialVector r = sample_small_vector(error_bound);

    PolynomialVector e1 = sample_small_vector(error_bound);
    Polynomial e2 = sample_small_poly(error_bound);

    // PolynomialVector u = pk.A.matrix_vector_mul(r) + e1;
    PolynomialVector u = pk.A.transpose().matrix_vector_mul(r) + e1;

    Polynomial tr = pk.t.inner_product(r);
    // Scale message by floor(q/2) for proper encoding
    Polynomial scaled_message = message.multiply_scalar(q / 2);
    Polynomial v = tr + e2 + scaled_message;

    return Ciphertext{u, v};
}

// rounding is done inside this function also
Polynomial Kyber::decrypt(const Kyber::SecretKey& sk, const Kyber::Ciphertext& ct) const {
    Polynomial sr = sk.s.inner_product(ct.u);

    Polynomial m_prime = ct.v - sr;

    // Decode message: coefficients closer to 0 mod q are 0, closer to q/2 are 1
    std::vector<uint64_t> decoded_coeffs(n);
    uint64_t threshold = q / 4;  // Midpoint for rounding

    for (uint64_t i = 0; i < n; ++i) {
        uint64_t coeff = m_prime.coeffs()[i] % q;
        if (coeff < threshold || coeff >= q - threshold) {
            decoded_coeffs[i] = 0;
        } else {
            decoded_coeffs[i] = 1;
        }
    }

    return Polynomial(decoded_coeffs, q);
}