#ifndef WAITFORWW_H
#define WAITFORWW_H

#include "ros/ros.h"
#include <iostream>
#include <string>
#include <QState>
#include <QTimer>

#define LEDS_NUMBER 18

class WaitForWw : public QState
{
	Q_OBJECT
	
public:
			
	WaitForWw(QState *parent);
	
	~WaitForWw();
	
protected:

	 virtual void onEntry(QEvent *event);
	 virtual void onExit(QEvent *event);
	 
 private:
 
	 void blink();
	 void loading();
	 
	 QTimer m_timer;
	 
	 // Blink
	 uint8_t m_intensity;
	 bool m_is_increment;

	// Loading
	uint8_t m_value;
	uint8_t m_index;
	 
};


#endif // WAITFORWW_H