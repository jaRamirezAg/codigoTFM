#ifndef INVOKEAPP_H
#define INVOKEAPP_H

#include <iostream>
#include <string>
#include <QState>
#include <QObject>
#include "ros/ros.h"
#include <QTimer>
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"




class InvokeApp : public QState
{
	Q_OBJECT
public:
			
	InvokeApp(QState *parent);
	
	~InvokeApp();
	
	void setPhrase(const std::string &phrase);
	
protected:

	virtual void onEntry(QEvent *event);	
	void timerWaitToAssistantFinished();
	void calculateTimer();
	
	ros::NodeHandle m_handler;
	ros::Subscriber m_subscriber;
	ros::Publisher m_control_relay;
	
	std_msgs::Bool m_state;

	QTimer m_timer_invoke_app;
	QTimer m_timer_wait_to_assistant;
	
signals:

	void errorSignal();
	void tofinishSignal();
	
private:
	
	std::string m_invoke_phrase;

};


#endif // INVOKEAPP_H
