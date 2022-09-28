#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <stdint.h>
#include <QTimer>
#include "ros/ros.h"
#include <QObject>
#include "speak_node/Play.h"
#include "speak_node/Volume.h"
#include "speak_node/Stop.h"
#include "speak_node/Speak.h"
#include <iostream>
#include <QSound>
#include <string>
#include <QSoundEffect>
#include "QCoreApplication"


class AudioManager : public QObject
{

public:
			
	AudioManager();
	
	~AudioManager();
	
private:


	void updateROS();
	bool playSound(speak_node::Play::Request &req, speak_node::Play::Response &res);
	bool volumeSound(speak_node::Volume::Request &req, speak_node::Volume::Response &res);
    bool stopSound(speak_node::Stop::Request &req, speak_node::Stop::Response &res);
	bool speakSound(speak_node::Speak::Request &req, speak_node::Speak::Response &res);


	QTimer m_ros_timer;
	ros::NodeHandle m_handler;
	ros::ServiceServer m_service;
	ros::ServiceServer m_service_volume;
	ros::ServiceServer m_service_stop;	
	ros::ServiceServer m_service_speak;	

	QSoundEffect m_player;
	std::string m_resources_path;
};


#endif // AUDIO_MANAGER_H
