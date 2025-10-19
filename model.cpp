#include "model.h"
#include <stdexcept>

#include <array>
#include <sstream>
#include <fstream>
Model::Model() {}

void Model::addVertex(Vec3f vertex)
{
    _vertices.push_back(vertex);
}

void Model::addFace(std::array<int, 3> face)
{
    _faces.push_back(face);
}

Vec3f Model::getVertex(int index)const
{
    assert(index > 0 && index < _vertices.size());
    return _vertices[index];
}

Vec3f Model::getVertexFromFace(int face_index, int relative_index_in_face) const
{
    assert(face_index > 0 && face_index < _faces.size());
    assert(relative_index_in_face > 0 && relative_index_in_face < 3);
    return _faces[face_index][relative_index_in_face];
}

int Model::numberOfVertices() const
{
    return _vertices.size();
}

int Model::numberOfFaces() const
{
    return _faces.size();
}


Model loadModel(std::string file_name)
{
    std::ifstream in;
    in.open(file_name, std::ifstream::in);
    if (in.fail()) throw std::runtime_error("Could not open file");
    Model model;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i : {0,1,2}) iss >> v[i];
            model.addVertex(v);
        } else if (!line.compare(0, 2, "f ")) {
            int f,t,n, cnt = 0;
            iss >> trash;
            std::array<int, 3> face_vertex_array{0};
            while (iss >> f >> trash >> t >> trash >> n) {
                face_vertex_array[cnt] = f - 1;
                cnt++;
            }
            if (3!=cnt) {
                std::cerr << "Error: the obj file is supposed to be triangulated" << std::endl;
                throw std::runtime_error("The obj file is supposed to be triangulated");
            }
            model.addFace(face_vertex_array);
        }
    }
    std::cout << "# v# " << model.numberOfVertices() << " f# "  << model.numberOfFaces() << std::endl;
    return model;
}