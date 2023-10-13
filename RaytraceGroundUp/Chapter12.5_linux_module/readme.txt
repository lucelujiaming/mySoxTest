g++ -o main -w -I. -IGeometricObjects -ILights \
-ISamplers  -ITracers -IUtilities -IWorld -ICameras \
 *.cpp GeometricObjects/*.cpp Cameras/*.cpp \
 Samplers/*.cpp Tracers/*.cpp Utilities/*.cpp World/*.cpp \
 Lights/*.cpp 

