#include "messagehandler.h"

MessageHandler*MessageHandler::messageHandler=0;

void MessageHandler::messageDispatcher(QtMsgType type, const char *msg) {
	if (messageHandler) messageHandler->handleMessage(type,msg);
}

void MessageHandler::installMessageHandler(MessageHandler*handler) {
	if (handler) qInstallMsgHandler(MessageHandler::messageDispatcher);
	else qInstallMsgHandler(0);
	messageHandler=handler;
}
