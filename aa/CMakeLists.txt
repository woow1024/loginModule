#project name

SET(CMAKE_BUILD_TYPE "debug")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_DEFINITIONS(-DDEBUG_MODE -g)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -Wl,-rpath=../lib") 
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Wl,-rpath=../lib") 

PROJECT(QCenter)
#head file path
INCLUDE_DIRECTORIES( 
/opt/amqpcpp-master/include/
include/
public/
./src/http/include/
./src/redis/
./src/rmq/include/
./src/tinyxml/
./src/toolkit/
/usr/include/
/usr/local/include/
/opt/rabbitmq-c-0.8.0/librabbitmq
/opt/rabbitmq-c-0.8.0/librabbitmq/unix
/usr/local/ssl/include
) 
 
LINK_DIRECTORIES(
${CMAKE_CURRENT_BINARY_DIR}/run/lib/
/usr/local/lib/
/opt/amqpcpp-master/
/usr/local/ssl/lib/
/opt/rabbitmq-c-0.8.0/librabbitmq/
#/home/meirui.yan/rabbitmqcpp/src
)
 
SET(EXECUTABLE_OUTPUT_PATH run/bin)
SET(LIBRARY_OUTPUT_PATH run/lib)


AUX_SOURCE_DIRECTORY(./src/tinyxml TINYXML_LIB_SRCS) 
ADD_DEFINITIONS(-DTIXML_USE_STL)
ADD_LIBRARY(tinyxml ${TINYXML_LIB_SRCS})  



AUX_SOURCE_DIRECTORY(./src/redis REDIS_LIB_SRCS) 
ADD_DEFINITIONS(-DREDIS_USE_STL)
ADD_LIBRARY(rediswrapper ${REDIS_LIB_SRCS})  

 

AUX_SOURCE_DIRECTORY(./src/toolkit TOOLKIT_LIB_SRCS) 
ADD_DEFINITIONS(-DTOOLKIT_USE_STL)
ADD_LIBRARY(toolkit ${TOOLKIT_LIB_SRCS})  


AUX_SOURCE_DIRECTORY(./src/QFront/QFrontServer QFRONT_SRCS)
ADD_EXECUTABLE(QFrontServer ${QFRONT_SRCS}) 
TARGET_LINK_LIBRARIES(QFrontServer toolkit tinyxml libjson.a pthread)
TARGET_LINK_LIBRARIES(QFrontServer libssl.a  libamqpcpp.a librabbitmq.a  util dl rt)  
