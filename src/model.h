/*
 * Copyright © 2008 Ben Smith
 * Copyright © 2010-2011 Linaro Limited
 *
 * This file is part of the glmark2 OpenGL (ES) 2.0 benchmark.
 *
 * glmark2 is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * glmark2 is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * glmark2.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *  Ben Smith (original glmark benchmark)
 *  Alexandros Frantzis (glmark2)
 */
#ifndef GLMARK2_MODEL_H_
#define GLMARK2_MODEL_H_

#include "mesh.h"

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>

enum ModelFormat
{
    MODEL_INVALID,
    MODEL_3DS,
    MODEL_OBJ
};

class ModelDescriptor
{
    std::string name_;
    std::string pathname_;
    ModelFormat format_;
    ModelDescriptor();
public:
    ModelDescriptor(const std::string& name, ModelFormat format, 
                    const std::string& pathname) :
        name_(name),
        pathname_(pathname),
        format_(format) {}
    ~ModelDescriptor() {}
    const std::string& pathname() const { return pathname_; }
    ModelFormat format() const { return format_; }
};

typedef std::map<std::string, ModelDescriptor*> ModelMap;

// A model as loaded from a 3D object data file
class Model
{
public:

    typedef enum {
        AttribTypePosition = 1,
        AttribTypeNormal = 2,
        AttribTypeTexcoord = 4,
        AttribTypeCustom = 8
    } AttribType;

    Model() {}
    ~Model() {}

    bool load_3ds(const std::string &filename);
    bool load_obj(const std::string &filename);
    bool load(const std::string& name);
    void calculate_normals();
    void convert_to_mesh(Mesh &mesh);
    void convert_to_mesh(Mesh &mesh, 
                         const std::vector<std::pair<AttribType, int> > &attribs);
    const LibMatrix::vec3& minVec() const { return minVec_; }
    const LibMatrix::vec3& maxVec() const { return maxVec_; }
    static const ModelMap& find_models();
private:
    struct Face {
        uint32_t a, b, c;
        uint16_t face_flags;
    };

    struct Vertex {
        LibMatrix::vec3 v;
        LibMatrix::vec3 n;
        LibMatrix::vec2 t;
    };

    struct Object {
        Object(const std::string &name) : name(name) {}
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<Face> faces;
    };

    void append_object_to_mesh(const Object &object, Mesh &mesh,
                               int p_pos, int n_pos, int t_pos);

    // For vertices of the bounding box for this model.
    void compute_bounding_box(const Object& object);
    LibMatrix::vec3 minVec_;
    LibMatrix::vec3 maxVec_;
    std::vector<Object> objects_;
};

#endif
