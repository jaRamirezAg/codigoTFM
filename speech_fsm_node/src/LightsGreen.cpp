#include "LightsGreen.h"
#include <matrix_node/MatrixLight.h>


using namespace std;

LightsGreen::LightsGreen(QState *parent) : QState(parent)
{
	
	
	
}


void LightsGreen::onEntry(QEvent *event)
{
	
	cout << "LightsGreen" << endl;
	
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<matrix_node::MatrixLight>("/matrix/light");
	
	matrix_node::MatrixLight srv;
	srv.request.r = 0;
	srv.request.g = 255;
	srv.request.b = 0;
	srv.request.intensity = 0;
	client.call(srv);
	emit lightsOk();
	
	
}


LightsGreen::~LightsGreen()
{
	
}