cmake_minimum_required(VERSION 2.8.3)
project(mantis2)
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  roscpp
  sensor_msgs
  geometry_msgs
  std_msgs
  cmake_modules
  tf
  message_filters
)

find_package(OpenCV REQUIRED)

find_package(Eigen3 REQUIRED)

find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(
	${OpenCV_INCLUDE_DIRS}
	include
	include/mantis2
	${catkin_INCLUDE_DIRS}
	${OpenGL_INCLUDE_DIRS}
 	${GLUT_INCLUDE_DIRS}
 	${GLEW_INCLUDE_DIRS}
	${Eigen_INCLUDE_DIRS}
)


catkin_package(
   INCLUDE_DIRS include
#  LIBRARIES pauvsi_vio
# CATKIN_DEPENDS cv_bridge image_transport roscpp sensor_msgs std_msgs tf
#  DEPENDS system_lib
   DEPENDS opencv
)


add_library(rpoly2 include/mantis2/RobustPlanarPose/Rpoly.cpp)
target_link_libraries(rpoly2 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

add_library(planar_pose include/mantis2/RobustPlanarPose/RPP.cpp)
target_link_libraries(planar_pose ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} rpoly2)

add_library(mantis2_params include/mantis2/Mantis2Parameters.h)
set_target_properties(mantis2_params PROPERTIES LINKER_LANGUAGE CXX)

add_library(mantis2_types include/mantis2/Mantis2Types.h)
set_target_properties(mantis2_types PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(mantis2_types ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Eigen_LIBRARIES})

add_library(mantis2_poseEstimation include/mantis2/CoPlanarPoseEstimator.cpp)
target_link_libraries(mantis2_poseEstimation ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} planar_pose)

add_library(mantis2_hypoGen include/mantis2/Mantis2HypothesisGeneration.h)
set_target_properties(mantis2_hypoGen PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(mantis2_hypoGen ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Eigen_LIBRARIES} mantis2_types mantis2_params mantis2_poseEstimation)

add_library(testPointGen include/mantis2/GridTestPointGeneration.h)
set_target_properties(testPointGen PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(testPointGen ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Eigen_LIBRARIES} mantis2_types)

add_library(yaw_markov include/mantis2/YawMarkov.cpp)
set_target_properties(yaw_markov PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(yaw_markov ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Eigen_LIBRARIES} mantis2_params mantis2_types)

add_library(xyMarkov include/mantis2/XYMarkovModel.cpp)
target_link_libraries(xyMarkov ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Eigen_LIBRARIES} mantis2_params mantis2_types)

add_library(hypErr include/mantis2/HypothesisError.h)
set_target_properties(hypErr PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(hypErr ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Eigen_LIBRARIES} mantis2_params mantis2_types)

add_executable(mantis2_node src/mantis2_node.cpp)
target_link_libraries(mantis2_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Eigen_LIBRARIES} mantis2_types mantis2_params testPointGen xyMarkov mantis2_hypoGen)