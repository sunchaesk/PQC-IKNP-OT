#ifndef EXTENSION_H
#define EXTENSION_H

#include "ot.h"

#include <unistd.h>
#include <cstdint>
#include <vector>
#include <memory>

#include <assert.h>


class OTExtensionReceiver {
private:
    uint32_t security_parameter;
    uint32_t num_ots;

    std::vector<std::unique_ptr<BaseOTReceiver>> base_ot_receivers;
    std::vector<uint8_t> choice_bits;

public:
    OTExtensionReceiver(uint32_t security_parameter, 
                        uint32_t num_ots,
                        const std::vector<uint8_t>& choice_bits)
     : security_parameter(security_parameter),
       num_ots(num_ots),
       choice_bits(choice_bits) {

        initialize_base_ots();
        generate_random_matrices_t_u();

        assert(choice_bits.size() == num_ots);

    }

    uint32_t get_security_parameter() const {
        return security_parameter;
    }

    void initialize_base_ots();
    void generate_random_matrices_t_u();
};

class OTExtensionSender {
private:
    uint32_t security_parameter;
    uint32_t num_ots;
    std::vector<std::unique_ptr<BaseOTSender>> base_ot_receivers;
public:
    OTExtensionSender(uint32_t security_parameter, uint32_t num_ots)
     : security_parameter(security_parameter),
       num_ots(num_ots) {

        initialize_base_ots();

    }

    uint32_t get_security_parameter() const {
        return security_parameter;
    }

    void initialize_base_ots();
};

#endif

