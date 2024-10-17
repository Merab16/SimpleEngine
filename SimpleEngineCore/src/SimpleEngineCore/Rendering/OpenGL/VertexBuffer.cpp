#include <glad/glad.h>

#include "VertexBuffer.h"
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

    constexpr unsigned int ShaderDataTypeToComponentsCount(const ShaderDataType type) {
        switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Int:
            return 1;

        case ShaderDataType::Float2:
        case ShaderDataType::Int2:
            return 2;

        case ShaderDataType::Float3:
        case ShaderDataType::Int3:
            return 3;

        case ShaderDataType::Float4:
        case ShaderDataType::Int4:
            return 4;
        }
        LOG_ERROR("ShaderDataTypeToComponentsCount: unknown ShaderDataType");
        return 0;
    }

    constexpr size_t ShaderDataTypeSize(const ShaderDataType type) {
        switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return sizeof(GLfloat) * ShaderDataTypeToComponentsCount(type);

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return sizeof(GLint) * ShaderDataTypeToComponentsCount(type);
        }

        LOG_ERROR("ShaderDataTypeSize: unknown ShaderDataType");
        return 0;
    }

    constexpr unsigned int ShaderDataTypeToComponentType(const ShaderDataType type) {
        switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return GL_FLOAT;

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        }

        LOG_ERROR("ShaderDataTypeToComponentType: unknown ShaderDataType");
        return GL_FLOAT;
    }

    //=========================BufferElement=========================//
    BufferElement::BufferElement(const ShaderDataType type)
        : type(type)
        , component_type(ShaderDataTypeToComponentType(type))
        , component_count(ShaderDataTypeToComponentsCount(type))
        , size(ShaderDataTypeSize(type))
        , offset(0) {}




    //=========================VertexBuffer=========================//
    // constr && destr
    VertexBuffer::VertexBuffer(const void* data, size_t size, BufferLayout buffer_layout,
        EUsage usage)
        : bufferLayout_(std::move(buffer_layout))
    {
        glGenBuffers(1, &id_);
        glBindBuffer(GL_ARRAY_BUFFER, id_);
        glBufferData(GL_ARRAY_BUFFER, size, data, UsageToGLenum(usage));
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &id_);
    }


    // move constr && move operator=
    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        : id_(other.id_)
        , bufferLayout_(std::move(other.bufferLayout_))
    {
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

