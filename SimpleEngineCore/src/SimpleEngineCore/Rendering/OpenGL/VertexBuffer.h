#pragma once 

#include <ctime>
#include <vector>

namespace SimpleEngine {

    enum class ShaderDataType {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    struct BufferElement {
        ShaderDataType type;
        uint32_t component_type;
        size_t component_count;
        size_t size;
        size_t offset;

        BufferElement(const ShaderDataType type);


    };

    class BufferLayout {
    private:
        std::vector<BufferElement> elements_;
        size_t stride_ = 0;

    private:


    public:
        BufferLayout(std::initializer_list<BufferElement> elements)
            : elements_(elements)
        {
            size_t offset = 0;
            stride_ = 0;
            for (auto& el : elements_) {
                el.offset = offset;
                offset += el.size;
                stride_ += el.size;
            }   
        }

        const std::vector<BufferElement>& GetElements() const { return elements_; }
        size_t GetStride() const { return stride_; }

    };


    class VertexBuffer {
    private:
        unsigned int id_ = 0;
        BufferLayout bufferLayout_;


    private:
        



    public:
        enum class EUsage {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void* data, size_t size, BufferLayout buffer_layout,
            EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&& other) noexcept;
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;
        
        void Bind() const;
        static void Unbind();

        const BufferLayout& GetLayout() const { return bufferLayout_; }


    };




}