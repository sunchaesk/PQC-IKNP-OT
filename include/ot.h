#ifndef OT_H
#define OT_H

#include "kyber.h"
#include <optional>
#include <assert.h>

class BaseOTReceiver {
private:
    Kyber kyber_instance;
    std::optional<std::pair<Kyber::PublicKey, Kyber::SecretKey>> real_keypair;
    std::optional<Kyber::PublicKey> fake_pk;

    Kyber::PublicKey oblivious_keygen(); // just generate random garbage

    std::optional<std::pair<Kyber::Ciphertext, Kyber::Ciphertext>> ciphertexts;

public:
    BaseOTReceiver(uint64_t n = 256, uint64_t q = 3329, uint64_t k = 3);

    std::pair<Kyber::PublicKey, Kyber::PublicKey> generate_public_keys(uint8_t bit);
    void receive_ciphertexts(const Kyber::Ciphertext& ct0, const Kyber::Ciphertext& ct1);
    Polynomial decrypt_bit(uint8_t choice_bit);
};

class BaseOTSender {
private:
    Kyber kyber_instance;
    std::optional<std::pair<Kyber::PublicKey, Kyber::PublicKey>> received_pks;


public:
    BaseOTSender(uint64_t n = 256, uint64_t q = 3329, uint64_t k = 3);

    void receive_public_keys(const Kyber::PublicKey& pk0, const Kyber::PublicKey& pk1);
    std::pair<Kyber::Ciphertext, Kyber::Ciphertext> encrypt_bits(const Polynomial& msg0, const Polynomial& msg1);
};


#endif