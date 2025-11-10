
#include <gtest/gtest.h>
#include "../include/ot.h"


class OTTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(OTTest, BaseOTTest) {
    BaseOTSender sender;
    BaseOTReceiver receiver;

    auto [pk0, pk1] = receiver.generate_public_keys(1); // 1 -> bit

    sender.receive_public_keys(pk0, pk1);
    // make size 256 poly msg
    // coeff must be 0 or 1 otherwise decryption rounding goes wrong
    Polynomial msg0(std::vector<uint64_t>(256, 0), 3329);
    Polynomial msg1(std::vector<uint64_t>(256, 1), 3329);
    auto [ct0, ct1] = sender.encrypt_bits(msg0, msg1);

    receiver.receive_ciphertexts(ct0, ct1);
    Polynomial received_msg = receiver.decrypt_bit(1); 

    EXPECT_EQ(received_msg.coeffs(), msg1.coeffs());

}


TEST_F(OTTest, BaseOTWrongChoiceTest) {
    BaseOTSender sender;
    BaseOTReceiver receiver;

    auto [pk0, pk1] = receiver.generate_public_keys(1); // 1 -> bit

    sender.receive_public_keys(pk0, pk1);
    // make size 256 poly msg
    // coeff must be 0 or 1 otherwise decryption rounding goes wrong
    Polynomial msg0(std::vector<uint64_t>(256, 0), 3329);
    Polynomial msg1(std::vector<uint64_t>(256, 1), 3329);
    auto [ct0, ct1] = sender.encrypt_bits(msg0, msg1);

    receiver.receive_ciphertexts(ct0, ct1);
    Polynomial received_msg = receiver.decrypt_bit(0); 

    EXPECT_NE(received_msg.coeffs(), msg1.coeffs());

}

TEST_F(OTTest, BaseOTRandomMessage) {
    BaseOTSender sender;
    BaseOTReceiver receiver;

    auto [pk0, pk1] = receiver.generate_public_keys(0); 

    sender.receive_public_keys(pk0, pk1);
    std::vector<uint64_t> msg0_coeffs(256);
    std::vector<uint64_t> msg1_coeffs(256);
    for (size_t i = 0; i < 256; ++i) {
        msg0_coeffs[i] = rand() % 2; // random 0 or 1
        msg1_coeffs[i] = rand() % 2; // random 0 or 1
    }
    Polynomial msg0(msg0_coeffs, 3329);
    Polynomial msg1(msg1_coeffs, 3329);
    auto [ct0, ct1] = sender.encrypt_bits(msg0, msg1);
    receiver.receive_ciphertexts(ct0, ct1);
    Polynomial received_msg = receiver.decrypt_bit(0);
    EXPECT_EQ(received_msg.coeffs(), msg0.coeffs());
}