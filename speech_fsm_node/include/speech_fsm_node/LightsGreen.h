#ifndef LIGHTSGREEN_H
#define LIGHTSGREEN_H

#include "ros/ros.h"
#include <iostream>
#include <string>
#include <QState>


class LightsGreen : public QState
{
	Q_OBJECT
public:
			
	LightsGreen(QState *parent);
	
	~LightsGreen();
	
protected:

	 virtual void onEntry(QEvent *event);

signals:

	void lightsOk();
	 
};


#endif // LIGHTSGREEN_H