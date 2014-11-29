cmake_minimum_required(VERSION 2.8.3)
project(motion_adaption)

find_package(Boost REQUIRED)
find_package(catkin REQUIRED COMPONENTS cmake_modules
					roscpp
                                        tf
                                        tf_conversions
                                        geometry_msgs)

find_package(Eigen REQUIRED)

catkin_package(INCLUDE_DIRS include
               LIBRARIES ${PROJECT_NAME}
               CATKIN_DEPENDS roscpp
                              tf
                              tf_conversions
                              geometry_msgs
               DEPENDS Eigen
                       Boost)

include_directories(include
                    ${catkin_INCLUDE_DIRS}
                    ${Boost_INCLUDE_DIRS})

add_library(motion_adaption src/motion_adaption.cpp)
target_link_libraries(motion_adaption ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_executable(test_motion_adaption src/test.cpp)
target_link_libraries(test_motion_adaption motion_adaption)


install(TARGETS motion_adaption
        LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
        
install(TARGETS test_motion_adaption
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
