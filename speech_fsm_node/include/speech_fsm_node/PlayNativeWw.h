#ifndef PLAYNATIVE_H
#define PLAYNATIVE_H

#include <iostream>
#include <string>
#include <QState>
#include <QObject>
#include "ros/ros.h"
#include <QTimer>



class PlayNativeWw : public QState
{
	Q_OBJECT
public:
			
	PlayNativeWw(QState *parent);
	
	~PlayNativeWw();
	
protected:

	virtual void onEntry(QEvent *event);	 
	ros::NodeHandle m_handler;
	QTimer m_timer_play_native_ww;
	

	
signals:

	void playingNativeWw();
	void readyToSpeak();

};


#endif // PLAYNATIVE_H
