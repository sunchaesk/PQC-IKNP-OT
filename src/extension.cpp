
#include "../include/extension.h"

///////////////////////////////////
// OT Extension Receiver
///////////////////////////////////

void OTExtensionReceiver::initialize_base_ots() {
    base_ot_receivers.reserve(this->security_parameter);

    for (size_t i = 0; i < this->security_parameter; ++i) {
        base_ot_receivers.push_back(std::make_unique<BaseOTReceiver>());
    }
}


void OTExtensionReceiver::generate_random_matrices_t_u() {
    uint32_t k = this->security_parameter;
    uint32_t m = this->num_ots;

    // m rows, k cols
    std::vector<std::vector<uint8_t>> t_maxtrix, u_matrix;
    t_maxtrix.resize(m, std::vector<uint8_t>(k, 0));
    u_matrix.resize(m, std::vector<uint8_t>(k, 0));

    // iterate over the rows --> at i-th row
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < k; ++j) {
            t_maxtrix[i][j] = rand() % 2; // random bit
        }
    }

    // construct u matrix
    // for row i -> u_i = t_i XOR choice_bit_i
    for (size_t i = 0; i < m; ++i) {
        uint8_t c = choice_bits[i];
        for (size_t j = 0; j < k; ++j) {
            u_matrix[i][j] = t_maxtrix[i][j] ^ c;
        }
    }
}
///////////////////////////////////
// OT Extension Sender
///////////////////////////////////

void OTExtensionSender::initialize_base_ots() {
    base_ot_receivers.reserve(this->security_parameter);

    for (size_t i = 0; i < this->security_parameter; ++i) {
        base_ot_receivers.push_back(std::make_unique<BaseOTSender>());
    }

}