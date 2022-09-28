#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/UInt32.h"
#include "std_msgs/Empty.h"

#include <nlohmann/json.hpp>

#include "mqtt_node/EndConversation.h"


#include <sstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include "mqtt/async_client.h"
#include "mqtt/connect_options.h"

const std::string SERVER_ADDRESS("tcp://seeiservices.es");
const std::string CLIENT_ID("victoria");
const std::string TOPIC_RETURN_TO_NOISE("victoria/wait_for_finish");
const std::string TOPIC_READY_TO_SPEAK("victoria/ready_to_speak");
const std::string TOPIC_RECEIVED_OK("victoria/received_ok");
const std::string TOPIC_STOP_TIMER("victoria/stop_wait_for_finish");



const std::string user = "victoria";
const std::string password = "VictoriaB0t";

const int QOS = 1;
const int N_RETRY_ATTEMPTS = 5;
bool received = false;
bool received_ready_to_speak = false;
bool received_msg_ok = false;
bool received_stop_timer = false;
std_msgs::String payload;	
	
bool end_conversation;

uint32_t delay;

using json = nlohmann::json;

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

class action_listener : public virtual mqtt::iaction_listener
{
	std::string name_;

	void on_failure(const mqtt::token& tok) override {
		std::cout << name_ << " failure";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		std::cout << std::endl;
	}

	void on_success(const mqtt::token& tok) override {
		std::cout << name_ << " success";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
		std::cout << std::endl;
	}

public:
	action_listener(const std::string& name) : name_(name) {}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
class callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
	
	// An action listener to display the result of actions.
	action_listener subListener_;

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		try {
			cli_.connect(connOpts_, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			std::cerr << "Error: " << exc.what() << std::endl;
			exit(1);
		}
	}

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {
		std::cout << "Connection attempt failed" << std::endl;
		if (++nretry_ > N_RETRY_ATTEMPTS)
			exit(1);
		reconnect();
	}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override {}

	// (Re)connection success
	void connected(const std::string& cause) override {
		std::cout << "\nConnection success" << std::endl;
		std::cout << "\nSubscribing to topic '" << TOPIC_RETURN_TO_NOISE << "'\n";
		std::cout << "\nSubscribing to topic '" << TOPIC_READY_TO_SPEAK << "'\n";
		std::cout << "\nSubscribing to topic '" << TOPIC_RECEIVED_OK << "'\n";
		std::cout << "\nSubscribing to topic '" << TOPIC_STOP_TIMER << "'\n"


			<< "\tfor client " << CLIENT_ID
			<< " using QoS" << QOS << "\n"
			<< "\nPress Q<Enter> to quit\n" << std::endl;

		cli_.subscribe(TOPIC_RETURN_TO_NOISE, QOS, nullptr, subListener_);
		cli_.subscribe(TOPIC_READY_TO_SPEAK, QOS, nullptr, subListener_);
		cli_.subscribe(TOPIC_RECEIVED_OK, QOS, nullptr, subListener_);
		cli_.subscribe(TOPIC_STOP_TIMER, QOS, nullptr, subListener_);

	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;

		std::cout << "Reconnecting..." << std::endl;
		nretry_ = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		std::cout << "Message arrived" << std::endl;
		std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
		std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
			
		if((msg->get_topic()) == TOPIC_RETURN_TO_NOISE)
		{	
			try
			{
				auto parser = json::parse(msg->to_string());
				auto it = parser.find("delay");
				if(it != parser.end())
				{
					delay = parser.at("delay").get<uint32_t>();
				}
			}
			catch(...)
			{
				std::cout << "Revienta" << std::endl;
				delay = 0;
			}
			std::cout << "Delay " << delay << std::endl;
			
			received = true;	
		} 
		else if ((msg->get_topic()) == TOPIC_READY_TO_SPEAK)
		{
			end_conversation = true;
			
			try
			{
				auto parser = json::parse(msg->to_string());
				auto it = parser.find("end_conversation");
				if(it != parser.end())
				{
					end_conversation = parser.at("end_conversation").get<bool>();
				}
			}
			catch(...)
			{
				std::cout << "Revienta" << std::endl;
				end_conversation = true;
			}
			
			std::cout << "End Conv: " << end_conversation << std::endl;
			received_ready_to_speak = true;

		}
		else if ((msg->get_topic()) == TOPIC_RECEIVED_OK)
		{
			received_msg_ok = true;
		}
		else if ((msg->get_topic()) == TOPIC_STOP_TIMER)
		{
			received_stop_timer= true;
		}
				
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
				: nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "mqtt_node");
	ros::NodeHandle n;
	ros::Publisher chatter_pub = n.advertise<std_msgs::UInt32>("/mqtt", 1000); 
	ros::Publisher ready_to_speak = n.advertise<mqtt_node::EndConversation>("/mqtt/ready_to_speak", 1000); 
	ros::Publisher received_ok = n.advertise<std_msgs::String>("/mqtt/received_ok", 1000); 
	ros::Publisher stop_timer = n.advertise<std_msgs::Empty>("/mqtt/stop_wait_for_finish", 1000); 
	
	
	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);

	mqtt::connect_options connOpts;
	connOpts.set_clean_session(false);
	connOpts.set_user_name(user);
	connOpts.set_password(password);

	// Install the callback(s) before connecting.
	callback cb(cli, connOpts);
	cli.set_callback(cb);

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		std::cout << "Connecting to the MQTT server..." << std::flush;
		cli.connect(connOpts, nullptr, cb);
	}
	catch (const mqtt::exception& exc) {
		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< SERVER_ADDRESS << "'" << exc << std::endl;
		return 1;
	}

	// Just block till user tells us to quit.
	

	while (ros::ok())
	{		
		if(received == true)
		{
			std_msgs::UInt32 time;
			time.data = delay;
			std::cout<<"delay enviando en mqtt: " << delay << std::endl;
			chatter_pub.publish(time);
			received = false;		
		}
		else if (received_ready_to_speak == true)
		{
			received_ready_to_speak = false;	
			mqtt_node::EndConversation msg_end_conversation;
			msg_end_conversation.end_conversation = end_conversation;
			std::cout << "en el publish estoy enviando: " << end_conversation << std::endl;
			ready_to_speak.publish(msg_end_conversation);
		}
		else if (received_msg_ok == true)
		{
			received_ok.publish(payload);
			received_msg_ok = false;	
			std::cout<<"publico mensaje ros en topic received_ok"<<std::endl;
		}
		else if (received_stop_timer == true)
		{
			std_msgs::Empty empty;
			stop_timer.publish(empty);
			received_stop_timer = false;
		}

		ros::spinOnce();
		
	}
		
	// Disconnect

	try {
		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
		cli.disconnect()->wait();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << exc << std::endl;
		return 1;
	}

 	return 0;
}