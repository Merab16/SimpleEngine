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
		glm::vec3 rotation_; // x - Roll, y - Pitch, z = Yaw
		glm::mat4 viewMatrix_;
		glm::mat4 projectionMatrix_;

		glm::vec3 direction_;
		glm::vec3 right_;
		glm::vec3 up_;

		static constexpr glm::vec3 worldUp_{ 0.f, 0.f, 1.f };
		static constexpr glm::vec3 worldRight_{ 0.f, -1.f, 0.f };
		static constexpr glm::vec3 worldForward_{ 1.f, 0.f, 0.f };

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

		void MoveForward(float delta);
		void MoveRight(float delta);
		void MoveUp(float delta);

		const glm::vec3& GetCameraPosition() const { return position_; }
		const glm::vec3& GetCameraRotation() const { return rotation_; }


		// movement_delta.x - forward	pitch
		// movement_delta.y - right		roll
		// movement_delta.z - up		yaw
		void AddMovementAndRotation(const glm::vec3& movement_delta,
			const glm::vec3& rotation_delta);


	};



}