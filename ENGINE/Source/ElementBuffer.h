#pragma once

class EBO {
public:
    EBO(const unsigned int* indices, size_t size);
    ~EBO();

    void Bind() const;
    void Unbind() const;
private:
    unsigned int ebo;
};
