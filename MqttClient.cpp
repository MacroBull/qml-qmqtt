
#include "MqttClient.h"

MqttClient::MqttClient(QObject *parent) :
    QObject(parent)
{
	m_port = 1883;
	m_qos = 0;
//	m_keepalive = 60;
    // re-initialize connection whenever the
	// host or the port changes
	QObject::connect(this, SIGNAL(hostChanged(QString)),
			this, SLOT(initializeConnection()));
	QObject::connect(this, SIGNAL(portChanged(int)),
			this, SLOT(initializeConnection()));
}


void MqttClient::initializeConnection()
{
    DEBUG;
    if(m_host.isEmpty() || !m_port || m_topic.isEmpty())
    {
		DEBUG << "No host, port and topic is defined, initial suspended";
//        disconnect();
        return;
    }

    this->client = new QMQTT::Client(m_host, m_port);
	this->client->setClientId(QString(QUuid::createUuid().toString()));

//    this->client->setUsername("user");
//    this->client->setPassword("password");

    QObject::connect(this->client, SIGNAL(received(const QMQTT::Message&)),
            this, SLOT(processReceivedMessage(const QMQTT::Message&)));
    QObject::connect(this->client, SIGNAL(connacked(quint8)),
                     this, SLOT(subscribeToTopic(quint8)));
    QObject::connect(this->client, SIGNAL(subscribed(QString)),
                     this, SLOT(subscribedToTopic(QString)));
	QObject::connect(this->client, SIGNAL(connected()),
					 this, SIGNAL(connected()));
    QObject::connect(this->client, SIGNAL(disconnected()),
                     this, SIGNAL(disconnected()));

	this->client->connect();
}

