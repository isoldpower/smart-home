#include "../../include/version1/ReferencedCommonData.h"


namespace smart_home::usp_server::version1 {

    ReferencedCommonData::ReferencedCommonData(
        const usp_protocol::ProtocolVersion& protocolVersion,
        const uint16_t& sessionId,
        const usp_protocol::version1::MessageType& messageType,
        const uint64_t& timestamp,
        const uint16_t& requestId,
        const size_t packetIndex,
        const size_t packetsCount,
        const char* rawDataReference,
        const UspServerClient &client
    )
        : usp_protocol::version1::CommonMessagePacketData(
            protocolVersion,
            sessionId,
            messageType,
            timestamp,
            requestId,
            packetIndex,
            packetsCount
        )
        , reference(rawDataReference)
        , client(client)
    {}

    ReferencedCommonData::ReferencedCommonData(
        const usp_protocol::version1::CommonMessagePacketData &commonData,
        const char* rawDataReference,
        const UspServerClient& client
    )
        : usp_protocol::version1::CommonMessagePacketData(commonData)
        , reference(rawDataReference)
        , client(client)
    {}

    char* ReferencedCommonData::getRawDataReference() const {
        return const_cast<char*>(reference);
    }

    UspServerClient ReferencedCommonData::getClient() const {
        return client;
    }
} // namespace smart_home::usp_server::version1