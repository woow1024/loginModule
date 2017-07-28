#project name

SET(CMAKE_BUILD_TYPE "debug")

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_DEFINITIONS(-DDEBUG_MODE -g)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -Wl,-rpath=../lib") 
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -Wl,-rpath=../lib") 

SET(CMAKE_SOURCE_DIR .)

PROJECT(QCenter)
#head file path
INCLUDE_DIRECTORIES( 
${CMAKE_SOURCE_DIR}/amqpcpp-master/include/
include/
public/
./src/http/include/
./src/redis/
./src/rmq/include/
./src/tinyxml/
./src/toolkit/
/usr/include/
/usr/local/include/
./rabbitmq-c-0.8.0/librabbitmq
./rabbitmq-c-0.8.0/librabbitmq/unix
#/usr/local/ssl/include
) 
 
LINK_DIRECTORIES(
${CMAKE_CURRENT_BINARY_DIR}/run/lib/
/usr/local/lib/
./amqpcpp-master/
#/usr/local/ssl/lib/
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


AUX_SOURCE_DIRECTORY(./src/PushCenter QFRONT_SRCS)
ADD_EXECUTABLE(PushCenter ${QFRONT_SRCS}) 
TARGET_LINK_LIBRARIES(PushCenter toolkit tinyxml libjson.a pthread)
TARGET_LINK_LIBRARIES(PushCenter libssl.a  libamqpcpp.a librabbitmq.a  util dl rt)  
