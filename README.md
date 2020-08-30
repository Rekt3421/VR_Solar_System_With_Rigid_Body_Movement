# VR_Solar_System_With_Rigid_Body_Movement
OpenVR Solar system project which makes use of the openGL programmable pipeline to apply textures, instancing and animation to meshes in a Solar system (Think meteors, planets, stars etc) 

This project allows users to control the complexity of the scene by specifiying number of objects to be loaded. Comes with real time performance monitoring intergrated into the UI using ImGui. 

Most of the work on the client side is related to specifying buffers and shaders while the four user defined shaders are controling movements etc are written in GLSL.

Project has an executable in the Debug folder in x64/Debug called VRTemplated.exe, requires the use of a head mounted display (HTC Vive) or someother hmd capable of using openVR.
