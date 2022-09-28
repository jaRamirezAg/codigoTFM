#include "InvokeApp.h"
#include <ros/ros.h>
#include <speak_node/Play.h>
#include <speak_node/Speak.h>
#include <speak_node/Volume.h>
#include <speak_node/AudioCodes.h>

using namespace std;


InvokeApp::InvokeApp(QState *parent) : QState(parent), m_invoke_phrase("")
{
	m_timer_invoke_app.setSingleShot(true);	
	m_timer_invoke_app.setInterval(7000);
	connect(&m_timer_invoke_app, &QTimer::timeout, this, &InvokeApp::errorSignal);
	
	m_timer_wait_to_assistant.setSingleShot(true);	
	m_timer_wait_to_assistant.setInterval(5000);
	connect(&m_timer_wait_to_assistant, &QTimer::timeout, this, &InvokeApp::timerWaitToAssistantFinished);

//	m_subscriber = m_handler.subscribe("/mqtt/received_ok", 1000, &InvokeApp::receivedCallBack, this);  

}


void InvokeApp::onEntry(QEvent *event)
{
	cout << "InvokeApp" << endl;
	
	ros::NodeHandle n;
	ros::ServiceClient client_speak = n.serviceClient<speak_node::Speak>("/speak_service");
	m_control_relay = n.advertise<std_msgs::Bool>("speaker/switch", 1000); 
	
	speak_node::Speak srv_speak;
	srv_speak.request.phrase = m_invoke_phrase;
	std::cout<<m_invoke_phrase<<std::endl;
	client_speak.call(srv_speak);
	
	calculateTimer();
	m_timer_invoke_app.start();

	
}

void InvokeApp::timerWaitToAssistantFinished()
{
	m_state.data = true;
	m_control_relay.publish(m_state);	
	
}

void InvokeApp::calculateTimer()
{
	string app_name;
	if(m_invoke_phrase == "hablar con el sistema")
	{
		app_name = "el sistema";
	}
	else
	{
		app_name = "locucion";
	}
	string prev_phrase = "Perfecto, voy a buscarte la útima versión de " + app_name;
	uint32_t duration = (prev_phrase.length() * 104);
	m_timer_wait_to_assistant.setInterval(duration);	
	m_timer_wait_to_assistant.start();
	m_state.data = false;
	m_control_relay.publish(m_state);	
}

InvokeApp::~InvokeApp()
{
	
}

void InvokeApp::setPhrase(const std::string &phrase)
{
	m_invoke_phrase = phrase;
}


