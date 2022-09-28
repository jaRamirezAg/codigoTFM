#include "LightsOff.h"
#include <matrix_node/MatrixLight.h>




using namespace std;

LightsOff::LightsOff(QState *parent) : QState(parent)
{
	
	
	
}


void LightsOff::onEntry(QEvent *event)
{
	
	cout << "LightsOff" << endl;
	
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<matrix_node::MatrixLight>("/matrix/light");
	
	matrix_node::MatrixLight srv;
	srv.request.r = 0;
	srv.request.g = 0;
	srv.request.b = 0;
	srv.request.intensity = 0;
	client.call(srv);
	emit lightsOk();
	
	
}


LightsOff::~LightsOff()
{
	
}