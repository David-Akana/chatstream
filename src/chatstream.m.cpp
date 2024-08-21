#include <iostream>
#include <librdkafka/rdkafka.h>
#include <glib.h>
#include "message_handler.h"

using namespace std;


int main(){
    std::cout << "WELCOME TO CHATSTREAM!\n";

    string chatName;
    std::cout << "Please enter your session username: ";
    std::getline(std::cin, chatName);
    std::cout << "\nWelcome " << chatName << "!\n";

    string connectName;
    std::cout << "Please enter the username you want to connect with: ";
    std::getline(std::cin, connectName);
    std::cout << "\nSetting up chat with " << connectName << "...\n";

    MessageHandler messageHandler(chatName, connectName);
    std::cout << "\nChat Started" << std::endl;

    string message;
    if(messageHandler.receiveMessage(message)){
        std::cout << connectName << ": " << message << std::endl;
    }

    while(true){
        string userMessage;
        std::cout << chatName << ": ";
        std::getline(std::cin, userMessage);
        messageHandler.sendMessage(userMessage);

        while(true){
            bool res = messageHandler.receiveMessage(message);
            if(res){
                std::cout << connectName << ": " << message << std::endl;
                break;
            }
        }
    }

    return 0;
}
