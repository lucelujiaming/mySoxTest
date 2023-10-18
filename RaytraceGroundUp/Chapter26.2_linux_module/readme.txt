g++ -o main -w -I. -IBRDFs -IGeometricObjects -ILights \
-ISamplers  -ITracers -IUtilities -IWorld -ICameras -IMaterials \
 *.cpp BRDFs/*.cpp GeometricObjects/*.cpp Cameras/*.cpp \
 Samplers/*.cpp Tracers/*.cpp Utilities/*.cpp World/*.cpp \
 Lights/*.cpp Materials/*.cpp 

