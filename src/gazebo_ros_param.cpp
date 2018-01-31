#include "gazebo_ros_param.h"
#include <fstream>
#include <gazebo/physics/Model.hh>

namespace gazebo
{

void GazeboRosParamPlugin::Load(gazebo::physics::ModelPtr _parent,
                                sdf::ElementPtr _sdf)
{
    std::cout << "Loading GazeboRosParam..." << std::endl;

    if (!_sdf->HasElement("param")) {
        gzwarn << "No params to set. The plugin won't do anything."
               << std::endl;
    } else {
        sdf::ElementPtr el = _sdf->GetElement("param");
        // TODO: use model:// like syntax to retrieve files
        common::ModelDatabase *db = common::ModelDatabase::Instance();
        do {
            if (!el->HasAttribute("name")) {
                gzwarn << "Parameter entry without any name, skipping."
                       << std::endl;
            } else {
                std::string val = el->Get<std::string>();
                if (val.substr(0, 8) == "xacro://") {
                    std::cout << "Opening xacro file " << val.substr(8) << "..."
                              << std::endl;
                    system(
                        std::string(
                            "bash -c \"xacro -o /tmp/xacro.out --check-order "
                            + val.substr(8) + "\"")
                            .c_str());
                    std::ifstream xacro_fin("/tmp/xacro.out");
                    if (!xacro_fin.is_open()) {
                        gzerr << "Error opening xacro file saved in "
                                 "/tmp/xacro.out, skipping."
                              << std::endl;
                    }
                    val.assign(std::istreambuf_iterator<char>(xacro_fin),
                               std::istreambuf_iterator<char>());
                }
                std::string key = el->GetAttribute("name")->GetAsString();
                this->_params[key] = val;
                std::cout << "assigned " << key << std::endl;
            }
        } while ((el = el->GetNextElement()) != nullptr);
    }
}

void GazeboRosParamPlugin::Init()
{
    std::cout << "Initialising GazeboRosParam..." << std::endl;

    for (auto &key_val : _params) {
        std::cout << "Setting param " << key_val.first << std::endl;
        ros::param::set(key_val.first, key_val.second);
    }
}

// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(GazeboRosParamPlugin)
}
