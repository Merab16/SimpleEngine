#pragma once 

#include <ctime>

namespace SimpleEngine {

    class VertexBuffer {
    private:




    private:
        unsigned int id_ = 0;



    public:
        enum class EUsage {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void* data, size_t size, EUsage usage = VertexBuffer::EUsage::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&& other) noexcept;
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;
        
        void Bind() const;
        static void Unbind();


    };




}