#include "../../include/version1/UspAsyncServer.h"


namespace smart_home::usp_server {

    UspAsyncServer::UspAsyncServer(
        const UspServerConfig &config,
        const HandlerFunction &onRequest
    )
        : UspServer(config, onRequest)
    {}

    void UspAsyncServer::sendRequest(
        const UspServerRequest&,
        const web_server::NetServerClientInfo&,
        ResponseReactionFunction
    ) {
        std::cout << "Trying to send request" << std::endl;
    }

    void UspAsyncServer::tryReceiveMessage(
        const timeval& timeout
    ) {
        auto buffer = new char[MessageSettings::MAX_PACKET_SIZE];
        const web_server::NetServerClientInfo client = netServer.receiveMessage(
            MessageSettings::MAX_PACKET_SIZE,
            buffer,
            timeout
        );

        if (client.isSuccessful) {
            std::cout << "Received message" << std::endl;
        } else {
            std::cout << "Failed to receive message" << std::endl;
        }
    }

} // namespace smart_home::usp_server