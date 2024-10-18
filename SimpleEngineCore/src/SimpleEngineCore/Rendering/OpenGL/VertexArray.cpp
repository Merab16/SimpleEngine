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
	void VertexArray::AddVertexBuffer(const VertexBuffer& vertex_buffer) {
		Bind();
		vertex_buffer.Bind();

		for (const BufferElement& current_element : vertex_buffer.GetLayout().GetElements()) {
			glEnableVertexAttribArray(elementsCount_);
			glVertexAttribPointer(
				elementsCount_,
				static_cast<GLint>(current_element.component_count),
				current_element.component_type,
				GL_FALSE,
				static_cast<GLsizei>(vertex_buffer.GetLayout().GetStride()),
				reinterpret_cast<const void*>(current_element.offset)
			);
			++elementsCount_;
		}
		
	}

	void VertexArray::SetIndexBuffer(const IndexBuffer& index_buffer) {
		Bind();
		index_buffer.Bind();
		indicesCount_ = index_buffer.GetCount();
	}



	void VertexArray::Bind() const {
		glBindVertexArray(id_);
	}

	void VertexArray::Unbind() {
		glBindVertexArray(0);
	}

	



}