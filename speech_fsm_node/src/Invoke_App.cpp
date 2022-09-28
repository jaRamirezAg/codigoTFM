#include "Invoke_App.h"
#include <ros/ros.h>
#include <speak_node/Play.h>
#include <speak_node/Speak.h>
#include <speak_node/Volume.h>
#include <speak_node/AudioCodes.h>

using namespace std;


Invoke_App::Invoke_App(QState *parent) : QState(parent)
{
	m_timer_invoke_app.setSingleShot(true);	
	m_timer_invoke_app.setInterval(7000);
	connect(&m_timer_invoke_app, &QTimer::timeout, this, &Invoke_App::errorSignal);

//	m_subscriber = m_handler.subscribe("/mqtt/received_ok", 1000, &Invoke_App::receivedCallBack, this);  

}


void Invoke_App::onEntry(QEvent *event)
{
	cout << "Invoke_App" << endl;
	
	ros::NodeHandle n;
	ros::ServiceClient client_speak = n.serviceClient<speak_node::Speak>("/speak_service");
	
	speak_node::Speak srv_speak;
	srv_speak.request.phrase = "hablar con el sistema";
	client_speak.call(srv_speak);
	
	m_timer_invoke_app.start();

		
}


/* void Invoke_App::receivedCallBack(const std_msgs::String::ConstPtr& msg)      
{
	emit tofinishSignal();
} */



Invoke_App::~Invoke_App()
{
	
}
