#ifndef KYBER_H
#define KYBER_H

#include "polynomial.h"

#include <vector>
#include <utility>

class Kyber {
private:
    using PolyVec = PolynomialVector;
    using PolyMat = PolynomialMatrix;

    uint64_t n;  // Polynomial degree (typically 256)
    uint64_t q;  // Modulus (typically 3329)
    uint64_t k;  // Vector dimension (2, 3, or 4 for Kyber512/768/1024)

public:
    Kyber(uint64_t n, uint64_t q, uint64_t k) : n(n), q(q), k(k) {}

    struct PublicKey {
        PolyMat A;
        PolyVec t;
    };
    struct SecretKey {
        PolyVec s;
    };
    struct Ciphertext {
        PolyVec u;
        Polynomial v;
    };

    // Simple sampling methods for development
    // Generates a polynomial with uniform random coefficients in [0, q-1]
    Polynomial sample_uniform_poly() const;
    // Generates a polynomial with small coefficients in [-bound, bound]
    // Use for secret/error vectors (typically bound = 2 or 3)
    Polynomial sample_small_poly(int bound) const;

    // Generate a k×k matrix of uniform random polynomials
    PolyMat sample_uniform_matrix() const;

    // Generate a vector of k small random polynomials
    PolyVec sample_small_vector(int bound) const;

    // Key generation
    // Returns pair of (public_key, secret_key)
    // error_bound: typical value is 2 or 3 for development
    std::pair<Kyber::PublicKey, Kyber::SecretKey> keygen(int error_bound) const;
    Kyber::Ciphertext encrypt(const Kyber::PublicKey& pk, const Polynomial& message, int error_bound) const;
    Polynomial decrypt(const Kyber::SecretKey& sk, const Kyber::Ciphertext& ct) const;
};
#endif