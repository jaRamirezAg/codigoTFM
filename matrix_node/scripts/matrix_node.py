#!/usr/bin/env python
# coding=utf-8
# Software License Agreement (BSD License)
#
# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Revision $Id$

## Simple talker demo that published std_msgs/Strings messages
## to the 'chatter' topic

import rospy
from std_msgs.msg import String
import websocket
import requests
import json
import time
import threading 
import enum
from matrix_lite import led
from matrix_node.srv import MatrixLight, MatrixOneLight, MatrixLightResponse, MatrixOneLightResponse
from matrix_node.msg import MatrixWakeWord

everloop = ['black'] * led.length

pub = rospy.Publisher('matrix/wakeword', MatrixWakeWord, queue_size=10)

class WakewordCode(enum.Enum):
    unknown = 0
    hello_walzru = 1
    
    def __str__(self):
        return '%s' % self.value
        
    def __int__(self):
        return self.value
    
def getWakewordCode(wakeword):
    if wakeword == "olagualzru":
        return WakewordCode.hello_walzru
    else:
        return WakewordCode.unknown
        
def matrixLightSrv(req):
    
    for i in range(len(everloop)):
        r = req.r
        g = req.g
        b = req.b
        w = req.intensity
        everloop[i] = {'r':r, 'g':g, 'b':b, 'w': w}
        
    led.set(everloop)
    return MatrixLightResponse(True);
    
def matrixOneLightSrv(req):
    
    r = req.r
    g = req.g
    b = req.b
    w = req.intensity
    i = req.index
    
    if i < len(everloop):
        everloop[i] = {'r':r, 'g':g, 'b':b, 'w': w}

    led.set(everloop)
    return MatrixOneLightResponse(True);

def websocketWakeword():
    ws = websocket.WebSocketApp("ws://localhost:12101/api/events/wake", on_message = on_message, on_error = on_error, on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()

def on_message(ws, message):
    data = json.loads(message)
    msg = MatrixWakeWord()
    msg.ww_code = int(getWakewordCode(data["wakewordId"]))
    msg.ww_str = data["wakewordId"]
    pub.publish(msg)

def on_error(ws, error):
    print(error)
    
def on_close(ws):
    print("**Websocket disconnected**\n")

def on_open(ws):
    print("**Websocket connected**\n")

if __name__ == '__main__':

    rospy.init_node('matrix_node', anonymous=True)
    print("**ROS node matrix_node running...**\n")
    hilo_wakeword = threading.Thread(target=websocketWakeword) 
    
    try:
        hilo_wakeword.start()    
        light_service = rospy.Service('matrix/light', MatrixLight, matrixLightSrv)
        one_light_service = rospy.Service('matrix/one_light', MatrixOneLight, matrixOneLightSrv)
        rospy.spin()
    except rospy.ROSInterruptException:
        hilo_wakeword.kill()