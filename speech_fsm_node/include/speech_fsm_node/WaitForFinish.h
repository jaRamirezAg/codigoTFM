#ifndef WAITFORFINISH_H
#define WAITFORFINISH_H

#include <iostream>
#include <string>
#include <QState>
#include <QTimer>

class WaitForFinish : public QState
{
	Q_OBJECT
public:
			
	WaitForFinish(QState *parent);
	
	~WaitForFinish();
	
	void stopTimer();
	void setExpirationTime(std::uint32_t delay);

	
protected:

	 virtual void onEntry(QEvent *event);
	 void onExit(QEvent *event);	
	 QTimer m_timer_none_intent;

signals:

	void noneIntent();

};


#endif // WAITFORFINISH_H