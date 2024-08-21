#include <iostream>
#include "librdkafka/rdkafkacpp.h"
using namespace std;

class MessageHandler {
    public:
        MessageHandler(const string& chatName, const string& connectName);
        ~MessageHandler();
        void sendMessage(const string& message);
        bool receiveMessage(string& message);
    private:
        string d_chatName;
        string d_connectName;
        RdKafka::Topic *d_sendTopic;
        RdKafka::Topic *d_receiveTopic;
        string d_broker;
        RdKafka::Producer *d_chatProducer;
        RdKafka::Consumer *d_chatConsumer;
};