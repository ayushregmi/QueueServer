#ifndef QUEUE_H
#define QUEUE_H

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
    Queue(std::string queueName);
    Queue() {};
    ~Queue();

    bool addMessageToQueue(std::string);
    const std::string getMessagesFromQueue(size_t);
    bool deleteMessageFromQueue(const std::string &);
};

#endif