// ROS node for LeddarTech Evaluation kit
// Nick Marquis, November 2014

#include "../include/leddar/LeddarC.h"
#include "../include/leddar/LeddarProperties.h"
#include <ros/ros.h>
#include <iostream>
#include "std_msgs/Float32.h"
#include "std_msgs/Int32.h"

#define ARRAY_LEN( a )  (sizeof(a)/sizeof(a[0]))
static LeddarHandle gHandle=NULL;

void scan()

int main(int argc, char **argv)
  { 
	ros::init(argc, argv, "leddar_p");
  ros::NodeHandle n;
  
  std_msgs::Int32 segment;
  std_msgs::Float32 distance;


  std::cout << "*************************************************"  << std::endl;
  std::cout << "             * Welcome to the Leddar! *"  << std::endl;
  std::cout << "*************************************************"  << std::endl;

	gHandle = LeddarCreate();
  char lAddress[24];
  lAddress[0] = 0;
  int count = 1;

	ros::Publisher mSegment_p = n.advertise<std_msgs::Int32>("segment", 1000); 
	ros::Publisher mDistance_p = n.advertise<std_msgs::Float32>("distance", 1000);
 	ros::Rate loop_rate(2);

	if ( LeddarConnect( gHandle, lAddress ) == LD_SUCCESS || 1) 
    {
    if (LeddarStartDataTransfer( gHandle, LDDL_DETECTIONS ) == LD_SUCCESS || 1)
      {

      while (ros::ok() && LeddarGetConnected( gHandle ) || 1) 
				{
        LdDetection lDetections[50];
        unsigned int lCount = LeddarGetDetectionCount( gHandle );
        
				if ( lCount > ARRAY_LEN( lDetections ) )
          {
          lCount = ARRAY_LEN( lDetections );
          }
          LeddarGetDetections( gHandle, lDetections, ARRAY_LEN( lDetections ) );
          
          int i, j;
          for( i=0, j=0; (i<lCount) && (j<12); ++i )
          {
              segment.data = lDetections[i].mSegment;
              distance.data = lDetections[i].mDistance;
              
              printf( "%d \n", segment.data );
              printf( "%5.2f \n", distance.data );
              
              mSegment_p.publish(segment); 
				      mDistance_p.publish(distance); 
              ++j;
          }
        
        count++; 
				
				ros::spinOnce();
				loop_rate.sleep();
				}
      }
    }
  LeddarDestroy( gHandle );
  return 0;
  }
