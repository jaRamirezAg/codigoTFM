#ifndef SPEECHFSM_H
#define SPEECHFSM_H

#include <chrono>
#include <thread>
#include <stdint.h>
#include <QTimer>
#include "ros/ros.h"
#include <QObject>
#include <iostream>
#include <string>
#include "std_msgs/String.h"
#include "matrix_node/MatrixWakeWord.h"
#include "matrix_node/MatrixLight.h"
#include "mqtt_node/EndConversation.h"
#include "speak_node/Play.h"
#include "speak_node/Volume.h"
#include "speak_node/Stop.h"
#include "speak_node/Speak.h"
#include <QtCore>
#include "PlayNoise.h"
#include "LightsRed.h"
#include "WaitForWw.h"
#include "LightsOff.h"
#include "WakeToAssistant.h"
#include "LightsGreen.h"
#include "WaitForFinish.h"
#include "PlayNativeWw.h"
#include "InvokeApp.h"
#include "std_msgs/UInt32.h"
#include "std_msgs/Empty.h"



class SpeechFsm : public QObject
{
	Q_OBJECT
	
public:
			
	SpeechFsm();
	
	~SpeechFsm();
	
private:


	void updateROS();
 	void wakewordCallBack(const matrix_node::MatrixWakeWord &msg);
	void mqttCallBack(const std_msgs::UInt32::ConstPtr& msg);
	void mqttReadyToSpeakCallBack(const mqtt_node::EndConversation& msg);
	void mqttReceivedOkCallBack(const std_msgs::String::ConstPtr& msg);
	void mqttStopTimer(std_msgs::Empty empty);


	void readyToSpeak();
	void stopReadyToSpeak();
	void waitTimeoutFunction();
 	
	
	std::atomic<bool> m_is_ready_to_speak;
	std::thread m_ready_to_speak_thread;
	
	uint8_t m_intensity;
	bool m_is_increment;
	

	QTimer m_ros_timer;


	ros::NodeHandle m_handler;
	ros::Subscriber m_subscriber_wakeword;
	ros::Subscriber m_subscriber_mqtt;
	ros::Subscriber m_subscriber_ready_to_speak;	
	ros::Subscriber m_subscriber_received_ok;		
	ros::Subscriber m_subscriber_stop_timer;		
	
	
	ros::ServiceClient m_client_play, m_client_stop, m_client_volume, m_client_speak, m_client_matrix_light;
	speak_node::Play m_srv_Play;
	speak_node::Stop m_srv_Stop;
	speak_node::Volume m_srv_Volume;
	speak_node::Speak m_srv_Speak;
    matrix_node::MatrixLight m_srv_Matrix_Light;
	
	QStateMachine m_machine;
	PlayNoise *m_play_noise_state;  
	LightsRed *m_lights_red_state;
	WaitForWw *m_wait_for_ww_state;
	LightsOff *m_lights_off_state;
	WakeToAssistant *m_wake_to_assistant_state;
	LightsGreen *m_lights_green_state;
	WaitForFinish *m_wait_for_finish_state;
	PlayNativeWw *m_play_ww_state_synthetic;
	InvokeApp *m_invoke_app_state;

signals:

	void wwArrived();
	void finished();
	void wwSpeak();
	void nativeWwFinished();
	void tofinishSignal();

};


#endif // SPEECHFSM.H