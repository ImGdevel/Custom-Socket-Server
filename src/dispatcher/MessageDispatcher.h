#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <utility>
#include "../session/ClientSession.h"

using HandlerFnc = std::function<void(const std::shared_ptr<ClientSession>&, const std::string&)>;

class MessageDispatcher {
public:
    void registerHandler(const std::string& type, HandlerFnc handler);
    void handleEvent(const std::shared_ptr<ClientSession>& session, const std::string& message);
    std::pair<std::string, std::string> extractMessageTypeAndContent(const std::string& message) const;

private:
    std::unordered_map<std::string, HandlerFnc> handlers;
};

#endif
