g++ -o main -w -I. -IBRDFs -Ibuild -ICameras -IGeometricObjects \
-ILights -IMaterials -ISamplers  -ITracers -IUtilities -IWorld -ITexture \
 *.cpp BRDFs/*.cpp  Cameras/*.cpp GeometricObjects/*.cpp Lights/*.cpp \
  Materials/*.cpp Samplers/*.cpp Tracers/*.cpp Utilities/*.cpp World/*.cpp \
  Texture/*.cpp

