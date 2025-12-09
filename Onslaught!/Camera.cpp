#include "Camera.hpp"

Camera::Camera(sf::RenderWindow& window, float width, float height) 
    : renderWindow(window), cameraView(sf::FloatRect({0.f, 0.f} , {width, height})), cameraDeadzone(200.f)
{
    renderWindow.setView(cameraView);
}

//the position should be center of player, not corner
void Camera::follow(const sf::Vector2f& targetPos) {

    
    sf::Vector2f cameraCenter = cameraView.getCenter();
    sf::Vector2f cameraSize = cameraView.getSize();

    sf::Vector2f newCameraPos;
    newCameraPos = cameraCenter + (targetPos - cameraCenter) * smoothing;

    

    cameraView.setCenter(newCameraPos);
}

void Camera::apply() {
    renderWindow.setView(cameraView);
}

sf::View Camera::getView() const {
    return cameraView;
}

sf::Vector2f Camera::getPos() const {
    return cameraView.getCenter();
}