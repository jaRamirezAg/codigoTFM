#include <iostream>
#include <ros/package.h>
#include <string>
#include <SpeechFsm.h>
#include <iomanip>

using namespace std;

SpeechFsm::SpeechFsm() : QObject()
{
	m_subscriber_wakeword = m_handler.subscribe("/matrix/wakeword", 1000, &SpeechFsm::wakewordCallBack,this);  
	
	m_subscriber_mqtt = m_handler.subscribe("/mqtt", 1000, &SpeechFsm::mqttCallBack,this);
	m_subscriber_ready_to_speak = m_handler.subscribe("/mqtt/ready_to_speak", 1000, &SpeechFsm::mqttReadyToSpeakCallBack,this);
	m_subscriber_received_ok = m_handler.subscribe("/mqtt/received_ok", 1000, &SpeechFsm::mqttReceivedOkCallBack, this);  
	m_subscriber_stop_timer = m_handler.subscribe("/mqtt/stop_wait_for_finish", 1000, &SpeechFsm::mqttStopTimer, this);  

	m_client_play = m_handler.serviceClient<speak_node::Play>("play_service");
	m_client_stop = m_handler.serviceClient<speak_node::Stop>("stop_service");
	m_client_volume = m_handler.serviceClient<speak_node::Volume>("volume_service");
	m_client_speak = m_handler.serviceClient<speak_node::Speak>("speak_service");
	m_client_matrix_light = m_handler.serviceClient<matrix_node::MatrixLight>("matrix/light");
	

	m_ros_timer.setSingleShot(false);	
	m_ros_timer.setInterval(100); 	
	connect(&m_ros_timer, &QTimer::timeout, this, &SpeechFsm::updateROS);
	
	m_ros_timer.start();
	
	 	
	QState *parent_state = new QState(QState::ExclusiveStates);
	m_play_noise_state = new PlayNoise(parent_state);
	m_lights_red_state = new LightsRed(parent_state);
	m_wait_for_ww_state = new WaitForWw(parent_state);
	m_lights_off_state = new LightsOff(parent_state);
	m_wake_to_assistant_state = new WakeToAssistant(parent_state);
	m_lights_green_state = new LightsGreen(parent_state);
	m_wait_for_finish_state = new WaitForFinish(parent_state);
	
	m_play_ww_state_synthetic = new PlayNativeWw(parent_state);
	m_invoke_app_state = new InvokeApp(parent_state);

	connect(m_play_ww_state_synthetic, &PlayNativeWw::readyToSpeak, this, &SpeechFsm::readyToSpeak);	
	connect(m_play_noise_state, &PlayNoise::stopBlink, this, &SpeechFsm::stopReadyToSpeak);

	m_play_noise_state->addTransition(m_play_noise_state, &PlayNoise::playingNoise, m_lights_red_state);
	m_lights_red_state->addTransition(m_lights_red_state, &LightsRed::lightsOk, m_wait_for_ww_state);
	m_wait_for_ww_state->addTransition(this, &SpeechFsm::wwArrived, m_lights_off_state);
	m_lights_off_state->addTransition(m_lights_off_state, &LightsOff::lightsOk, m_wake_to_assistant_state);
	m_wake_to_assistant_state->addTransition(m_wake_to_assistant_state, &WakeToAssistant::assistantWokeup, m_lights_green_state);
	m_lights_green_state->addTransition(m_lights_green_state, &LightsGreen::lightsOk, m_wait_for_finish_state);
	m_lights_green_state->addTransition(this, &SpeechFsm::wwArrived, m_lights_off_state);
	m_wait_for_finish_state->addTransition(this, &SpeechFsm::wwArrived, m_lights_off_state);
	m_wait_for_finish_state->addTransition(this, &SpeechFsm::finished, m_play_noise_state);

	m_wait_for_ww_state->addTransition(this, &SpeechFsm::wwSpeak, m_play_ww_state_synthetic);
	m_play_ww_state_synthetic->addTransition(m_play_ww_state_synthetic, &PlayNativeWw::playingNativeWw, m_invoke_app_state);
	m_invoke_app_state->addTransition(m_invoke_app_state, &InvokeApp::errorSignal, m_play_noise_state);
	m_invoke_app_state->addTransition(this, &SpeechFsm::tofinishSignal, m_wait_for_finish_state);
	
	
	

	m_wait_for_finish_state->addTransition(m_wait_for_finish_state, &WaitForFinish::noneIntent, m_play_noise_state);

	
	//connect(m_wait_for_finish_state, &WaitForFinish::waitTimeout, this, &SpeechFsm::waitTimeoutFunction);

    m_machine.addState(m_play_noise_state);
	m_machine.addState(m_lights_red_state);
	m_machine.addState(m_wait_for_ww_state);
	m_machine.addState(m_lights_off_state);
	m_machine.addState(m_wake_to_assistant_state);
	m_machine.addState(m_lights_green_state);
	m_machine.addState(m_wait_for_finish_state);
	m_machine.addState(m_play_ww_state_synthetic);
	m_machine.addState(m_invoke_app_state);

	
	m_machine.setInitialState(m_play_noise_state);
	m_machine.start();
	
}

void SpeechFsm::updateROS()
{
	
	ros::spinOnce();
}


 void SpeechFsm::wakewordCallBack(const matrix_node::MatrixWakeWord &msg)
{
	emit wwArrived();	

} 

void SpeechFsm::mqttCallBack(const std_msgs::UInt32::ConstPtr& msg)
{
	uint32_t time = msg->data;
	m_wait_for_finish_state->setExpirationTime(time);
	cout << "estamos en mqttCallBack" << endl;
	//emit finished();

}

void SpeechFsm::mqttReadyToSpeakCallBack(const mqtt_node::EndConversation& msg)
{
	bool end_conversation = msg.end_conversation;
	if(end_conversation)
	{
		m_invoke_app_state->setPhrase(std::string("hablar con locucion"));
	}
	else
	{
		m_invoke_app_state->setPhrase(std::string("hablar con el sistema"));
	}
	emit wwSpeak();	
}

void SpeechFsm::mqttReceivedOkCallBack(const std_msgs::String::ConstPtr& msg)
{
	cout << "estamos en mqttReceivedOkCallBack" << endl;
//	m_wait_for_finish_state->stopTimer();
	emit tofinishSignal();
}

void SpeechFsm::mqttStopTimer(std_msgs::Empty empty)
{
	cout << "estamos en mqttStopTimer" << endl;
	m_wait_for_finish_state->stopTimer();

}

void SpeechFsm::waitTimeoutFunction()
{
	//m_timer.start();
}

void SpeechFsm::readyToSpeak()
{
	
	m_ready_to_speak_thread = std::thread([this]() {
		uint8_t intensity = 0;
		bool is_increment = true;
		m_is_ready_to_speak = true;
		
		ros::NodeHandle n;
		ros::ServiceClient client = n.serviceClient<matrix_node::MatrixLight>("/matrix/light");
		matrix_node::MatrixLight srv;
		srv.request.r = 0;
		srv.request.g = 0;
		srv.request.b = 255;
		
		while(m_is_ready_to_speak)
		{
			if(is_increment)
			{
				intensity += 10;
				if(intensity >= 100)
				{
					intensity = 100;
					is_increment = false;
				}
			}
			else
			{
				intensity -= 10;
				if(intensity <= 0)
				{
					intensity = 0;
					is_increment = true;
				}
			}
			
			
			srv.request.intensity = intensity;
			client.call(srv);
			
			this_thread::sleep_for (chrono::milliseconds(20));
				
		}
	});
	
}

void SpeechFsm::stopReadyToSpeak()
{
	m_is_ready_to_speak = false;
	if(m_ready_to_speak_thread.joinable())
	{
		m_ready_to_speak_thread.join();
	}
}

SpeechFsm::~SpeechFsm()
{
	delete  m_play_noise_state;
	
}



