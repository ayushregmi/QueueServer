#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

#include "json.h"
#include "Logger.h"
#include "Queue.h"
#include "utils.h"

class QueueManager
{
private:
    std::unordered_map<std::string, Queue> queueMap;
    std::unordered_map<std::string, std::function<void(const JSON &, JSON &)>> operationMap;

    const size_t maxNumberOfQueues = 5;
    size_t numOfQueues = 0;

    void createNewQueue(const JSON &, JSON &);
    void getMessagesFromQueue(const JSON &, JSON &);
    void addMessageToQueue(const JSON &, JSON &);
    void deleteQueue(const JSON &, JSON &);

    static std::string getQueueName(const JSON &message);

public:
    QueueManager();
    ~QueueManager() {};

    JSON handleMessage(const JSON &);
};

#endif