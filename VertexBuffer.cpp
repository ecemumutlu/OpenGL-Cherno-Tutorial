#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    glGenBuffers(1, &mRendererId); // (number of buffers desired, the id of the buffer will be stored here by the function)
    //After getting a buffer ID, we need to select a buffer. 
    // A buffer is selected by Bind buffer as such
    glBindBuffer(GL_ARRAY_BUFFER, mRendererId); // we simply say that our buffer is an array
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &mRendererId);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, mRendererId); // we simply say that our buffer is an array
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0); // we simply say that our buffer is an array
}