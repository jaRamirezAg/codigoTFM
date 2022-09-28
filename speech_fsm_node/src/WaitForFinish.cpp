#include "WaitForFinish.h"

using namespace std;

WaitForFinish::WaitForFinish(QState *parent) : QState(parent)
{
	
	m_timer_none_intent.setSingleShot(true);	
	m_timer_none_intent.setInterval(7000);
	connect(&m_timer_none_intent, &QTimer::timeout, this, &WaitForFinish::noneIntent);

	
}


void WaitForFinish::onEntry(QEvent *event)
{
	m_timer_none_intent.start();
	cout << "WaitForFinish" << endl;
		
}

void WaitForFinish::onExit(QEvent *event)
{
	m_timer_none_intent.setInterval(7000);	
	m_timer_none_intent.stop();

}	


void WaitForFinish::stopTimer()
{
	m_timer_none_intent.stop();
	cout << "el timer se ha parado" << endl;

}

WaitForFinish::~WaitForFinish()
{
	
}

void WaitForFinish::setExpirationTime(std::uint32_t delay)
{
	if(delay <= 0)
		return; 
	
	cout << "tiempo de set: " << delay << endl;
	m_timer_none_intent.setInterval(delay);	
	m_timer_none_intent.start();	

}