#include "Server.h"
#include "Logger.h"

int main()
{
    Server server(1234);
    server.run();
    return 0;
}