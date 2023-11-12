#pragma once

class VAO {
public:
    VAO();
    ~VAO();

    void Bind() const;
    void Unbind() const;
private:
    unsigned int vao;
};