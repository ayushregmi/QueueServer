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

    static std::string getQueueName(const JSON &message);

public:
    QueueManager() = default;
    ~QueueManager() = default;


    int createNewQueue(const std::string &, JSON &);
    int getMessagesFromQueue(const std::string&,JSON &);
    int addMessageToQueue(const std::string&,const JSON &, JSON &);
    int deleteMessageFromQueue(const std::string&, const JSON &, JSON &);

};

#endif