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
    std::unordered_map<std::string, std::function<int(const JSON &, JSON &)>> operationMap;

    const size_t maxNumberOfQueues = 5;
    size_t numOfQueues = 0;

    int createNewQueue(const JSON &, JSON &);
    int getMessagesFromQueue(const JSON &, JSON &);
    int addMessageToQueue(const JSON &, JSON &);
    int deleteMessageFromQueue(const JSON &, JSON &);

    static std::string getQueueName(const JSON &message);

public:
    QueueManager();
    ~QueueManager() {};


    int createNewQueue(const std::string &, JSON &);
    int getMessagesFromQueue(const std::string&,JSON &);
    int addMessageToQueue(const std::string&,const JSON &, JSON &);
    int deleteMessageFromQueue(const std::string&, const JSON &, JSON &);

    int handleRequest(const JSON &, JSON &);
};

#endif