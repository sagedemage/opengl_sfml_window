#include "opengl_driver.hpp"

bool OpenGLDriver::InitGL() {
    // Graphics program
    GLuint g_program_id = 0;
    GLuint g_vbo = 0;
    GLuint g_vao = 0;

    const char* vertex_shader_file_path = "shader/shader.vert";
    const char* fragment_shader_file_path = "shader/shader.frag";

    std::string debug_msg;

    // Create vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // Get vertex source
    std::string vertex_shader_source_s;

    if (!GetShaderCode(vertex_shader_file_path, &vertex_shader_source_s)) {
        std::cout << "Unable to get vertex shader source code!" << std::endl;
        return false;
    }

    GLchar* vertex_shader_source =
        const_cast<GLchar*>(vertex_shader_source_s.c_str());

    // Set vertex source
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    // Compile vertex source
    glCompileShader(vertex_shader);

    // Check vertex source for errors
    GLint v_shader_compiled = GL_FALSE;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &v_shader_compiled);

    if (v_shader_compiled != GL_TRUE) {
        debug_msg = "Unable to compile vertex shader " +
                    std::to_string(vertex_shader) + "!";
        std::cout << debug_msg << std::endl;
        DebugShaderLog(vertex_shader);
        return false;
    }

    // Create fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Get fragment source
    std::string fragment_shader_source_s;

    if (!GetShaderCode(fragment_shader_file_path, &fragment_shader_source_s)) {
        std::cout << "Unable to get fragment shader source code!" << std::endl;
        return false;
    }

    GLchar* fragment_shader_source =
        const_cast<GLchar*>(fragment_shader_source_s.c_str());

    // Set fragment source
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);

    // Compile fragment source
    glCompileShader(fragment_shader);

    // Check fragment shader for errors
    GLint f_shader_compiled = GL_FALSE;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &f_shader_compiled);
    if (f_shader_compiled != GL_TRUE) {
        debug_msg = "Unable to compile fragment shader " +
                    std::to_string(fragment_shader) + "!";
        std::cout << debug_msg << std::endl;
        DebugShaderLog(fragment_shader);
        return false;
    }

    /* Link program */
    // Generate program
    g_program_id = glCreateProgram();

    // Attach vertex shader to program
    glAttachShader(g_program_id, vertex_shader);

    // Attach fragment shader to program
    glAttachShader(g_program_id, fragment_shader);

    glLinkProgram(g_program_id);

    // Check for errors
    GLint program_success = GL_TRUE;
    glGetProgramiv(g_program_id, GL_LINK_STATUS, &program_success);
    if (program_success != GL_TRUE) {
        debug_msg =
            "Error linking program " + std::to_string(g_program_id) + "!";
        std::cerr << debug_msg << std::endl;
        DebugProgramLog(g_program_id);
        return false;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Initilalize clear color
    glClearColor(0.F, 0.F, 0.F, 1.F);

    // VBO data
    std::array<GLfloat, 18> vertex_data = {
        // positions        // colors
        -0.5F, -0.5F, 0.0F, 1.0F, 0.0F, 0.0F,  // bottom left vertex
        0.5F,  -0.5F, 0.0F, 0.0F, 1.0F, 0.0F,  // bottom right vertex
        0.0F,  0.5F,  0.0F, 0.0F, 0.0F, 1.0F,  // top vertex
    };

    glGenVertexArrays(1, &g_vao);
    glGenBuffers(1, &g_vbo);
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data.data(),
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    this->g_vao = g_vao;
    this->g_vbo = g_vbo;
    this->g_program_id = g_program_id;

    glUseProgram(this->g_program_id);

    return true;
}

bool OpenGLDriver::GetShaderCode(const char* shader_file_path,
                                 std::string* shader_source) {
    /* Get shader source code from a file as a string */
    std::ifstream read_shader_file(shader_file_path);
    if (!read_shader_file.is_open()) {
        std::cout << "failed to open shader file: " +
                         static_cast<std::string>(shader_file_path)
                  << std::endl;
        return false;
    }

    std::string s_code;
    std::string line;

    while (getline(read_shader_file, line)) {
        // add line to the shader code
        s_code += line + "\n";
    }

    read_shader_file.close();
    *shader_source = s_code;

    return true;
}

void OpenGLDriver::DebugShaderLog(GLuint shader) {
    std::string debug_msg;
    // make sure name is shader
    if (glIsShader(shader)) {
        // Program log length
        int info_log_length = 0;
        int max_length = info_log_length;

        // Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);

        // Allocate string
        std::unique_ptr<char> info_log(new char[max_length]);

        // Get info log
        glGetShaderInfoLog(shader, max_length, &info_log_length,
                           info_log.get());

        if (info_log_length > 0) {
            // Print log
            std::cout << info_log.get() << std::endl;
        }
    } else {
        debug_msg = "Name " + std::to_string(shader) + " is not a shader";
        std::cerr << debug_msg << std::endl;
    }
}

void OpenGLDriver::DebugProgramLog(GLuint program) {
    std::string debug_msg;
    // make sure name is program
    if (glIsShader(program)) {
        // Program log length
        int info_log_length = 0;
        int max_length = info_log_length;

        // Get info string length
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &max_length);

        // Allocate string
        std::unique_ptr<char> info_log(new char[max_length]);

        // Get info log
        glGetShaderInfoLog(program, max_length, &info_log_length,
                           info_log.get());

        if (info_log_length > 0) {
            // Print log
            std::cout << info_log.get() << std::endl;
        }
    } else {
        debug_msg = "Name " + std::to_string(program) + " is not a program";
        std::cerr << debug_msg << std::endl;
    }
}

void OpenGLDriver::Render() {
    // clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw triangle
    glUseProgram(this->g_program_id);
    glBindVertexArray(this->g_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
