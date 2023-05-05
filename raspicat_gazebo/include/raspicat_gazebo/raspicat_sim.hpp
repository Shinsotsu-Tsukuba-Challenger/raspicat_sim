// Copyright 2023 RT Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef RASPICAT_GAZEBO__RASPICAT_SIM_HPP_
#define RASPICAT_GAZEBO__RASPICAT_SIM_HPP_

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "lifecycle_msgs/msg/transition.hpp"

#include "rclcpp/rclcpp.hpp"
#include "rclcpp/publisher.hpp"

#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"

#include <std_srvs/srv/set_bool.hpp>
#include <nav_msgs/msg/odometry.hpp>

namespace raspicatsim
{
class RaspicatSim : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit RaspicatSim(const std::string & node_name, bool intra_process_comms = false);

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_configure(const rclcpp_lifecycle::State &);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State & state);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State & state);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(
    const rclcpp_lifecycle::State &);
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_shutdown(const rclcpp_lifecycle::State & state);

  void velocity_command(const geometry_msgs::msg::Twist::SharedPtr msg);
  void handle_motor_power(
    const std::shared_ptr<rmw_request_id_t> request_header,
    const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
    const std::shared_ptr<std_srvs::srv::SetBool::Response> response);

  void watchdog();
  void set_motor_power(bool value);
  void stop_motors();

  void release_pointers();

private:
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr sim_cmd_vel_pub_;
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr velocity_sub_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr power_service_;
  rclcpp::TimerBase::SharedPtr watchdog_timer_;

  bool motor_power_flag_;
};
}  // namespace raspicatsim

#endif  // RASPICAT_GAZEBO__RASPICAT_SIM_HPP_
