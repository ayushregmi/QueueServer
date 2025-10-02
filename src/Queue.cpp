#include "Queue.h"

Queue::Queue(std::string queueName)
{
    this->queueName = queueName;
}

Queue::~Queue()
{
}

bool Queue::addMessageToQueue(std::string data)
{
    QueueMessage queueMessage;

    std::string messageId = std::to_string(generateMessageId());
    auto now = std::chrono::system_clock::now();

    queueMessage.receivedDate = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    queueMessage.content = data;
    queueMessage.messageId = messageId;
    this->messagesMap[messageId] = queueMessage;

    this->messagesOrder.push_back(messageId);

    return true;
}

const std::string Queue::getMessagesFromQueue(size_t numberOfMessages)
{
    JSON jsonMessage = new std::vector<JSON>();

    if (messagesOrder.size() < numberOfMessages)
    {
        numberOfMessages = messagesOrder.size();
    }
    for (size_t i = 0; i < numberOfMessages; i++)
    {
        JSON msg;
        QueueMessage message = messagesMap[this->messagesOrder[i]];

        msg["messageId"] = std::string(message.messageId);
        msg["receivedDate"] = epochToUTCString(message.receivedDate);
        msg["content"] = message.content;

        jsonMessage.as_array().push_back(msg);
    }
    return jsonMessage.dump();
}

// bool Queue::deleteMessageFromQueue(const std::string &)
// {

// }
