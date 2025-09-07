#include "../../include/commands/ProtocolSerializationCommand.h"

#include <smart_home/usp_protocol/include/messages/AcknowledgementMessage.h>
#include <smart_home/usp_protocol/include/messages/ProtocolMessage.h>
#include <smart_home/usp_protocol/include/messages/RequestMessage.h>
#include <smart_home/usp_protocol/include/messages/ResponseMessage.h>
#include <smart_home/usp_protocol/include/handlers/AcknowledgementMessageHandler.h>
#include <smart_home/usp_protocol/include/handlers/ProtocolMessageHandler.h>
#include <smart_home/usp_protocol/include/handlers/RequestMessageHandler.h>
#include <smart_home/usp_protocol/include/handlers/ResponseMessageHandler.h>

#include <iostream>
#include <ostream>


namespace smart_home::playground::commands {
    using namespace usp_protocol;

    int ProtocolSerializationCommand::execute() {
        messages::ProtocolVersion protocolVersion = messages::ProtocolVersion::VERSION_1;
        std::string sessionId = "sessionId";
        time_t timestamp = time(nullptr);
        std::string requestId = "requestId";

        std::string dataToSerialize;
        std::cout << "Enter data to serialize: ";
        std::cin >> dataToSerialize;
        std::cout << std::endl;
        size_t size = dataToSerialize.length();

        // Acknowledgement requests serialization/deserialization
        messages::AcknowledgementMessage ackMessage {
            protocolVersion,
            sessionId,
            timestamp,
            requestId,
            messages::AcknowledgementStatus::ACKNOWLEDGEMENT_SUCCESS,
            size,
            dataToSerialize
        };
        handlers::AcknowledgementMessageHandler ackMessageHandler;
        std::unique_ptr<handlers::AcknowledgementSerializationResult> ackSerializationResult = ackMessageHandler.serialize(&ackMessage);
        std::unique_ptr<handlers::AcknowledgementDeserializationResult> ackDeserializationResult = ackMessageHandler.deserialize(ackSerializationResult->getSerializationState());

        std::cout << "Acknowledgement serialization: " << ackSerializationResult->getSerializationState() << std::endl;
        std::cout << "Acknowledgement deserialization: " << ackDeserializationResult->getDeserializationState()->data << std::endl;

        // Request requests serialization/deserialization
        messages::RequestMessage reqMessage {
            protocolVersion,
            sessionId,
            timestamp,
            1,
            0,
            requestId,
            "",
            0x01,
            0x01,
            size,
            dataToSerialize
        };
        handlers::RequestMessageHandler reqMessageHandler;
        std::unique_ptr<handlers::RequestSerializationResult> reqSerializationResult = reqMessageHandler.serialize(&reqMessage);
        std::unique_ptr<handlers::RequestDeserializationResult> reqDeserializationResult = reqMessageHandler.deserialize(reqSerializationResult->getSerializationState());

        std::cout << "Request serialization: " << reqSerializationResult->getSerializationState() << std::endl;
        std::cout << "Request deserialization: " << reqDeserializationResult->getDeserializationState()->data << std::endl;

        // Response requests serialization/deserialization
        messages::ResponseMessage respMessage {
            protocolVersion,
            sessionId,
            timestamp,
            1,
            0,
            requestId,
            messages::ResponseStatus::STATUS_OK,
            size,
            dataToSerialize
        };
        handlers::ResponseMessageHandler respMessageHandler;
        std::unique_ptr<handlers::ResponseSerializationResult> respSerializationResult = respMessageHandler.serialize(&respMessage);
        std::unique_ptr<handlers::ResponseDeserializationResult> respDeserializationResult = respMessageHandler.deserialize(respSerializationResult->getSerializationState());

        std::cout << "Response serialization: " << respSerializationResult->getSerializationState() << std::endl;
        std::cout << "Response deserialization: " << respDeserializationResult->getDeserializationState()->data << std::endl;

        // Protocol requests serialization/deserialization
        messages::ProtocolMessage protoMessage {
            protocolVersion,
            sessionId,
            timestamp,
            messages::ProtocolAction::ACTION_HANDSHAKE,
            size,
            dataToSerialize
        };
        handlers::ProtocolMessageHandler protoMessageHandler;
        std::unique_ptr<handlers::ProtocolSerializationResult> protoSerializationResult = protoMessageHandler.serialize(&protoMessage);
        std::unique_ptr<handlers::ProtocolDeserializationResult> protoDeserializationResult = protoMessageHandler.deserialize(protoSerializationResult->getSerializationState());

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