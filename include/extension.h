#ifndef EXTENSION_H
#define EXTENSION_H

#include "ot.h"

#include <unistd.h>
#include <cstdint>
#include <vector>
#include <memory>

class OTExtensionReceiver {
private:
    uint32_t security_parameter;
    uint32_t num_ots;

    std::vector<std::unique_ptr<BaseOTReceiver>> base_ot_receivers;

public:
    OTExtensionReceiver(uint32_t security_parameter, uint32_t num_ots)
     : security_parameter(security_parameter),
       num_ots(num_ots) {

        initialize_base_ots();
    }

    uint32_t get_security_parameter() const {
        return security_parameter;
    }

    void initialize_base_ots();
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

