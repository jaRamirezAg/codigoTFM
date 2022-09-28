#include "LightsRed.h"
#include <matrix_node/MatrixLight.h>


using namespace std;

LightsRed::LightsRed(QState *parent) : QState(parent)
{

}


void LightsRed::onEntry(QEvent *event)
{
	
	cout << "LightsRed" << endl;
	
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<matrix_node::MatrixLight>("/matrix/light");
	
	matrix_node::MatrixLight srv;
	srv.request.r = 255;
	srv.request.g = 0;
	srv.request.b = 0;
	srv.request.intensity = 0;
	client.call(srv);
	emit lightsOk();

	
	
}


LightsRed::~LightsRed()
{
	
}