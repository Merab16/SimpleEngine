#pragma once

#include <glad/glad.h>


#include "VertexArray.h"
#include "SimpleEngineCore/Log.h"


namespace SimpleEngine {
	// constr && destr
	VertexArray::VertexArray() {
		glGenVertexArrays(1, &id_);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &id_);
	}

	// move constr && move operator=
	VertexArray::VertexArray(VertexArray&& other) noexcept
		: id_(other.id_)
		, elementsCount_(other.elementsCount_)
	{
		other.id_ = 0;
		other.elementsCount_ = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
		id_ = other.id_;
		elementsCount_ = other.elementsCount_;
		other.id_ = 0;
		other.elementsCount_ = 0;

		return *this;
	}

	// public
	void VertexArray::Bind() const {
		glBindVertexArray(id_);
	}

	void VertexArray::Unbind() {
		glBindVertexArray(0);
	}

	void VertexArray::AddBuffer(const VertexBuffer& vertex_buffer) {
		Bind();
		vertex_buffer.Bind();

		glEnableVertexAttribArray(elementsCount_);
		glVertexAttribPointer(elementsCount_, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		++elementsCount_;
	}




}