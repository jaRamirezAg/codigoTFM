#include "WaitForWw.h"
#include <matrix_node/MatrixOneLight.h>
#include <matrix_node/MatrixLight.h>

#include<iostream>
#include<ctime>

using namespace std;

WaitForWw::WaitForWw(QState *parent) : QState(parent)
{
    m_timer.setSingleShot(false);	
	m_timer.setInterval(55); 
	
	connect(&m_timer, &QTimer::timeout, this, &WaitForWw::blink);
	
}


void WaitForWw::onEntry(QEvent *event)
{
	cout << "WaitForWw" << endl;
	
	m_intensity = 0;
	m_is_increment = true;
	
	m_value = 255;
	m_index = 0;
			
	m_timer.start();

}

void WaitForWw::onExit(QEvent *event)
{
	m_timer.stop();
	cout << "onExit" << endl;
}

void WaitForWw::blink()
{
	if(m_is_increment)
	{
		m_intensity=m_intensity+15;
		if(m_intensity >= 255)
		{
			m_intensity = 255;
			m_is_increment = false;
		}
	}
	else
	{
		m_intensity=m_intensity-15;
		if(m_intensity <= 0)
		{
			m_intensity = 0;
			m_is_increment = true;
		}
	}
	
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<matrix_node::MatrixLight>("/matrix/light");
	matrix_node::MatrixLight srv;
	srv.request.r = 255 - m_intensity;
	srv.request.g = 0;
	srv.request.b = 0;
	srv.request.intensity = 0;
	client.call(srv);

}

void WaitForWw::loading()
{
	if(m_index >= LEDS_NUMBER)
	{
		m_index = 0;
		if(m_value == 255)
		{
			m_value = 0;
		}
		else
		{
			m_value = 255;
		}
	}
	
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<matrix_node::MatrixOneLight>("/matrix/one_light");
	matrix_node::MatrixOneLight srv;
	srv.request.r = m_value;
	srv.request.g = 0;
	srv.request.b = 0;
	srv.request.intensity = m_intensity;
	srv.request.index = m_index;
	client.call(srv);
	m_index++;
}

WaitForWw::~WaitForWw()
{
	
}