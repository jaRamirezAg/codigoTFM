#include "ros/ros.h"
#include <QtCore>
#include <SpeechFsm.h>
#include <csignal>

using namespace std;


/* void signalHandler( int signum ) 
{
	signal(signum, SIG_DFL);
    qApp->quit();
} */


int main(int argc, char **argv)
{
	
	QCoreApplication a(argc, argv);
	
	/* signal(SIGINT, signalHandler); 
	signal(SIGTERM, signalHandler); */ 

	ros::init(argc, argv, "Fsm_node");
	
	ros::service::waitForService("/matrix/light");
	ros::service::waitForService("/play_service");
	ros::service::waitForService("/volume_service");
	ros::service::waitForService("/speak_service");
	ros::service::waitForService("/stop_service");
	ros::service::waitForService("/matrix/light");
	
  
	SpeechFsm speech_fsm;
								
	return a.exec();
}  		