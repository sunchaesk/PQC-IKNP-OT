
#include "../include/ot.h"

///////////////////////////
//Base OT Receiver
///////////////////////////

BaseOTReceiver::BaseOTReceiver(uint64_t n, uint64_t q, uint64_t k)
    : kyber_instance(n, q, k) {}

Kyber::PublicKey BaseOTReceiver::oblivious_keygen() {
    // generate garbage as the "fake pk"
    PolynomialMatrix A = kyber_instance.sample_uniform_matrix();
    PolynomialVector t = kyber_instance.sample_small_vector(3); 
    return Kyber::PublicKey{A, t};
}

std::pair<Kyber::PublicKey, Kyber::PublicKey> BaseOTReceiver::generate_public_keys(uint8_t bit) {
    real_keypair = kyber_instance.keygen(3);
    fake_pk = oblivious_keygen();

    if (bit == 0) {
        return {real_keypair->first, *fake_pk};
    } else {
        return {*fake_pk, real_keypair->first};
    }
}

void BaseOTReceiver::receive_ciphertexts(const Kyber::Ciphertext& ct0, const Kyber::Ciphertext& ct1) {
    ciphertexts = std::make_pair(ct0, ct1);
}

Polynomial BaseOTReceiver::decrypt_bit(uint8_t choice_bit) {
    assert(choice_bit == 0 || choice_bit == 1);

    Kyber::Ciphertext ct = (choice_bit == 0) ? ciphertexts->first : ciphertexts->second;
    Polynomial decrypted_msg = kyber_instance.decrypt(real_keypair->second, ct);

    return decrypted_msg;
}
///////////////////////////
//Base OT Sender
///////////////////////////

BaseOTSender::BaseOTSender(uint64_t n, uint64_t q, uint64_t k)
    : kyber_instance(n, q, k) {}


void BaseOTSender::receive_public_keys(const Kyber::PublicKey& pk0, const Kyber::PublicKey& pk1) {
    received_pks = {pk0, pk1}; 
}

std::pair<Kyber::Ciphertext, Kyber::Ciphertext> BaseOTSender::encrypt_bits(const Polynomial& msg0, const Polynomial& msg1) {
    Kyber::Ciphertext ct0 = kyber_instance.encrypt(received_pks->first, msg0, 3);
    Kyber::Ciphertext ct1 = kyber_instance.encrypt(received_pks->second, msg1, 3);
    return {ct0, ct1};
}