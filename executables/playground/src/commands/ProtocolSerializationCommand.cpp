#include "../../include/commands/ProtocolSerializationCommand.h"

#include <../../../../smart_home/usp_protocol/include/messages/AcknowledgementMessage.h>
#include <../../../../smart_home/usp_protocol/include/messages/ResponseMessage.h>
#include <smart_home/usp_protocol/include/ProtocolMessage.h>
#include <smart_home/usp_protocol/include/RequestMessage.h>

#include <iostream>
#include <ostream>

namespace smart_home::playground::commands {
    using namespace usp_protocol;

    int ProtocolSerializationCommand::execute() {
        std::string dataToSerialize;
        std::cout << "Enter data to serialize: ";
        std::cin >> dataToSerialize;
        std::cout << std::endl;

        AcknowledgementMessage ackMessage { dataToSerialize };
        AcknowledgementMessageHandler ackMessageHandler;
        std::unique_ptr<AcknowledgementSerializationResult> ackSerializationResult = ackMessageHandler.serialize(&ackMessage);
        std::unique_ptr<AcknowledgementDeserializationResult> ackDeserializationResult = ackMessageHandler.deserialize(ackSerializationResult->getSerializationState());

        std::cout << "Acknowledgement serialization: " << ackSerializationResult->getSerializationState() << std::endl;
        std::cout << "Acknowledgement deserialization: " << ackDeserializationResult->getDeserializationState()->getData() << std::endl;

        ResponseMessage respMessage { dataToSerialize };
        ResponseMessageHandler respMessageHandler;
        std::unique_ptr<ResponseSerializationResult> respSerializationResult = respMessageHandler.serialize(&respMessage);
        std::unique_ptr<ResponseDeserializationResult> respDeserializationResult = respMessageHandler.deserialize(respSerializationResult->getSerializationState());

        std::cout << "Response serialization: " << respSerializationResult->getSerializationState() << std::endl;
        std::cout << "Response deserialization: " << respDeserializationResult->getDeserializationState()->getData() << std::endl;

        RequestMessage reqMessage { dataToSerialize };
        RequestMessageHandler reqMessageHandler;
        std::unique_ptr<RequestSerializationResult> reqSerializationResult = reqMessageHandler.serialize(&reqMessage);
        std::unique_ptr<RequestDeserializationResult> reqDeserializationResult = reqMessageHandler.deserialize(reqSerializationResult->getSerializationState());

        std::cout << "Request serialization: " << reqSerializationResult->getSerializationState() << std::endl;
        std::cout << "Request deserialization: " << reqDeserializationResult->getDeserializationState()->getData() << std::endl;

        ProtocolMessage protoMessage { dataToSerialize };
        ProtocolMessageHandler protoMessageHandler;
        std::unique_ptr<ProtocolSerializationResult> protoSerializationResult = protoMessageHandler.serialize(&protoMessage);
        std::unique_ptr<ProtocolDeserializationResult> protoDeserializationResult = protoMessageHandler.deserialize(protoSerializationResult->getSerializationState());

        std::cout << "Protocol serialization: " << protoSerializationResult->getSerializationState() << std::endl;
        std::cout << "Protocol deserialization: " << protoDeserializationResult->getDeserializationState()->getData() << std::endl;

        return 0;
    }

    void ProtocolSerializationCommand::present() const {
        std::cout << "Protocol Serialization Command: "
                     "Demonstrates serialization and deserialization process of a data with "
                     "a format that will be used to travel over the net " << std::endl;
    }
} // namespace smart_home::playground