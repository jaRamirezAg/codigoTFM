#include "ros/ros.h"
#include <QtCore>
#include "AudioManager.h"

using namespace std;


int main(int argc, char **argv)
{
	QCoreApplication a(argc, argv);
	ros::init(argc, argv, "Speak_node");
  
	AudioManager audio_manager;

	return a.exec();
}
