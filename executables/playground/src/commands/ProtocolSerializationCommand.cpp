#include "../../include/commands/ProtocolSerializationCommand.h"

#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessage.h>
#include <smart_home/usp_protocol/include/version1/acknowledgement/AcknowledgementMessageHandler.h>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessage.h>
#include <smart_home/usp_protocol/include/version1/protocol/ProtocolMessageHandler.h>
#include <smart_home/usp_protocol/include/version1/request/RequestMessage.h>
#include <smart_home/usp_protocol/include/version1/request/RequestMessageHandler.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessageHandler.h>
#include <smart_home/usp_protocol/include/version1/response/ResponseMessage.h>

#include <iostream>
#include <ostream>

namespace smart_home::playground::commands {
    using namespace usp_protocol;

    int ProtocolSerializationCommand::execute(int, char*[]) {
        ProtocolVersion protocolVersion = ProtocolVersion::VERSION_1;
        std::string sessionId = "sessionId";
        uint64_t timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string requestId = "requestId";

        std::string dataToSerialize;
        std::cout << "Enter data to serialize: ";
        std::cin >> dataToSerialize;
        std::cout << std::endl;
        size_t size = dataToSerialize.length();

        // Acknowledgement requests serialization/deserialization
        version1::AcknowledgementMessage ackMessage {
            protocolVersion,
            22233,
            timestamp,
            22233,
            version1::AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS,
            size,
            dataToSerialize
        };
        version1::AcknowledgementMessageHandler ackMessageHandler;
        std::unique_ptr<version1::AcknowledgementSerializationResult> ackSerializationResult = ackMessageHandler.serialize(&ackMessage);
        std::unique_ptr<version1::AcknowledgementDeserializationResult> ackDeserializationResult = ackMessageHandler.deserialize(ackSerializationResult->getSerializationState());

        std::cout << "Acknowledgement serialization: " << ackSerializationResult->getSerializationState() << std::endl;
        std::cout << "Acknowledgement deserialization: " << ackDeserializationResult->getDeserializationState()->data << std::endl;

        // Request requests serialization/deserialization
        version1::RequestMessage reqMessage {
            protocolVersion,
            22233,
            timestamp,
            22233,
            1,
            0,
            "",
            0x01,
            0x01,
            size,
            dataToSerialize
        };
        version1::RequestMessageHandler reqMessageHandler;
        std::unique_ptr<version1::RequestSerializationResult> reqSerializationResult = reqMessageHandler.serialize(&reqMessage);
        std::unique_ptr<version1::RequestDeserializationResult> reqDeserializationResult = reqMessageHandler.deserialize(reqSerializationResult->getSerializationState());

        std::cout << "Request serialization: " << reqSerializationResult->getSerializationState() << std::endl;
        std::cout << "Request deserialization: " << reqDeserializationResult->getDeserializationState()->data << std::endl;

        // Response requests serialization/deserialization
        version1::ResponseMessage respMessage {
            protocolVersion,
            22233,
            timestamp,
            22233,
            1,
            0,
            version1::ResponseStatus::STATUS_OK,
            size,
            dataToSerialize
        };
        version1::ResponseMessageHandler respMessageHandler;
        std::unique_ptr<version1::ResponseSerializationResult> respSerializationResult = respMessageHandler.serialize(&respMessage);
        std::unique_ptr<version1::ResponseDeserializationResult> respDeserializationResult = respMessageHandler.deserialize(respSerializationResult->getSerializationState());

        std::cout << "Response serialization: " << respSerializationResult->getSerializationState() << std::endl;
        std::cout << "Response deserialization: " << respDeserializationResult->getDeserializationState()->data << std::endl;

        // Protocol requests serialization/deserialization
        version1::ProtocolMessage protoMessage {
            protocolVersion,
            22233,
            0,
            22233,
            version1::ProtocolAction::ACTION_HANDSHAKE,
            size,
            dataToSerialize
        };
        version1::ProtocolMessageHandler protoMessageHandler;
        std::unique_ptr<version1::ProtocolSerializationResult> protoSerializationResult = protoMessageHandler.serialize(&protoMessage);
        std::unique_ptr<version1::ProtocolDeserializationResult> protoDeserializationResult = protoMessageHandler.deserialize(protoSerializationResult->getSerializationState());

        std::cout << "Protocol serialization: " << protoSerializationResult->getSerializationState() << std::endl;
        std::cout << "Protocol deserialization: " << protoDeserializationResult->getDeserializationState()->data << std::endl;

        return 0;
    }

    void ProtocolSerializationCommand::present() const {
        std::cout << "Protocol Serialization Command: "
                     "Demonstrates serialization and deserialization process of a data with "
                     "a format that will be used to travel over the net " << std::endl;
    }
} // namespace smart_home::playground