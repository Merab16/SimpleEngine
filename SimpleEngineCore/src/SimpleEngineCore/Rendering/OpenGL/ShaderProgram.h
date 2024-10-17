#pragma once

namespace SimpleEngine {
    class ShaderProgram {
    private:
        bool isCompiled_ = false;
        unsigned int id_ = 0;

    private:



    public:
        ShaderProgram(const char* vertex_shader_src,
                    const char* fragment_shader_src);

        ShaderProgram(ShaderProgram&&);
        ShaderProgram& operator= (ShaderProgram&&);
        ~ShaderProgram();

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        void Bind() const;
        static void Unbind();
        bool IsCompiled() const { return isCompiled_; }



    };




}