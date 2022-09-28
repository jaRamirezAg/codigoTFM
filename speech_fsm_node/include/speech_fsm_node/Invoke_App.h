#ifndef INVOKE_APP_H
#define INVOKE_APP_H

#include <iostream>
#include <string>
#include <QState>
#include <QObject>
#include "ros/ros.h"
#include <QTimer>
#include "std_msgs/String.h"



class Invoke_App : public QState
{
	Q_OBJECT
public:
			
	Invoke_App(QState *parent);
	
	~Invoke_App();
	
protected:

	virtual void onEntry(QEvent *event);	
//	void receivedCallBack(const std_msgs::String::ConstPtr& msg);
	
	ros::NodeHandle m_handler;
	ros::Subscriber m_subscriber;

	QTimer m_timer_invoke_app;
	

	
signals:

	void errorSignal();
	void tofinishSignal();

};


#endif // INVOKE_APP_H
