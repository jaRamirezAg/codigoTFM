#ifndef WAKETOASSISTANT_H
#define WAKETOASSISTANT_H

#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include <iostream>
#include <string>
#include <QState>
#include <QTimer>
#include <thread>

#define LEDS_NUMBER 18

class WakeToAssistant : public QState
{
	Q_OBJECT
public:
			
	WakeToAssistant(QState *parent);
	
	~WakeToAssistant();
	
protected:

	 virtual void onEntry(QEvent *event);
	 void onExit(QEvent *event);	

	 void timerWaitToAssistantFinished();
	 void loading();
	 void calculateTimer();
	 
	 QTimer m_timer_wait_to_assistant;
	 
	 ros::Publisher m_control_relay;
	 std_msgs::Bool m_state;
	
	 std::atomic<bool> m_loading_start;
	 std::thread m_loading_thread;
	
	// Blink
	 uint8_t m_intensity;
	 bool m_is_increment;

	// Loading
	 uint8_t m_value;
	 uint8_t m_index;

signals:

	void assistantWokeup();
	 
};


#endif // WAKETOASSISTANT_H