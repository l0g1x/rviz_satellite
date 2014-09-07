/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AERIAL_MAP_DISPLAY_H
#define AERIAL_MAP_DISPLAY_H

#include <ros/ros.h>
#include <ros/time.h>
#include <rviz/display.h>

#include <OGRE/OgreTexture.h>
#include <OGRE/OgreMaterial.h>
#include <OGRE/OgreVector3.h>

#include <sensor_msgs/NavSatFix.h>

#include <QObject>
#include <QtConcurrentRun>
#include <QFuture>
#include <QHttp>
#include <QUrl>
#include <QByteArray>
#include <QFile>

namespace Ogre {
class ManualObject;
}

namespace rviz {

class FloatProperty;
class IntProperty;
class Property;
class QuaternionProperty;
class RosTopicProperty;
class VectorProperty;

/**
 * \class AerialMapDisplay
 * \brief Displays a map along the XY plane.
 */
class AerialMapDisplay: public Display
{
Q_OBJECT
public:
  AerialMapDisplay();
  virtual ~AerialMapDisplay();

  // Overrides from Display
  virtual void onInitialize();
  virtual void fixedFrameChanged();
  virtual void reset();
  virtual void update( float wall_dt, float ros_dt );

  float getResolution() { return resolution_; }
  int getWidth() { return width_; }
  int getHeight() { return height_; }
  Ogre::Vector3 getPosition() { return position_; }
  Ogre::Quaternion getOrientation() { return orientation_; }

protected Q_SLOTS:
  void updateAlpha();
  void updateTopic();
  void updateDrawUnder();
  void requestFinished(int id, bool error);

protected:
  // overrides from Display
  virtual void onEnable();
  virtual void onDisable();

  virtual void subscribe();
  virtual void unsubscribe();

  void navFixCallback(const sensor_msgs::NavSatFixConstPtr& msg);

  void loadImagery();
  
  void clear();

  void transformAerialMap();

  static void latLonToTileCoords(double lat, double lon, unsigned int zoom, 
                                 double&x, double& y);
  
  //  scene graph objects
  Ogre::ManualObject* manual_object_;
  Ogre::TexturePtr texture_;
  Ogre::MaterialPtr material_;
  bool loaded_;

  std::string topic_;
  float resolution_;
  int width_;
  int height_;
  Ogre::Vector3 position_;
  Ogre::Quaternion orientation_;
  std::string frame_;

  ros::Subscriber coord_sub_;

  RosTopicProperty* topic_property_;
  FloatProperty* resolution_property_;
  IntProperty* width_property_;
  IntProperty* height_property_;
  VectorProperty* position_property_;
  QuaternionProperty* orientation_property_;
  FloatProperty* alpha_property_;
  Property* draw_under_property_;

  boost::mutex mutex_;
  bool new_coords_;
  double ref_lat_;
  double ref_lon_;
  
  //  networking objects
  QHttp * http_;
  int request_id_;
};

} // namespace rviz

 #endif
