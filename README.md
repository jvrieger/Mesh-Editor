A mesh editor application in the style of Autodesk Maya or Blender.
Building upon knowledge of C++ and OpenGL, this is a half-edge data structure created from interlinked pointers and visualized mesh using OpenGL vertex buffers. Additionally implemented is a Catmull-Clark subdivision algorithm to smooth the meshes, and some simpler mesh editing operations as well.

Technical Implementation
This project focused on the foundational data structures and algorithms required for high-quality surface modeling in 3D graphics applications.
Implemented the Half-Edge Data Structure (HEDS), a highly efficient and robust way to represent arbitrary manifold mesh topology (faces, edges, and vertices).
HEDS allows for constant-time complexity O(1) traversal between adjacent faces, edges, and vertices, which is crucial for mesh editing and subdivision operations.
This required defining and managing the pointers for next, twin, vertex, and face for each half-edge.
Implemented the full Catmull-Clark subdivision algorithm to generate smooth, high-resolution surfaces from an initial coarse polygonal mesh.
This involved calculating and inserting three new types of points for each subdivision step: Face Points, Edge Points, and Vertex Points.
Successfully applied the limit rules to calculate the new positions of the original (old) vertices, ensuring geometric continuity (C^2 continuity everywhere except at extraordinary vertices).
Handled the topological changes resulting from subdivision, including the splitting of faces and edges, and subsequently recalculated vertex normals to ensure the final subdivided surface appeared smooth and correctly shaded.

Process and Contribution
The project demonstrated an ability to work with advanced data structures and implement core algorithms used in professional 3D modeling packages.
The process began by establishing a correct and testable HEDS implementation, followed by the incremental implementation of the three Catmull-Clark point calculation rules. The primary challenge was ensuring the new half-edge structure was correctly formed after each subdivision step to maintain topological integrity.
Created a stable mesh processing pipeline that can take a general polygonal mesh (e.g., loaded from an OBJ file) and successfully generate a smooth limit surface after multiple iterations of the Catmull-Clark algorithm.
This served as a deep dive into the computational geometry principles underlying modern 3D modeling tools like Maya or Blender for University of Pennsylvania's Advanced Computer Graphics course 5610.
Code available on GitHub at jvrieger/Monte-Carlo-Path-Tracer
