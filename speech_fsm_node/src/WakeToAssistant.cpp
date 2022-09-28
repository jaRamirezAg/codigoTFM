#include "WakeToAssistant.h"
#include <ros/ros.h>
#include <speak_node/Play.h>
#include <speak_node/Volume.h>
#include <speak_node/AudioCodes.h>
#include <matrix_node/MatrixOneLight.h>
#include <matrix_node/MatrixLight.h>

using namespace std;

const float WW_VOLUME = 0.4;

WakeToAssistant::WakeToAssistant(QState *parent) : QState(parent)
{	
	m_timer_wait_to_assistant.setSingleShot(true);	
	m_timer_wait_to_assistant.setInterval(5000);
	connect(&m_timer_wait_to_assistant, &QTimer::timeout, this, &WakeToAssistant::timerWaitToAssistantFinished);
	
}

void WakeToAssistant::onEntry(QEvent *event)
{
	cout << "WakeToAssistant" << endl;
		
	loading();
	
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<speak_node::Play>("/play_service");
	ros::ServiceClient client_volume = n.serviceClient<speak_node::Volume>("/volume_service");
	m_control_relay = n.advertise<std_msgs::Bool>("speaker/switch", 1000); 

	speak_node::Volume srv_volume;
	srv_volume.request.level = WW_VOLUME;
	client_volume.call(srv_volume);
	
	speak_node::Play srv;
	srv.request.cod_audio = static_cast<uint8_t>(AudioCode::ok_google);
	srv.request.infinite = false;
	srv.request.sync = true;
	client.call(srv);
	
	calculateTimer();

}

void WakeToAssistant::onExit(QEvent *event)
{
	m_loading_start = false;
	m_loading_thread.join();
}	

void WakeToAssistant::timerWaitToAssistantFinished()
{
	m_state.data = true;
	m_control_relay.publish(m_state);
	
	ros::NodeHandle n;
	ros::ServiceClient client_beep = n.serviceClient<speak_node::Play>("/play_service");
	ros::ServiceClient client_volume_beep = n.serviceClient<speak_node::Volume>("/volume_service");
	
	speak_node::Volume srv_volume_beep;
	srv_volume_beep.request.level = 1.0;
	client_volume_beep.call(srv_volume_beep);

	speak_node::Play srv_beep;
	srv_beep.request.cod_audio = static_cast<uint8_t>(AudioCode::beep);
	srv_beep.request.infinite = false;
	srv_beep.request.sync = true;
	client_beep.call(srv_beep);
	
	emit assistantWokeup();	
}

void WakeToAssistant::loading()
{
	m_loading_start = true;
	m_loading_thread = std::thread([this]() {
		
		int intensity;	
		intensity = 20;
		m_is_increment = true;	
		m_value = 255;
		m_index = 0;
		
		while(m_loading_start)
		{
			if(m_index >= LEDS_NUMBER)
			{
				m_index = 0;
				if(m_value == 255)
					{
						m_value = 0;
					}
				else
					{
						m_value = 255;
					}
				if(m_value == 0)
				{
					intensity = 0;
				}
				else
				{
					intensity = 20;
				}
			}
		
		ros::NodeHandle n;
		ros::ServiceClient client = n.serviceClient<matrix_node::MatrixOneLight>("/matrix/one_light");
		matrix_node::MatrixOneLight srv;
		srv.request.r = 0;
		srv.request.g = 0;
		srv.request.b = m_value;
		srv.request.intensity = intensity;
		srv.request.index = m_index;
		client.call(srv);
		m_index++;
		this_thread::sleep_for (chrono::milliseconds(55));
	
		}	
	});
}


void WakeToAssistant::calculateTimer()
{
	string app_name = "el sistema";
	string prev_phrase = "Perfecto, voy a buscarte la útima versión de " + app_name;
	uint32_t duration = (prev_phrase.length() * 104);
	m_timer_wait_to_assistant.setInterval(duration);	
	m_timer_wait_to_assistant.start();
	m_state.data = false;
	m_control_relay.publish(m_state);	
}




WakeToAssistant::~WakeToAssistant()
{
	
}