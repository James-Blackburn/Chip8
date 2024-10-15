#pragma once

#include <vector>
#include <string>

struct Shader {
    std::string source;
    unsigned int type;
    unsigned int glId;
};

class ShaderProgram {
    private:
        bool initialised = false;
        unsigned int glId;
    public:
        ~ShaderProgram();
        void create(std::vector<Shader> shaders);
        inline unsigned int getId() const;
};

unsigned int ShaderProgram::getId() const { 
    return glId;
}