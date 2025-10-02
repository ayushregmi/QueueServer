#include "QueueManager.h"

QueueManager::QueueManager()
{
    this->operationMap["create_queue"] = [this](const JSON &res, JSON &req)
    {
        createNewQueue(res, req);
    };
    this->operationMap["add_message"] = [this](const JSON &res, JSON &req)
    {
        addMessageToQueue(res, req);
    };
    this->operationMap["get_message"] = [this](const JSON &res, JSON &req)
    {
        addMessageToQueue(res, req);
    };
    this->operationMap["delete_queue"] = [this](const JSON &res, JSON &req)
    {
        addMessageToQueue(res, req);
    };
}

JSON QueueManager::handleMessage(const JSON &message)
{
    JSON rtnMessage;

    if (!message.contains("operation"))
    {
        rtnMessage["Error"] = "Operation Missing";
        return rtnMessage;
    }

    std::string operation = message["operation"];

    auto operationHandler = operationMap.find(operation);

    if (operationHandler == operationMap.end())
    {
        rtnMessage["Error"] = "Invalid Operation";
    }
    else
    {
        operationHandler->second(message, rtnMessage);
    }

    return rtnMessage;
}

void QueueManager::createNewQueue(const JSON &req, JSON &res)
{
    if (this->numOfQueues == this->maxNumberOfQueues)
    {
        res["Error"] = "Cannot create more queues. Maximum number of queues are already created.";
        return;
    }

    std::string queueName = getQueueName(req);
    if (queueName == "")
    {
        res["Error"] = "Queue name missing";
        return;
    }

    if (queueMap.find(queueName) != queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' already exists";
        return;
    }

    Queue newQueue(queueName);
    this->queueMap[queueName] = newQueue;
    this->numOfQueues += 1;

    res["Success"] = "Queue created with name '" + queueName + "'";
}

void QueueManager::getMessagesFromQueue(const JSON &req, JSON &res)
{
    Logger::log("QueueManager", req.dump());
    res["Success"] = "Message aayo hai";
}

void QueueManager::addMessageToQueue(const JSON &req, JSON &res)
{
    Logger::log("QueueManager", req.dump());
    res["Success"] = "Message add vayo hai";
}

void QueueManager::deleteQueue(const JSON &req, JSON &res)
{
    Logger::log("QueueManager", req.dump());
    res["Success"] = "Queue Delete vayo hai";
}

std::string QueueManager::getQueueName(const JSON &message)
{
    return message.contains("queue") ? message["queue"] : "";
}
