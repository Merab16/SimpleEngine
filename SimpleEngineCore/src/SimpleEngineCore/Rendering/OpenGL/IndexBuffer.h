#pragma once

#include "VertexBuffer.h"

namespace SimpleEngine {

    class IndexBuffer {
    private:
        unsigned int id_ = 0;
        size_t count_ = 0;


    private:



    public:
        // constr && destr
        IndexBuffer(const void* data, size_t count, VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&& other) noexcept;
        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        void Bind() const;
        static void Unbind();
        size_t GetCount() const { return count_; }


    };
}