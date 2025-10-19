#pragma once

#include <iostream>
#include <vector>
#include "vec.hpp"

class Model
{
public:
    Model();

    void addVertex(Vec3f vertex);

    void addFace(std::array<int, 3> face);
    
    Vec3f getVertex(int index) const;
    Vec3f getVertexFromFace(int face_index, int relative_index_in_face) const;

    int numberOfVertices() const;

    int numberOfFaces() const;

private:
    std::vector<Vec3f> _vertices; 
    
    std::vector<std::array<int, 3>> _faces;  
    

};

Model loadModel(std::string file_name);