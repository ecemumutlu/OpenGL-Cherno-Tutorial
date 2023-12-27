#include "IndexBuffer.h"
#include "Renderer.h"
#include <cassert>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) 
    : mCount(count) 
{

    assert(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &mRendererId); // (number of buffers desired, the id of the buffer will be stored here by the function)
    //After getting a buffer ID, we need to select a buffer. 
    // A buffer is selected by Bind buffer as such
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId); // we simply say that our buffer is an array
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &mRendererId);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId); // we simply say that our buffer is an array
}

void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // we simply say that our buffer is an array
}