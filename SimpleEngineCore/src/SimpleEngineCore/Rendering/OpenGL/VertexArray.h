#pragma once
#include "VertexBuffer.h"


namespace SimpleEngine {

    class VertexArray {
    private:




    private:
        unsigned int id_ = 0;
        unsigned int elementsCount_ = 0;



    public:


        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void AddBuffer(const VertexBuffer& vertex_buffer);
        void Bind() const;
        static void Unbind();


    };




}