#ifndef PLAY_NATIVE_H
#define PLAY_NATIVE_H

#include <iostream>
#include <string>
#include <QState>
#include <QObject>
#include "ros/ros.h"
#include <QTimer>



class Play_native_ww : public QState
{
	Q_OBJECT
public:
			
	Play_native_ww(QState *parent);
	
	~Play_native_ww();
	
protected:

	virtual void onEntry(QEvent *event);	 
	ros::NodeHandle m_handler;
	QTimer m_timer_play_native_ww;
	

	
signals:

	void playingNativeWw();
	void readyToSpeak();

};


#endif // PLAY_NATIVE_H
