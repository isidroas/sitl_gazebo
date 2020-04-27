#include "gazebo_visual_plugin.h"

#define y_offset 0.5e3
#define x_offset 0.6e3
#define z_offset 0.25e3
namespace gazebo
{
  namespace rendering
  {
     void SomeVisualPlugin::cb(const boost::shared_ptr<const gazebo::msgs::Quaternion> &_msg)
     {
         //std::cout << _msg->DebugString();
         this->VisualizeForceOnLink(_msg->x(),_msg->y(),_msg->z(),_msg->w());
         //this->VisualizeForceOnLink(0,0,0,0);
         //std::cout << _msg->x();
     }

    ////////////////////////////////////////////////////////////////////////////////
    // Constructor
    SomeVisualPlugin::SomeVisualPlugin(): 
      line(NULL)
    {

    }

    ////////////////////////////////////////////////////////////////////////////////
    // Destructor
    SomeVisualPlugin::~SomeVisualPlugin()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////
    // Load the plugin
    void SomeVisualPlugin::Load( VisualPtr _parent, sdf::ElementPtr _sdf )
    {
      gzdbg << " El plugin visual se va a cargar 2\n ";
      this->visual_ = _parent;

      this->visual_namespace_ = "visual/";

      // start ros node


      // Listen to the update event. This event is broadcast every
      // simulation iteration.
      this->update_connection_ = event::Events::ConnectRender(
          boost::bind(&SomeVisualPlugin::OnUpdate, this));

      this->line = this->visual_->CreateDynamicLine(RENDERING_LINE_STRIP);
      this->line_2 = this->visual_->CreateDynamicLine(RENDERING_LINE_STRIP);

      common::Color _color=common::Color::Green;
      this->line->AddPoint(0+x_offset, y_offset, z_offset, _color);
      this->line->AddPoint(0+x_offset, y_offset, 1, _color);
      this->line_2->AddPoint(0+x_offset, -y_offset, z_offset, _color);
      this->line_2->AddPoint(0+x_offset, -y_offset, 1*1e3, _color);
      this->line->setMaterial("Gazebo/Purple");
      this->line->setVisibilityFlags(GZ_VISIBILITY_GUI);
      this->line_2->setMaterial("Gazebo/Purple");
      this->line_2->setVisibilityFlags(GZ_VISIBILITY_GUI);
      this->visual_->SetVisible(true);

      this->VisualizeForceOnLink(1,1,1,1);

      node = gazebo::transport::NodePtr(new transport::Node());
      node->Init();
      sub = node->Subscribe<gazebo::msgs::Quaternion>("~/visualMenssage", &SomeVisualPlugin::cb,this);
      gzdbg << " El plugin visual se ha cargado\n ";

    }

    void SomeVisualPlugin::OnUpdate()
    {
    }

    void SomeVisualPlugin::VisualizeForceOnLink(float x1, float z1, float x2, float z2)
    {

         ignition::math::Vector3d _value(x1*1e3+x_offset,y_offset,z1*1e3+z_offset);
         this->line->SetPoint(1, _value);

         ignition::math::Vector3d _value_2(x2*1e3+x_offset,-y_offset,z2*1e3+z_offset);
         this->line_2->SetPoint(1, _value_2);
      // Add two points to a connecting line strip from link_pose to endpoint
      //this->line->setMaterial("Gazebo/Purple");
      //this->line->setVisibilityFlags(GZ_VISIBILITY_GUI);
      //this->visual_->SetVisible(true);
    }

    // Register this plugin within the simulator
    GZ_REGISTER_VISUAL_PLUGIN(SomeVisualPlugin)
  }
}
