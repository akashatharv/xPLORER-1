
/**============================================================================
 * @file       : navigator.cpp
 * @author     : Akash Atharv, Rishabh Choudhary
 * @version    : 1.0
 * @Copyright  : 3-Clause BSD
Copyright (c) 2018, Akash Atharv, Rishabh Choudhary
 
Redistribution and use in source and binary forms, with or without  
modification, are permitted provided that the following conditions are 
met:
 
1. Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.
 
2. Redistributions in binary form must reproduce the above copyright 
notice, this list of conditions and the following disclaimer in the   
documentation and/or other materials provided with the distribution.
 
3. Neither the name of the copyright holder nor the names of its 
contributors may be used to endorse or promote products derived from this 
software without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS 
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
 * @brief      : Functional implementation for Robot Navigation functionality
 *============================================================================
 */

#include <iostream>
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include "../include/xplorer/obstacleDetector.hpp"
#include "../include/xplorer/navigator.hpp"

navigator::navigator() {
      ROS_INFO("Initializing navigator");
// Publisher initialized for publishing to /mobile_base/commands/velocity topic
      pub2 = n2.advertise<geometry_msgs::Twist>
                     ("/mobile_base/commands/velocity", 1000);
// Initially zero velocities are assigned to the turtlebot
      msg.linear.x = 0.0;
      msg.linear.y = 0.0;
      msg.linear.z = 0.0;
      msg.angular.x = 0.0;
      msg.angular.y = 0.0;
      msg.angular.z = 0.0;
// Zero Velocity message is sent to turtle bot
      pub2.publish(msg);
}

navigator::~navigator() {
// Finally, before exiting, zero velocities are assigned to the turtlebot
      msg.linear.x = 0.0;
      msg.linear.y = 0.0;
      msg.linear.z = 0.0;
      msg.angular.x = 0.0;
      msg.angular.y = 0.0;
      msg.angular.z = 0.0;
// Zero Velocity message is sent to turtle bot
      pub2.publish(msg);
}

void navigator::explore(int flag) {
     // Publishing rate is set up at 10Hz
      ros::Rate loop_rate(10);
     // Condition to keep running, until ROS functions properly
      while (ros::ok()) {
     // Obstacles are checked
           if (obsDet.collisionDetect()) {
     // Collision is expected, Robot is turned
                ROS_WARN("Collision expected, turning to avoid obstacle");
                msg.linear.x = 0.0;
                msg.angular.z = 0.5;
           } else {
                // No obstacle detected, moving straight
                ROS_INFO("No collision is expected, Moving straight");
                msg.linear.x = 0.5;
                msg.angular.z = 0.0;
           }
     // The  updated velocity message is published
      pub2.publish(msg);
      ros::spinOnce();
      loop_rate.sleep();
      if (flag == 0) {
            break;
      }
      }
}
