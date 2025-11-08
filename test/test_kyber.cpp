#include <gtest/gtest.h>
#include "../include/kyber.h"

// Test fixture for Kyber tests
class KyberTest : public ::testing::Test {
protected:
    // Small parameters for fast testing
    uint64_t n_small = 16;   // Polynomial degree
    uint64_t q = 3329;        // Kyber modulus
    uint64_t k = 2;           // Vector dimension
    int error_bound = 2;

    // Typical Kyber512 parameters (for more thorough tests)
    uint64_t n_kyber = 256;
    uint64_t k_kyber512 = 2;
    uint64_t k_kyber768 = 3;
    uint64_t k_kyber1024 = 4;
};

// Test 1: Key generation doesn't crash and produces keys
TEST_F(KyberTest, KeyGeneration) {
    Kyber kyber(n_small, q, k);
    auto [pk, sk] = kyber.keygen(error_bound);

    // Check that keys have the right dimensions
    EXPECT_EQ(pk.A.get_rows(), k);
    EXPECT_EQ(pk.A.get_cols(), k);
    EXPECT_EQ(pk.t.size(), k);
    EXPECT_EQ(sk.s.size(), k);
}

// Test 2: Encrypt and decrypt all-zero message
TEST_F(KyberTest, EncryptDecryptZeroMessage) {
    Kyber kyber(n_small, q, k);
    auto [pk, sk] = kyber.keygen(error_bound);

    // Create all-zero message
    std::vector<uint64_t> zero_msg(n_small, 0);
    Polynomial message(zero_msg, q);

    // Encrypt and decrypt
    auto ciphertext = kyber.encrypt(pk, message, error_bound);
    Polynomial decrypted = kyber.decrypt(sk, ciphertext);

    // Check that decrypted message matches original
    const auto& decrypted_coeffs = decrypted.coeffs();
    for (uint64_t i = 0; i < n_small; ++i) {
        EXPECT_EQ(decrypted_coeffs[i], 0) << "Mismatch at coefficient " << i;
    }
}

// Test 3: Encrypt and decrypt all-one message
TEST_F(KyberTest, EncryptDecryptOneMessage) {
    Kyber kyber(n_small, q, k);
    auto [pk, sk] = kyber.keygen(error_bound);

    // Create all-one message
    std::vector<uint64_t> one_msg(n_small, 1);
    Polynomial message(one_msg, q);

    // Encrypt and decrypt
    auto ciphertext = kyber.encrypt(pk, message, error_bound);
    Polynomial decrypted = kyber.decrypt(sk, ciphertext);

    // Check that decrypted message matches original
    const auto& decrypted_coeffs = decrypted.coeffs();
    for (uint64_t i = 0; i < n_small; ++i) {
        EXPECT_EQ(decrypted_coeffs[i], 1) << "Mismatch at coefficient " << i;
    }
}

// Test 4: Encrypt and decrypt random binary message
TEST_F(KyberTest, EncryptDecryptRandomBinaryMessage) {
    Kyber kyber(n_small, q, k);
    auto [pk, sk] = kyber.keygen(error_bound);

    // Create random binary message (alternating pattern)
    std::vector<uint64_t> random_msg(n_small);
    for (uint64_t i = 0; i < n_small; ++i) {
        random_msg[i] = i % 2;  // Alternating 0, 1, 0, 1, ...
    }
    Polynomial message(random_msg, q);

    // Encrypt and decrypt
    auto ciphertext = kyber.encrypt(pk, message, error_bound);
    Polynomial decrypted = kyber.decrypt(sk, ciphertext);

    // Check that decrypted message matches original
    const auto& decrypted_coeffs = decrypted.coeffs();
    for (uint64_t i = 0; i < n_small; ++i) {
        EXPECT_EQ(decrypted_coeffs[i], random_msg[i])
            << "Mismatch at coefficient " << i;
    }
}

// Test 5: Multiple encrypt/decrypt cycles
TEST_F(KyberTest, MultipleEncryptDecryptCycles) {
    Kyber kyber(n_small, q, k);
    auto [pk, sk] = kyber.keygen(error_bound);

    // Try multiple different messages
    for (int cycle = 0; cycle < 5; ++cycle) {
        std::vector<uint64_t> msg(n_small);
        for (uint64_t i = 0; i < n_small; ++i) {
            msg[i] = (i + cycle) % 2;
        }
        Polynomial message(msg, q);

        auto ciphertext = kyber.encrypt(pk, message, error_bound);
        Polynomial decrypted = kyber.decrypt(sk, ciphertext);

        const auto& decrypted_coeffs = decrypted.coeffs();
        for (uint64_t i = 0; i < n_small; ++i) {
            EXPECT_EQ(decrypted_coeffs[i], msg[i])
                << "Cycle " << cycle << ", coefficient " << i;
        }
    }
}

// Test 6: Larger Kyber512-like parameters
TEST_F(KyberTest, Kyber512Parameters) {
    Kyber kyber(n_kyber, q, k_kyber512);
    auto [pk, sk] = kyber.keygen(error_bound);

    // Create a random binary message
    std::vector<uint64_t> msg(n_kyber);
    for (uint64_t i = 0; i < n_kyber; ++i) {
        msg[i] = (i * 7) % 2;  // Pseudo-random pattern
    }
    Polynomial message(msg, q);

    auto ciphertext = kyber.encrypt(pk, message, error_bound);
    Polynomial decrypted = kyber.decrypt(sk, ciphertext);

    // Check correctness
    const auto& decrypted_coeffs = decrypted.coeffs();
    int errors = 0;
    for (uint64_t i = 0; i < n_kyber; ++i) {
        if (decrypted_coeffs[i] != msg[i]) {
            errors++;
        }
    }

    // With proper parameters, should have zero errors
    EXPECT_EQ(errors, 0) << "Had " << errors << " decryption errors";
}

// Test 7: Kyber768-like parameters
TEST_F(KyberTest, Kyber768Parameters) {
    Kyber kyber(n_kyber, q, k_kyber768);
    auto [pk, sk] = kyber.keygen(error_bound);

    std::vector<uint64_t> msg(n_kyber);
    for (uint64_t i = 0; i < n_kyber; ++i) {
        msg[i] = (i * 13) % 2;
    }
    Polynomial message(msg, q);

    auto ciphertext = kyber.encrypt(pk, message, error_bound);
    Polynomial decrypted = kyber.decrypt(sk, ciphertext);

    const auto& decrypted_coeffs = decrypted.coeffs();
    int errors = 0;
    for (uint64_t i = 0; i < n_kyber; ++i) {
        if (decrypted_coeffs[i] != msg[i]) {
            errors++;
        }
    }

    EXPECT_EQ(errors, 0) << "Had " << errors << " decryption errors";
}

// Test 8: Different error bounds
TEST_F(KyberTest, DifferentErrorBounds) {
    Kyber kyber(n_small, q, k);

    // Test with error_bound = 1, 2, 3
    for (int eb = 1; eb <= 3; ++eb) {
        auto [pk, sk] = kyber.keygen(eb);

        std::vector<uint64_t> msg(n_small);
        for (uint64_t i = 0; i < n_small; ++i) {
            msg[i] = i % 2;
        }
        Polynomial message(msg, q);

        auto ciphertext = kyber.encrypt(pk, message, eb);
        Polynomial decrypted = kyber.decrypt(sk, ciphertext);

        const auto& decrypted_coeffs = decrypted.coeffs();
        for (uint64_t i = 0; i < n_small; ++i) {
            EXPECT_EQ(decrypted_coeffs[i], msg[i])
                << "Error bound " << eb << ", coefficient " << i;
        }
    }
}