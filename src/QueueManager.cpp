#include "QueueManager.h"

std::string QueueManager::getQueueName(const JSON &message)
{
    return message.contains("queue") ? message["queue"] : "";
}


int QueueManager::createNewQueue(const std::string &queueName, JSON & resp) {
    if (this->numOfQueues == this->maxNumberOfQueues)
    {
        resp["Error"] = "Cannot create more queues. Maximum number of queues are already created.";
        return 400;
    }

    if (queueMap.find(queueName) != queueMap.end())
    {
        resp["Error"] = "Queue with name '" + queueName + "' already exists";
        return 400;
    }

    Queue newQueue(queueName);
    this->queueMap[queueName] = newQueue;
    this->numOfQueues += 1;

    resp["Success"] = "Queue created with name '" + queueName + "'";
    return 201;
}

int QueueManager::getMessagesFromQueue(const std::string& queueName,JSON & res) {

    const auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' does not exist";
        return 400;
    }

    const JSON messages = q->second.getMessagesFromQueue(10);
    res["Messages"] = messages;
    return 200;
}

int QueueManager::addMessageToQueue(const std::string& queueName,const JSON & req, JSON & resp) {
    const auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        resp["Error"] = "Queue with name '" + queueName + "' does not exist";
        return 400;
    }

    if (!req.contains("data"))
    {
        resp["Error"] = "Data is missing";
        return 400;
    }

    JSON data = req["data"];

    q->second.addMessageToQueue(data);

    resp["Success"] = "Message added to queue";
    return 200;
}

int QueueManager::deleteMessageFromQueue(const std::string& queueName, const JSON &req, JSON &res) {

    if (!req.contains("messageId"))
    {
        res["Error"] = "Missing error Id";
        return 400;
    }

    auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' does not exist";
        return 400;
    }

    std::string messageId = req["messageId"];

    if (!q->second.deleteMessageFromQueue(messageId))
    {
        res["Error"] = "Queue is empty or Invalid message id ";
        return 400;
    }
    res["Success"] = "Deleted message from queue";
    return 200;
}

