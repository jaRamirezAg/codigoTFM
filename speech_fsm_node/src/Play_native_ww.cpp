#include "Play_native_ww.h"
#include <ros/ros.h>
#include <speak_node/Play.h>
#include <speak_node/Speak.h>
#include <speak_node/Volume.h>
#include <speak_node/Stop.h>

#include <speak_node/AudioCodes.h>

using namespace std;


Play_native_ww::Play_native_ww(QState *parent) : QState(parent)
{
	m_timer_play_native_ww.setSingleShot(true);	
	m_timer_play_native_ww.setInterval(500);
	connect(&m_timer_play_native_ww, &QTimer::timeout, this, &Play_native_ww::playingNativeWw);

}


void Play_native_ww::onEntry(QEvent *event)
{
	cout << "Play_native_ww" << endl;
	
	emit readyToSpeak();
	
	ros::NodeHandle n;
	ros::ServiceClient client_speak = n.serviceClient<speak_node::Speak>("/speak_service");
	ros::ServiceClient client_stop = n.serviceClient<speak_node::Stop>("/stop_service");
	
	speak_node::Stop srv_stop;
	client_stop.call(srv_stop);
	
	
	speak_node::Speak srv_speak;
	srv_speak.request.phrase = "okei guguel";
	client_speak.call(srv_speak);
	
	m_timer_play_native_ww.start();
		
/* 	speak_node::Play srv;
	srv.request.cod_audio = static_cast<uint8_t>(AudioCode::noise);
	srv.request.infinite = true;
	srv.request.sync = false;
	client.call(srv); */
	
	
}


Play_native_ww::~Play_native_ww()
{
	
}
