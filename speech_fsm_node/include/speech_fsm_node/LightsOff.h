#ifndef LIGHTSOFF_H
#define LIGHTSOFF_H

#include "ros/ros.h"
#include <iostream>
#include <string>
#include <QState>


class LightsOff : public QState
{
	Q_OBJECT
public:
			
	LightsOff(QState *parent);
	
	~LightsOff();
	
protected:

	 virtual void onEntry(QEvent *event);
	
signals:

	void lightsOk();	
};


#endif // LIGHTSOFF_H