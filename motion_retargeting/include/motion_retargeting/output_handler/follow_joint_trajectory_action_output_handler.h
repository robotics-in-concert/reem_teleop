/**
 * @file /motion_retargeting_controller/include/follow_joint_trajectory_action_handler.h
 *
 * @brief File comment
 *
 * File comment
 *
 * @date Apr 13, 2013
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef FOLLOW_JOINT_TRAJECTORY_ACTION_OUTPUT_HANDLER_H_
#define FOLLOW_JOINT_TRAJECTORY_ACTION_OUTPUT_HANDLER_H_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/FollowJointTrajectoryGoal.h>
#include <trajectory_msgs/JointTrajectoryPoint.h>
#include "output_handler.h"

namespace motion_retargeting
{

/**
 * Output handler for publishing the goal joint states via a FollowJointTrajectoryAction
 */
class FollowJointTrajectoryActionHandler : public OutputHandler
{
public:
  FollowJointTrajectoryActionHandler(const std::string& action_server_name = std::string("action_server_name")) :
                                     OutputHandler(),
                                     action_server_name_(action_server_name)
  {};

  ~FollowJointTrajectoryActionHandler(){};

  virtual bool init()
  {
    if (!initialised_)
    {
      ROS_INFO_STREAM("FollowJointTrajectoryAction Output Handler: Connecting to action server '"
                       << action_server_name_ << "'.");
      action_client_.reset(new actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction>(
                           action_server_name_, true));
      action_client_->waitForServer();
      initialised_ = true;
      ROS_INFO_STREAM("FollowJointTrajectoryAction Output Handler: Connected to action server.");
    }
    else
    {
      ROS_INFO_STREAM("FollowJointTrajectoryAction Output Handler: Already initialised.");
    }
    return true;
  };

  virtual bool setOutput(const sensor_msgs::JointState& output_joint_states)
  {
    if (initialised_)
    {
      action_goal_.trajectory.points.clear();
      action_goal_.trajectory.joint_names.clear();
      trajectory_point_.positions.clear();
      trajectory_point_.velocities.clear();
      trajectory_point_.accelerations.clear();
      for ( unsigned int joint = 0; joint < output_joint_states.name.size(); ++joint )
      {
        if((output_joint_states.name[joint] != "wheel_left") && (output_joint_states.name[joint] != "wheel_right"))
        {
          action_goal_.trajectory.joint_names.push_back(output_joint_states.name[joint]);
          trajectory_point_.positions.push_back(output_joint_states.position[joint]);
          trajectory_point_.velocities.push_back(output_joint_states.velocity[joint]);
          trajectory_point_.accelerations.push_back(0.0);
        }
      }
      action_goal_.trajectory.points.push_back(trajectory_point_);
      action_goal_.trajectory.header.stamp = output_joint_states.header.stamp;
      action_client_->sendGoal(action_goal_);
      ROS_DEBUG_STREAM("FollowJointTrajectoryAction Output Handler: Action goal sent.");
      return true;
    }
    else
    {
      ROS_WARN_STREAM("FollowJointTrajectoryAction Output Handler: Not initialised.");
      return false;
    }
  };

private:
  boost::shared_ptr<actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> > action_client_;
  std::string action_server_name_;
  control_msgs::FollowJointTrajectoryGoal action_goal_;
  trajectory_msgs::JointTrajectoryPoint trajectory_point_;
};

typedef boost::shared_ptr<FollowJointTrajectoryActionHandler> FollowJointTrajectoryActionHandlerPtr;

} // namespace motion_retargeting

#endif /* FOLLOW_JOINT_TRAJECTORY_ACTION_OUTPUT_HANDLER_H_ */
