#include "QueueManager.h"

QueueManager::QueueManager()
{
    this->operationMap["create_queue"] = [this](const JSON &res, JSON &req)
    {
        return createNewQueue(res, req);
    };
    this->operationMap["add_message"] = [this](const JSON &res, JSON &req)
    {
        return addMessageToQueue(res, req);
    };
    this->operationMap["get_message"] = [this](const JSON &res, JSON &req)
    {
        return getMessagesFromQueue(res, req);
    };
    this->operationMap["delete_message"] = [this](const JSON &res, JSON &req)
    {
        return deleteMessageFromQueue(res, req);
    };
}

int QueueManager::handleRequest(const JSON &request, JSON &response)
{
    if (!request.contains("operation"))
    {
        response["Error"] = "Operation Missing";
        return 404;
    }

    std::string operation = request["operation"];

    auto operationHandler = operationMap.find(operation);

    if (operationHandler == operationMap.end())
    {
        response["Error"] = "Invalid Operation";
        return 400;
    }
    return operationHandler->second(request, response);
}

int QueueManager::createNewQueue(const JSON &req, JSON &res)
{
    if (this->numOfQueues == this->maxNumberOfQueues)
    {
        res["Error"] = "Cannot create more queues. Maximum number of queues are already created.";
        return 400;
    }

    const std::string queueName = getQueueName(req);
    if (queueName == "")
    {
        res["Error"] = "Queue name missing";
        return 404;
    }

    if (queueMap.find(queueName) != queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' already exists";
        return 400;
    }

    Queue newQueue(queueName);
    this->queueMap[queueName] = newQueue;
    this->numOfQueues += 1;

    res["Success"] = "Queue created with name '" + queueName + "'";
    return 201;
}

int QueueManager::getMessagesFromQueue(const JSON &req, JSON &res)
{
    std::string queueName = getQueueName(req);
    if (queueName.empty())
    {
        res["Error"] = "Queue name missing";
        return 404;
    }

    const auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' does not exist";
        return 400;
    }

    std::string messages = q->second.getMessagesFromQueue(10);

    res["Messages"] = messages;
    return 200;
}

int QueueManager::addMessageToQueue(const JSON &req, JSON &res)
{

    std::string queueName = getQueueName(req);
    if (queueName.empty())
    {
        res["Error"] = "Queue name missing";
        return 404;
    }

    auto q = queueMap.find(queueName);

    if (q == queueMap.end())
    {
        res["Error"] = "Queue with name '" + queueName + "' does not exist";
        return 400;
    }

    if (!req.contains("data"))
    {
        res["Error"] = "Data is missing";
        return 400;
    }

    std::string data = req["data"];

    q->second.addMessageToQueue(data);

    res["Success"] = "Message added to queue";
    return 200;
}

int QueueManager::deleteMessageFromQueue(const JSON &req, JSON &res)
{
    std::string queueName = getQueueName(req);
    if (queueName.empty())
    {
        res["Error"] = "Queue name missing";
        return 404;
    }

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

    const std::string messages = q->second.getMessagesFromQueue(10);
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

    std::string data = req["data"];

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

