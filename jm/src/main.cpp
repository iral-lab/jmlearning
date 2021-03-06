 /*
  * Software License Agreement (BSD License)
  *
  *  Copyright (c) 2009, Willow Garage, Inc.
  *  All rights reserved.
  *
  *  Redistribution and use in source and binary forms, with or without
  *  modification, are permitted provided that the following conditions
  *  are met:
  *
  *   * Redistributions of source code must retain the above copyright
  *     notice, this list of conditions and the following disclaimer.
  *   * Redistributions in binary form must reproduce the above
  *     copyright notice, this list of conditions and the following
  *     disclaimer in the documentation and/or other materials provided
  *     with the distribution.
  *   * Neither the name of Willow Garage, Inc. nor the names of its
  *     contributors may be used to endorse or promote products derived
  *     from this software without specific prior written permission.
  *
  *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  *  POSSIBILITY OF SUCH DAMAGE.
  *
  * $Id: pointcloud_to_pcd.cpp 33238 2010-03-11 00:46:58Z rusu $
  *
  */
 
 // ROS core
 #include <ros/ros.h>
 // PCL includes
 #include <pcl/io/io.h>
 #include <pcl/io/pcd_io.h>
 #include <pcl/point_types.h>
 #include <sensor_msgs/PointCloud2.h>
 #include <pcl_conversions/pcl_conversions.h>
 #include <pcl/point_cloud.h>

 #include <image_transport/image_transport.h>
 #include <cv_bridge/cv_bridge.h>
 #include <sensor_msgs/image_encodings.h>


 using namespace std;
 
 class PointCloudToPCD
 {
     protected:
     ros::NodeHandle nh_;
 
     public:
     string cloud_topic_;
 
     ros::Subscriber sub_;
 
     // Callback
     void
       cloud_cb (const sensor_msgs::PointCloud2ConstPtr& cloud)
     {
       if ((cloud->width * cloud->height) == 0)
         return;
 
       ROS_INFO ("Received %d data points in frame %s with the following fields: %s", (int)cloud->width * cloud->height, cloud->header.frame_id.c_str (), pcl::getFieldsList (*cloud).c_str ());
 
       std::stringstream ss;
       // Check if there is a parameter on the server that sets the prefix for the output file
       std::string prefix;
       nh_.getParam ("prefix", prefix);
 
       ss << prefix << cloud->header.stamp << ".pcd";
       ROS_INFO ("Data saved to %s", ss.str ().c_str ());
 
       
//       pcl::io::savePCDFileBinary (ss.str (), *cloud, Eigen::Vector4f::Zero (),
 //                   Eigen::Quaternionf::Identity ());
       pcl::io::savePCDFile (ss.str (), *cloud, Eigen::Vector4f::Zero (),
                             Eigen::Quaternionf::Identity (), false);
     }
 
     PointCloudToPCD ()
     {
       cloud_topic_ = "/kinect2/qhd/points";
 
       sub_ = nh_.subscribe (cloud_topic_, 1,  &PointCloudToPCD::cloud_cb, this);
       ROS_INFO ("Listening for incoming data on topic %s", nh_.resolveName (cloud_topic_).c_str ());
     }
 };
 
 /* ---[ */
 int
   main (int argc, char** argv)
 {
   ros::init (argc, argv, "pointcloud_to_pcd", ros::init_options::AnonymousName);
 
   PointCloudToPCD b;
   ros::spin ();

   return (0);
  }
 /* ]--- */
