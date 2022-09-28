#include "AudioManager.h"
#include <iostream>
#include "AudioCodes.h"
#include <ros/package.h>
#include <string>
#include <festival/festival.h>
#include <sys/types.h>


using namespace std;


AudioManager::AudioManager() 
: QObject()
, m_resources_path{ros::package::getPath("speak_node") + "/resources"}
{
	festival_initialize(1, 210000);
	festival_init_lang("spanish");
	
	m_service = m_handler.advertiseService("play_service", &AudioManager::playSound, this);
	m_service_volume = m_handler.advertiseService("volume_service", &AudioManager::volumeSound, this);
	m_service_stop = m_handler.advertiseService("stop_service", &AudioManager::stopSound, this);
	m_service_speak = m_handler.advertiseService("speak_service", &AudioManager::speakSound, this);

	m_ros_timer.setSingleShot(false);
	m_ros_timer.setInterval(100);
	
	connect(&m_ros_timer, &QTimer::timeout, this, &AudioManager::updateROS);
	
	m_ros_timer.start();
}

void AudioManager::updateROS()
{
	
	ros::spinOnce();
}

bool AudioManager::playSound(speak_node::Play::Request  &req, speak_node::Play::Response &res)
{
	cout << m_resources_path << endl;
	
	res.result = true;
	res.error_code = 200;
	bool sync = req.sync;
	cout << "SYNC: " << sync << endl;
   
	switch(static_cast<AudioCode>(req.cod_audio))
	{
		case AudioCode::ok_google:
			 cout << "OK Google" << endl;
			 m_player.setSource(QUrl::fromLocalFile(QString(m_resources_path.c_str()) + QString("/ok_google.wav")));
			 m_player.setLoopCount(req.infinite ? m_player.Infinite : 1);
			 m_player.play();
			 break;
			 
		case AudioCode::alexa:
			 cout << "Alexa" << endl;
			 m_player.setSource(QUrl::fromLocalFile(QString(m_resources_path.c_str()) + QString("/hey_alexa.wav")));
			 m_player.setLoopCount(req.infinite ? m_player.Infinite : 1);
             m_player.play();		
			 break;
			 
		case AudioCode::echo:
			 cout << "Echo" << endl;
			 break;
		case AudioCode::noise:
			 m_player.setSource(QUrl::fromLocalFile(QString(m_resources_path.c_str()) + QString("/noise.wav")));
			 m_player.setLoopCount(req.infinite ? m_player.Infinite : 1);
			 m_player.play();
			 break; 
		case AudioCode::beep:
			 system(" ( speaker-test -t sine -f 1000 )& pid=$! ; sleep 0.1s ; kill -9 $pid");
			 break;
		default:
			res.result = false;
			res.error_code = 500;
			m_player.setLoopCount(1);
			cout << "Error, code not found" << endl;
	}
	
	while(sync && m_player.isPlaying())
	{
		//qApp->processEvents();
		 QCoreApplication::processEvents();
	}		
	cout << "return" << endl;
	return true;
}


bool AudioManager::volumeSound(speak_node::Volume::Request &req, speak_node::Volume::Response &res)
{
	m_player.setVolume(req.level);
	res.result = true;
	
	return true;
}


bool AudioManager::stopSound(speak_node::Stop::Request &req, speak_node::Stop::Response &res)
{
	m_player.stop();
	res.result = true;
	
	return true;
}

bool AudioManager::speakSound(speak_node::Speak::Request &req, speak_node::Speak::Response &res)
{
	EST_String phrase(req.phrase.c_str());
	festival_say_text(phrase);
	res.result = true;
	return true;
}



AudioManager::~AudioManager()
{
	
}
