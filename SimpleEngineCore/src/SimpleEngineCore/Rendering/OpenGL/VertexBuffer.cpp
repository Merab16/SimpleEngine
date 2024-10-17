#include <glad/glad.h>

#include "VertexBuffer.h"
#include "SimpleEngineCore/Log.h"


namespace SimpleEngine {
    constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage) {
        switch (usage) {
            case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
            case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
            case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
        }

        LOG_ERROR("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }


    // constr && destr
    VertexBuffer::VertexBuffer(const void* data, size_t size, EUsage usage) {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &id_);
    }


    // move constr && move operator=
    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept {
        id_ = other.id_;
        other.id_ = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
        id_ = other.id_;
        other.id_ = 0;

        return *this;
    }

    // public
    void VertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    void VertexBuffer::Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }



}

