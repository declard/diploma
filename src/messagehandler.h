#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QtGlobal>

class MessageHandler {
	static MessageHandler*messageHandler;
public:
	virtual void handleMessage(QtMsgType type,const char*msg)=0;
	static void messageDispatcher(QtMsgType type, const char *msg);
	static void installMessageHandler(MessageHandler*handler);
};

#endif // MESSAGEHANDLER_H
