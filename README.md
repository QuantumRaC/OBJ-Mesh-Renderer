# OBJ Mesh Renderer – CG Project 3

This project is a C++ OpenGL-based mesh renderer built for rendering `.obj` models with perspective projection, camera control, and basic texture mapping. Initially made as a course project for CAP4740 Computational Structures in CG (Prof Jorg Peters).

## 🎯 Features

- Loads `.obj` meshes using TinyObjLoader
- Real-time rendering with OpenGL
- Perspective projection (45° FOV, near 0.1, far 100)
- Camera movement with arrow keys
- Wireframe toggle (`F` key)
- Texture mapping with flipped UVs
- Model reset with `R` key

## 📁 Structure

cg_project_3/
├── source/ # Source code (C++, shaders)
├── shaders/ # GLSL shader files
├── models/ # OBJ mesh files
├── textures/ # Image textures
├── CMakeLists.txt # Build configuration
└── Readme.txt # Original project notes

## 🖼️ Model Attribution

The head model used in this project (`texturedhead.obj`) was downloaded from

**TurboSquid – Head Base Mesh 3D Model**  
https://www.turbosquid.com/3d-models/head-base-mesh-3d-model/951201

UV mapping and texture application were done using **Blender**, and a photo of the author's face was used for texturing.

## 🛠️ Requirements

- OpenGL 3.3+
- GLFW
- GLAD
- GLM
- stb_image
- CMake

## 🎮 Controls
← → – Rotate camera horizontally
↑ ↓ – Rotate camera vertically
F – Toggle wireframe mode
R – Reset camera and render states
ESC – Exit program

📜 License
This project is for educational use. The head model is subject to the terms of its original source (TurboSquid).
