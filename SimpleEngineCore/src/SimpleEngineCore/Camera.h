#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace SimpleEngine {

	class Camera {
	public:
		enum class ProjectionMode {
			Perspective,
			Orthographic
		};

	private:
		ProjectionMode projectionMode_;
		glm::vec3 position_;
		glm::vec3 rotation_;
		glm::mat4 viewMatrix_;
		glm::mat4 projectionMatrix_;


	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();



	public:
		Camera(const glm::vec3& position = {0, 0, 0},
			const glm::vec3& rotation = {0, 0, 0},
			const ProjectionMode projectionMode = ProjectionMode::Perspective);

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetPositionRotation(const glm::vec3& position,
			const glm::vec3& rotation);
		void SetProjectionMode(ProjectionMode projectionMode);
		glm::mat4 GetViewMatrix() const { return viewMatrix_; }
		glm::mat4 GetProjectionMatrix() const { return projectionMatrix_; }


	};



}