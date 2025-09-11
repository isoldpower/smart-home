#include "../../include/commands/ProtocolSerializationCommand.h"

#include <smart_home/usp_protocol/include/handlers/version1/acknowledgement/AcknowledgementMessage.h>
#include <smart_home/usp_protocol/include/handlers/version1/acknowledgement/AcknowledgementMessageHandler.h>
#include <smart_home/usp_protocol/include/handlers/version1/protocol/ProtocolMessage.h>
#include <smart_home/usp_protocol/include/handlers/version1/protocol/ProtocolMessageHandler.h>
#include <smart_home/usp_protocol/include/handlers/version1/request/RequestMessage.h>
#include <smart_home/usp_protocol/include/handlers/version1/request/RequestMessageHandler.h>
#include <smart_home/usp_protocol/include/handlers/version1/response/ResponseMessageHandler.h>
#include <smart_home/usp_protocol/include/handlers/version1/response/ResponseMessage.h>

#include <iostream>
#include <ostream>

namespace smart_home::playground::commands {
    using namespace usp_protocol;

    int ProtocolSerializationCommand::execute() {
        handlers::ProtocolVersion protocolVersion = handlers::ProtocolVersion::VERSION_1;
        std::string sessionId = "sessionId";
        time_t timestamp = time(nullptr);
        std::string requestId = "requestId";

        std::string dataToSerialize;
        std::cout << "Enter data to serialize: ";
        std::cin >> dataToSerialize;
        std::cout << std::endl;
        size_t size = dataToSerialize.length();

        // Acknowledgement requests serialization/deserialization
        handlers::version1::AcknowledgementMessage ackMessage {
            protocolVersion,
            sessionId,
            timestamp,
            requestId,
            handlers::version1::AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS,
            size,
            dataToSerialize
        };
        handlers::version1::AcknowledgementMessageHandler ackMessageHandler;
        std::unique_ptr<handlers::version1::AcknowledgementSerializationResult> ackSerializationResult = ackMessageHandler.serialize(&ackMessage);
        std::unique_ptr<handlers::version1::AcknowledgementDeserializationResult> ackDeserializationResult = ackMessageHandler.deserialize(ackSerializationResult->getSerializationState());

        std::cout << "Acknowledgement serialization: " << ackSerializationResult->getSerializationState() << std::endl;
        std::cout << "Acknowledgement deserialization: " << ackDeserializationResult->getDeserializationState()->data << std::endl;

        // Request requests serialization/deserialization
        handlers::version1::RequestMessage reqMessage {
            protocolVersion,
            sessionId,
            timestamp,
            requestId,
            1,
            0,
            "",
            0x01,
            0x01,
            size,
            dataToSerialize
        };
        handlers::version1::RequestMessageHandler reqMessageHandler;
        std::unique_ptr<handlers::version1::RequestSerializationResult> reqSerializationResult = reqMessageHandler.serialize(&reqMessage);
        std::unique_ptr<handlers::version1::RequestDeserializationResult> reqDeserializationResult = reqMessageHandler.deserialize(reqSerializationResult->getSerializationState());

        std::cout << "Request serialization: " << reqSerializationResult->getSerializationState() << std::endl;
        std::cout << "Request deserialization: " << reqDeserializationResult->getDeserializationState()->data << std::endl;

        // Response requests serialization/deserialization
        handlers::version1::ResponseMessage respMessage {
            protocolVersion,
            sessionId,
            timestamp,
            requestId,
            1,
            0,
            handlers::version1::ResponseStatus::STATUS_OK,
            size,
            dataToSerialize
        };
        handlers::version1::ResponseMessageHandler respMessageHandler;
        std::unique_ptr<handlers::version1::ResponseSerializationResult> respSerializationResult = respMessageHandler.serialize(&respMessage);
        std::unique_ptr<handlers::version1::ResponseDeserializationResult> respDeserializationResult = respMessageHandler.deserialize(respSerializationResult->getSerializationState());

        std::cout << "Response serialization: " << respSerializationResult->getSerializationState() << std::endl;
        std::cout << "Response deserialization: " << respDeserializationResult->getDeserializationState()->data << std::endl;

        // Protocol requests serialization/deserialization
        handlers::version1::ProtocolMessage protoMessage {
            protocolVersion,
            sessionId,
            timestamp,
            requestId,
            handlers::version1::ProtocolAction::ACTION_HANDSHAKE,
            size,
            dataToSerialize
        };
        handlers::version1::ProtocolMessageHandler protoMessageHandler;
        std::unique_ptr<handlers::version1::ProtocolSerializationResult> protoSerializationResult = protoMessageHandler.serialize(&protoMessage);
        std::unique_ptr<handlers::version1::ProtocolDeserializationResult> protoDeserializationResult = protoMessageHandler.deserialize(protoSerializationResult->getSerializationState());

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