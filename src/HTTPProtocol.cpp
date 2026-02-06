#include "HTTPProtocol.h"

#include "utils.h"

std::vector<Request> HTTPProtocol::parseData(const std::string &buffer)
{
    prevBuffer += buffer;
    std::vector<Request> requests;
    while (!prevBuffer.empty()) {
        Request request;
        size_t headerEnd = prevBuffer.find("\r\n\r\n");
        if (headerEnd == std::string::npos) {
            break;
        }

        std::string headerBlock = prevBuffer.substr(0, headerEnd);
        std::istringstream headerStream(headerBlock);
        std::string line;

        if (!std::getline(headerStream, (line)))
            continue;
        {
            std::istringstream requestLine(trim_2(line));
            requestLine >> request.method >> request.uri;
        }
        // Headers
        while (std::getline(headerStream, line)) {
            auto colonPos = line.find(':');
            if (colonPos == std::string::npos) {
                continue;
            }

            std::string key = toLower(trim_2(line.substr(0, colonPos)));
            std::string value = trim_2(line.substr(colonPos + 1));
            request.headers[key] = value;
        }
        // Body
        size_t contentLength = 0;
        auto it = request.headers.find("content-length");

        if (it != request.headers.end()) {
            contentLength = std::stoul(it->second);
        }


        size_t totalSize = headerEnd + contentLength + 4;

        if (prevBuffer.size() < totalSize) {
            break;
        }

        std::string body = prevBuffer.substr(headerEnd + 4, contentLength);


        prevBuffer.erase(0, totalSize);


        if (contentLength > 0 && request.getHeader("content-type") != "application/json") {
                request.errorCode = 415;
                request.error = "Only application/json type allowed in body";
                request.isValid = false;
        }
        else {
            if (!body.empty()) {
                try {
                    request.jsonMessage = JSON::loads(body);
                    request.isValid = true;
                }catch (std::exception &e) {
                    request.isValid = false;
                    request.error = e.what();
                }
            }
            else {
                request.isValid = true;
            }
        }
        requests.push_back(request);
    }

    return requests;
}

std::string HTTPProtocol::prepareResponse(const Response &response) {
    std::ostringstream responseStream;

    responseStream << "HTTP/1.1 " << response.statusCode << " " << getStatusMessage(response.statusCode) << "\r\n";
    for (const auto&[key, value]: response.headers) {
        responseStream << key << ": " << value << "\r\n";
    }
    responseStream << "Content-Type: application/json\r\n";
    const std::string body = response.body.is_null()? "":response.body.dump();
    responseStream << "Content-Length: " << body.size() << "\r\n";
    responseStream << "\r\n";
    responseStream << body;

    return responseStream.str();
}

std::string HTTPProtocol::getStatusMessage(const unsigned int code) {
    switch (code) {
        case 200: return "OK";
        case 201: return "Created";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 415: return "Unsupported Content Type";
        case 500: return "Internal Server Error";
        default: return "UNKNOWN";
    };
}
