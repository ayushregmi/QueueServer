#ifndef QUEUE_H
#define QUEUE_H

#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <chrono>
#include <algorithm>

#include "json.h"
#include "utils.h"

struct QueueMessage
{
    unsigned long long receivedDate;
    JSON content;
    std::string messageId;
};
class Queue
{
private:
    std::vector<std::string> messagesOrder;
    std::unordered_map<std::string, QueueMessage> messagesMap;
    std::string queueName;

public:
    explicit Queue(std::string  queueName): queueName(std::move(queueName)){};
    Queue() = default;
    ~Queue() = default;

    bool addMessageToQueue(const JSON&);
    JSON getMessagesFromQueue(size_t);
    bool deleteMessageFromQueue(const std::string &);
};

#endif