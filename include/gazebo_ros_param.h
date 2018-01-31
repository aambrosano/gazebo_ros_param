#ifndef GAZEBO_ROS_PARAM_H
#define GAZEBO_ROS_PARAM_H

#include <gazebo/common/common.hh>
#include <map>
#include <string>
#include <ros/ros.h>
#include <ros/param.h>

namespace gazebo {

class GazeboRosParamPlugin : public ModelPlugin {

public:
    // ~GazeboRosParamPlugin();

    virtual void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) override;
    virtual void Init() override;

private:
    std::map<std::string, std::string> _params;
    ros::NodeHandle nh;
};

}

#endif
