#ifndef PLAY_NOISE_H
#define PLAY_NOISE_H

#include <iostream>
#include <string>
#include <QState>
#include <QObject>
#include "ros/ros.h"


class PlayNoise : public QState
{
	Q_OBJECT
public:
			
	PlayNoise(QState *parent);
	
	~PlayNoise();
	
protected:

	virtual void onEntry(QEvent *event);
	 
	ros::NodeHandle m_handler;
	
signals:

	void playingNoise();
	void stopBlink();

};


#endif // PLAY_NOISE_H