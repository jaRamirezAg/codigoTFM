#ifndef LIGHTSRED_H
#define LIGHTSRED_H

#include "ros/ros.h"
#include <iostream>
#include <string>
#include <QState>
#include <QObject>


class LightsRed : public QState
{
	Q_OBJECT

public:
			
	LightsRed(QState *parent);
	
	~LightsRed();
	
protected:

	 virtual void onEntry(QEvent *event);

signals:

	void lightsOk();
	 
};


#endif // PLAY_NOISE_H