#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H

#include <unordered_map>
#include <string>
#include <vector>

class Queue;

class QueueManager
{
private:
    std::unordered_map<std::string, Queue> queueMap;
    const size_t maxNumberOfQueues = 5;

public:
    QueueManager() {};
    ~QueueManager() {};

    void createNewQueue(const std::string &queueName);
    const std::vector<std::string> getMessagesFromQueue(const std::string &queueName);
    void addMessageToQueue(const std::string &queueName, const std::string &message);
    void deleteQueue(const std::string &queueName);
};

#endif