#pragma once

class VertexBuffer {
private :
	unsigned int mRendererId; // opengl identifies the specfic objects by a specific id 
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};