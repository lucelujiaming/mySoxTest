g++ -o main -w -I. -IGeometricObjects \
-ISamplers  -ITracers -IUtilities -IWorld \
 *.cpp GeometricObjects/*.cpp \
 Samplers/*.cpp Tracers/*.cpp Utilities/*.cpp World/*.cpp

