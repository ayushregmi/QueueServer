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
        getMessagesFromQueue(res, req);
    };
    this->operationMap["delete_message"] = [this](const JSON &res, JSON &req)
    {
        deleteMessageFromQueue(res, req);
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
    std::string queueName = getQueueName(req);
    if (queueName == "")
    {
        res["Error"] = "Queue name missing";
        return;
    }

    auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' does not exist";
        return;
    }

    std::string messages = q->second.getMessagesFromQueue(10);

    res["Messages"] = messages;
}

void QueueManager::addMessageToQueue(const JSON &req, JSON &res)
{

    std::string queueName = getQueueName(req);
    if (queueName == "")
    {
        res["Error"] = "Queue name missing";
        return;
    }

    auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' does not exist";
        return;
    }

    if (!req.contains("data"))
    {
        res["Error"] = "Data is missing";
        return;
    }

    std::string data = req["data"];

    q->second.addMessageToQueue(data);

    res["Success"] = "Message added to queue";
}

void QueueManager::deleteMessageFromQueue(const JSON &req, JSON &res)
{
    std::string queueName = getQueueName(req);
    if (queueName == "")
    {
        res["Error"] = "Queue name missing";
        return;
    }

    if (!req.contains("messageId"))
    {
        res["Error"] = "Missing error Id";
        return;
    }

    auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' does not exist";
        return;
    }

    std::string messageId = req["messageId"];

    if (!q->second.deleteMessageFromQueue(messageId))
    {
        res["Error"] = "Queue is empty or Invalid message id ";
        return;
    }

    res["Success"] = "Deleted message from queue";
}

std::string QueueManager::getQueueName(const JSON &message)
{
    return message.contains("queue") ? message["queue"] : "";
}
