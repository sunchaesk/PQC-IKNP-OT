
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

///////////////////////////////////
// OT Extension Sender
///////////////////////////////////

void OTExtensionSender::initialize_base_ots() {
    base_ot_receivers.reserve(this->security_parameter);

    for (size_t i = 0; i < this->security_parameter; ++i) {
        base_ot_receivers.push_back(std::make_unique<BaseOTSender>());
    }

}