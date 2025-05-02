Setup:

Perspective Projection is set using glm::perspective() with a 45° field of view, near plane 0.1, far plane 100.

Camera Movement:

LEFT and RIGHT  rotate the camera horizontally along the equator.
UP and DOWN  rotate the camera vertically along the red orthogonal circle.
The camera always looks at the origin with (0, 1, 0) as the "up" vector.
R Key resets the camera angles and other states to the startup settings.

Task 1: Display Face Net
Mesh loading is implemented using TinyObjLoader to load a custom OBJ file (texturedhead.obj).
Model is downloaded from https://www.turbosquid.com/3d-models/head-base-mesh-3d-model/951201.

Wireframe:
The f key toggles wireframe mode on and off by switching glPolygonMode() between GL_LINE and GL_FILL.

Task 2: Add a Texture
Texture Mapping:
A photo of my face (facefront.jpg) is mapped onto the face mesh using Blender.
Texture coordinates (UVs) are flipped vertically to match OpenGL's expected UV origin (bottom-left).
Texture applied successfully and correctly appears on the model.

Bonus: Smooth Surface with Wireframe Control Net
Smooth Surface Rendering implemented using PN-Triangles tessellation (level 4).
Key P toggles between smooth PN-triangle tessellated mesh and original coarse mesh.
Wrote a generateSmoothPNMesh() function to generate smooth vertices, normals, UVs, and upload them to a separate VAO.

Bonus 2: partially implemented - P key toggles subdivided model
