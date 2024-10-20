#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>


#include "SimpleEngineCore/Camera.h"


namespace SimpleEngine {
	// constr & destr
	Camera::Camera(const glm::vec3& position,
		const glm::vec3& rotation,
		const ProjectionMode projectionMode)
		: position_(position)
		, rotation_(rotation)
		, projectionMode_(projectionMode)
	{

		UpdateViewMatrix();
		UpdateProjectionMatrix();


	}

	// private
	void Camera::UpdateViewMatrix() {

		const float roll_in_radians		= glm::radians(rotation_.x);
		const float pitch_in_radians	= glm::radians(rotation_.y);
		const float yaw_in_radians		= glm::radians(rotation_.z);


		glm::mat3 rotate_matrix_x(
			1, 0, 0,
			0, cos(roll_in_radians), sin(roll_in_radians),
			0, -sin(roll_in_radians), cos(roll_in_radians)
		);
		 
		glm::mat3 rotate_matrix_y(
			cos(pitch_in_radians), 0, -sin(pitch_in_radians),
			0, 1, 0,
			sin(pitch_in_radians), 0, cos(pitch_in_radians)
		);

		glm::mat3 rotate_matrix_z(
			cos(yaw_in_radians), sin(yaw_in_radians), 0,
			-sin(yaw_in_radians), cos(yaw_in_radians), 0,
			0, 0, 1
		);

		const glm::mat3 euler_rotate_matrix =
			rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;

		direction_ = glm::normalize(euler_rotate_matrix * worldForward_);
		right_ = glm::normalize(euler_rotate_matrix * worldRight_);
		up_ = glm::cross(right_, direction_);



		viewMatrix_ = glm::lookAt(position_, position_ + direction_, up_);
	}

	void Camera::UpdateProjectionMatrix() {
		if (projectionMode_ == ProjectionMode::Perspective) {
			float r = 0.1f;
			float t = 0.1f;
			float f = 10.f;
			float n = 0.1f; 
			projectionMatrix_ = glm::mat4(
				n / r, 0, 0, 0,
				0, n / t, 0, 0,
				0, 0, (-f - n) / (f - n), -1,
				0, 0, -2 * f * n / (f - n), 0

			);
		}
		else {
			float r = 2.f;
			float t = 2.f;
			float f = 100.f;
			float n = 0.1f;
			projectionMatrix_ = glm::mat4(
				1 / r, 0, 0, 0,
				0, 1 / t, 0, 0,
				0, 0, -2 / (f - n), 0,
				0, 0, (-f - n) / (f - n), 1
			);
		}
	}


	// public
	void Camera::SetPosition(const glm::vec3& position) {
		position_ = position;
		UpdateViewMatrix();
	}

	void Camera::SetRotation(const glm::vec3& rotation) {
		rotation_ = rotation;
		UpdateViewMatrix();
	}

	void Camera::SetPositionRotation(const glm::vec3& position, const glm::vec3& rotation) {
		position_ = position;
		rotation_ = rotation;
		UpdateViewMatrix();
	}

	void Camera::SetProjectionMode(ProjectionMode projectionMode) {
		projectionMode_ = projectionMode;
		UpdateProjectionMatrix();
	}

	void Camera::MoveForward(float delta) {
		position_ += direction_ * delta;
		UpdateViewMatrix();
	}

	void Camera::MoveRight(float delta) {
		position_ += right_ * delta;
		UpdateViewMatrix();
	}

	void Camera::MoveUp(float delta) {
		position_ += up_  * delta;
		UpdateViewMatrix();
	}

	void Camera::AddMovementAndRotation(const glm::vec3& movement_delta,
		const glm::vec3& rotation_delta) {
		position_ += direction_ * movement_delta.x;
		position_ += right_ * movement_delta.y;
		position_ += up_ * movement_delta.z;
		rotation_ += rotation_delta;
		UpdateViewMatrix();
	}


}