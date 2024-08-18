#ifndef OPENGL_DRIVER_HPP
#define OPENGL_DRIVER_HPP

#include <array>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

class OpenGLDriver {
   private:
    GLuint g_program_id;
    GLuint g_vbo;
    GLuint g_vao;

    static bool GetShaderCode(const char* shader_file_path,
                              std::string* shader_source);
    static void DebugShaderLog(GLuint shader);
    static void DebugProgramLog(GLuint program);

   public:
    bool InitGL();
    void Render();
};

#endif  // OPENGL_DRIVER_HPP
