/*
 tdogl::Camera

 Copyright 2012 Thomas Dalling - http://tomdalling.com/

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
#define _USE_MATH_DEFINES
#include <cmath>
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace tdogl;

Camera::Camera() :
    _position(0.0f, 0.0f, 0.0f),
    _up(0.0f, 1.0f, 0.0f),
    _right(1.0f, 0.0f, 0.0f),
    _forward(0.0f, 0.0f, -1.0f),

    _fieldOfView(50.0f),
    _nearPlane(0.01f),
    _farPlane(100.0f),
    _viewportAspectRatio(4.0f/3.0f)
{
}

const glm::vec3& Camera::position() const {
    return _position;
}

void Camera::setPosition(const glm::vec3& position) {
    _position = position;
}

void Camera::offsetPosition(const glm::vec3& offset) {
    _position += offset;
}

glm::vec3 Camera::forward() const {
    return _forward;
}

glm::vec3 Camera::right() const {
    return _right;
}

glm::vec3 Camera::up() const {
    return _up;
}

float Camera::fieldOfView() const {
    return _fieldOfView;
}

void Camera::setFieldOfView(float fieldOfView) {
    assert(fieldOfView > 0.0f && fieldOfView < 180.0f);
    _fieldOfView = fieldOfView;
}

float Camera::nearPlane() const {
    return _nearPlane;
}

float Camera::farPlane() const {
    return _farPlane;
}

void Camera::setNearAndFarPlanes(float nearPlane, float farPlane) {
    assert(nearPlane > 0.0f);
    assert(farPlane > nearPlane);
    _nearPlane = nearPlane;
    _farPlane = farPlane;
}

glm::mat4 Camera::orientation() const {
    glm::mat4 orientation;

    orientation = glm::mat4(
        _right.x, _up.x, -_forward.x, 0, // first column (not row!)
        _right.y, _up.y, -_forward.y, 0,
        _right.z, _up.z, -_forward.z, 0,
               0,     0,           0, 1
    );
    
     return orientation;
}

void Camera::fly(float distance){
    _position = _position + distance * forward();
    
}
void Camera::pitch(float degree){
    
    glm::mat4 pitching = glm::rotate(glm::mat4(), glm::radians(degree), right());
    
    //pitching = glm::inverse(pitching);
    
    _forward = glm::vec3( pitching * glm::vec4(_forward, 1.0f) );
    _up = glm::vec3( pitching * glm::vec4(_up, 1.0f) );
    
}
void Camera::yaw(float degree){
    
    glm::mat4 yawing = glm::rotate(glm::mat4(), glm::radians(degree), up());
    
    _right = glm::vec3( yawing * glm::vec4(right(), 1.0f) );
    _forward = glm::vec3( yawing * glm::vec4(forward(), 1.0f) );
    
}
void Camera::roll(float degree){
    
    glm::mat4 rolling = glm::rotate(glm::mat4(), glm::radians(degree), forward());
    
    _right = glm::vec3( rolling * glm::vec4(right(), 1.0f) );
    _up = glm::vec3( rolling * glm::vec4(up(), 1.0f) );
    
}

float Camera::viewportAspectRatio() const {
    return _viewportAspectRatio;
}

void Camera::setViewportAspectRatio(float viewportAspectRatio) {
    assert(viewportAspectRatio > 0.0);
    _viewportAspectRatio = viewportAspectRatio;
}

glm::mat4 Camera::matrix() const {
    return projection() * view();
}

glm::mat4 Camera::projection() const {
    return glm::perspective(glm::radians(_fieldOfView), _viewportAspectRatio, _nearPlane, _farPlane);
}

glm::mat4 Camera::view() const {
    return orientation() * glm::translate(glm::mat4(), -_position);
}