#include <unistd.h>
#include <iostream>
#include "matrix_hal/gpio_control.h"
#include "matrix_hal/matrixio_bus.h"
#include "ros/ros.h"
#include "std_msgs/Bool.h"


const uint16_t GPIOOutputMode = 1;
const uint16_t GPIOInputMode = 0;

uint16_t pin_out;
matrix_hal::GPIOControl gpio;

using namespace std;

void switchCallback(const std_msgs::Bool::ConstPtr& msg)
{
  ROS_INFO("Se modifica el pin de salida");
  bool value = msg->data;
  gpio.SetGPIOValue(pin_out, value ? 1 : 0);
  cout << "pin puesto a: " << value << endl;
 
}

int main(int argc, char **argv) 
{
	ros::init(argc, argv, "listener");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("speaker/switch", 1000, switchCallback);

	pin_out = 0;
	
	matrix_hal::MatrixIOBus bus;
	
	if (!bus.Init()) return false;

	
	gpio.Setup(&bus);
	gpio.SetMode(pin_out, GPIOOutputMode);
	

	ros::spin();

	return 0;
}