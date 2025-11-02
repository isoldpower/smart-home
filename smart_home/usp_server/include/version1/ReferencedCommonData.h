#pragma once

#include <smart_home/usp_protocol/include/version1/CommonMessageData.h>
#include <smart_home/usp_protocol/include/CommonOptions.h>

#include "../UspServerClient.h"


namespace smart_home::usp_server::version1 {

    struct ReferencedCommonData : public usp_protocol::version1::CommonMessagePacketData {
    private:
        const char* reference;
        const UspServerClient client;
    public:
        [[nodiscard]] char* getRawDataReference() const;
        [[nodiscard]] UspServerClient getClient() const;

        ReferencedCommonData(
            const usp_protocol::ProtocolVersion& protocolVersion,
            const uint16_t& sessionId,
            const usp_protocol::version1::MessageType& messageType,
            const uint64_t& timestamp,
            const uint16_t& requestId,
            size_t packetIndex,
            size_t packetsCount,
            const char* rawDataReference,
            const UspServerClient &client
        );

        ReferencedCommonData(
            const usp_protocol::version1::CommonMessagePacketData &commonData,
            const char* rawDataReference,
            const UspServerClient& client
        );


    };
} // namespace smart_home::usp_server::version1