#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace SimpleEngine {

    class VertexArray {
    private:




    private:
        unsigned int id_ = 0;
        unsigned int elementsCount_ = 0;
        size_t indicesCount_ = 0;



    public:


        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void AddVertexBuffer(const VertexBuffer& vertex_buffer);
        void SetIndexBuffer(const IndexBuffer& index_buffer);
        void Bind() const;
        static void Unbind();

        size_t GetIndicesCount() const { return indicesCount_; }

    };




}