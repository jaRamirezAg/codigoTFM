#include "PlayNoise.h"
#include <ros/ros.h>
#include <speak_node/Play.h>
#include <speak_node/Volume.h>
#include <speak_node/AudioCodes.h>

using namespace std;

const float NOISE_VOLUME = 0.2;

PlayNoise::PlayNoise(QState *parent) : QState(parent)
{
	
}


void PlayNoise::onEntry(QEvent *event)
{
	cout << "PlayNoise" << endl;
	
	emit stopBlink();
	
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<speak_node::Play>("/play_service");
	ros::ServiceClient client_volume = n.serviceClient<speak_node::Volume>("/volume_service");
	
	speak_node::Volume srv_volume;
	srv_volume.request.level = NOISE_VOLUME;
	client_volume.call(srv_volume);
	
	speak_node::Play srv;
	srv.request.cod_audio = static_cast<uint8_t>(AudioCode::noise);
	srv.request.infinite = true;
	srv.request.sync = false;
	client.call(srv);
	emit playingNoise();
	
}


PlayNoise::~PlayNoise()
{
	
}