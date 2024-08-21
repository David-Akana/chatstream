#include "message_handler.h"


MessageHandler::MessageHandler(const string& chatName, const string& connectName):
    d_chatName(chatName),
    d_connectName(connectName),
    d_broker("localhost:9092")
{
    RdKafka::Conf *conf  = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
    string errstr;

    conf->set("metadata.broker.list", d_broker, errstr);
    conf->set("default_topic_conf", tconf, errstr);

    d_chatProducer = RdKafka::Producer::create(conf, errstr);
    if (!d_chatProducer) {
      std::cerr << "Failed to create producer: " << errstr << std::endl;
    }

    const string sendTopicStr = chatName + '-' + connectName;
    d_sendTopic = RdKafka::Topic::create(d_chatProducer, sendTopicStr, tconf, errstr);
    if (!d_sendTopic) {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
    }

    // conf->set("enable.partition.eof", "true", errstr);
    d_chatConsumer = RdKafka::Consumer::create(conf, errstr);
    if (!d_chatConsumer) {
      std::cerr << "Failed to create consumer: " << errstr << std::endl;
      exit(1);
    }

    const string receiveTopicStr = connectName + '-' + chatName;
    d_receiveTopic = RdKafka::Topic::create(d_chatConsumer, receiveTopicStr, tconf, errstr);
    if (!d_receiveTopic) {
        std::cerr << "Failed to create topic: " << errstr << std::endl;
    }


    // Start consumer
    int32_t partition = 0;
    int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;

    RdKafka::ErrorCode resp = d_chatConsumer->start(d_receiveTopic, partition, start_offset);
    if (resp != RdKafka::ERR_NO_ERROR) {
      std::cerr << "Failed to start consumer: " << RdKafka::err2str(resp)
                << std::endl;
    }
}

MessageHandler::~MessageHandler(){
    d_chatProducer->poll(1000);
    d_chatConsumer->poll(1000);
    delete d_sendTopic;
    delete d_receiveTopic;
    delete d_chatProducer;
    delete d_chatConsumer;
}

void MessageHandler::sendMessage(const string& message){
    int32_t partition = RdKafka::Topic::PARTITION_UA;

    RdKafka::ErrorCode resp = 
        d_chatProducer->produce(
            d_sendTopic->name(),
            partition,
            RdKafka::Producer::RK_MSG_COPY,
            const_cast<char *>(message.c_str()),
            message.size(),
            NULL,
            0,
            0,
            NULL);

        d_chatProducer->poll(0);
}


bool MessageHandler::receiveMessage(string& message){
    RdKafka::Message *msg = d_chatConsumer->consume(d_receiveTopic, 0, 1000);

    if(msg->len() == 0){
        return false;
    }

    message = static_cast<const char* >(msg->payload());
    d_chatConsumer->poll(10);
    return true;
}
