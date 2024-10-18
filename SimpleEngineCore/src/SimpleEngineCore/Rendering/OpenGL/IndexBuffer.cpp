#include <glad/glad.h>

#include "IndexBuffer.h"
#include "SimpleEngineCore/Log.h"


namespace SimpleEngine {
    constexpr GLenum UsageToGLenum(const VertexBuffer::EUsage usage) {
        switch (usage) {
            case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
            case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
            case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
        }

        LOG_ERROR("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }

    // const && destr
    IndexBuffer::IndexBuffer(const void* data, size_t count, VertexBuffer::EUsage usage)
        : count_(count)
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, UsageToGLenum(usage));
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &id_);
    }

    // move constr && move operator=
    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept 
        : id_(other.id_)
        , count_(other.count_)
    {
        other.id_ = 0;
        other.count_ = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
        id_ = other.id_;
        count_ = other.count_;
        other.id_ = 0;
        other.count_ = 0;

        return *this;
    }

    // public
    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void IndexBuffer::Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }




}