#include "Window.hpp"
#include "CircleShape.hpp"
#include "RectShape.hpp"
#include "Button.hpp"
#include "CallBack.hpp"
#include "KeyState.hpp"
#include "Debug/Debug.hpp"
#include "Debug/Time.hpp"

#include<string>
#include<algorithm>

using namespace Pulsar;

const int WINDOW_WIDTH = 350;
const int WINDOW_HEIGHT = 200;

class HorrizontalMove : public Callback {
public:
    HorrizontalMove(Button* button, WindowBase* window) : m_button(button), m_window(window) {}
    virtual void onRClick() override {};
    virtual void onRRelease() override {};
    virtual void onLClick() override {  }
    virtual void onLRelease() override {};
    virtual void onHover() override { m_button->setColor(ivec3(150, 150, 150)); }
    virtual void onNotHover() override { m_button->setColor(ivec3(255, 255, 255)); }
    virtual void onDrag(const ivec2& drag) override { 
        m_button->setPosition(ivec2(
            std::max(0, std::min(WINDOW_WIDTH - (int)m_button->getWidth(), m_button->getPosition().x + drag.x)),
            m_button->getPosition().y
        ));
    }
private:
    WindowBase* m_window;
    Button* m_button;
};

struct Block {
    Rect* rect;
    bool eliminate = false;
};

Window* window;

Circle ball;
ivec2 direction(1, 1);

std::vector<Block> blocks;

Button* platform;

void SetupScene();
void checkCollision();

template<typename RectOrButton>
bool checkSphereToRect(Circle& circle, const RectOrButton* rect);

int main() {
    window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
    SetupScene();
    while (window->Alive()) {
        KeyState::update();
        checkCollision();
        ball.setPosition(ball.getPosition() + direction);
        window->fill();
        window->UpdateGadgets();
        window->draw(&ball);
        for (Block block : blocks) {
            if(!block.eliminate) window->draw(block.rect);
        }
        window->render();
    }
}

void SetupScene() {
    ball = Circle(4);
    ball.setPosition(ivec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2));
    ball.setColor(ivec3(255, 0, 0));
    const int blockWidth = 28, blockHeight = 25;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 12; ++j) {
            blocks.push_back(
                { new Rect(blockWidth, blockHeight), false }
            );
            blocks.back().rect->setPosition(ivec2(1 + j * (blockWidth+1), i * (blockHeight+1)));
            blocks.back().rect->setColor(ivec3(0, 255, 0));
        }
    }
    platform = new Button(80, 10);
    platform->setPosition(ivec2(WINDOW_WIDTH/2, 175));
    platform->setColor(ivec3(255, 0, 0));
    platform->addCallback(new HorrizontalMove(platform, window));
    window->addGadget(platform);
}

template<typename RectOrButton>
bool checkSphereToRect(Circle& circle, const RectOrButton* rect) {
    const int maxX = circle.getPosition().x + circle.getRadius();
    const int minX = circle.getPosition().x - circle.getRadius();
    const int maxY = circle.getPosition().y + circle.getRadius();
    const int minY = circle.getPosition().y - circle.getRadius();
    const int rectMaxX = rect->getPosition().x + rect->getWidth();
    const int rectMinX = rect->getPosition().x;
    const int rectMaxY = rect->getPosition().y + rect->getHeight();
    const int rectMinY = rect->getPosition().y;
    if (!(maxX > rectMinX && rectMaxX > minX && maxY > rectMinY && rectMaxY > minY)) {
        return false;
    }
    const int deltax = std::min(
        maxX - rectMinX,
        rectMaxX - minX
    );
    const int deltay = std::min(
        maxY - rectMinY,
        rectMaxY - minY
    );
    if (deltax < deltay) {
        direction.x *= -1;
        circle.setPosition(circle.getPosition() - ivec2(deltax, 0));
    }
    else if (deltax > deltay) {
        direction.y *= -1;
        circle.setPosition(circle.getPosition() - ivec2(0, deltay));
    }
    else {
        direction.x *= -1;
        circle.setPosition(circle.getPosition() - ivec2(deltax, 0));
        direction.y *= -1;
        circle.setPosition(circle.getPosition() - ivec2(0, deltay));
    }
    return true;
}

void checkCollision() {
    for (Block& block : blocks) {
        if (block.eliminate) continue;
        if (checkSphereToRect(ball, block.rect)) {
            block.eliminate = 1;
        }
    }
    checkSphereToRect(ball, platform);
    const int maxX = ball.getPosition().x + ball.getRadius();
    const int minX = ball.getPosition().x - ball.getRadius();
    const int maxY = ball.getPosition().y + ball.getRadius();
    const int minY = ball.getPosition().y - ball.getRadius();
    if (minX < 0) {
        ball.setPosition(ivec2(ball.getRadius(), ball.getPosition().y));
        direction.x *= -1;
    }
    if (maxX > WINDOW_WIDTH) {
        ball.setPosition(ivec2(WINDOW_WIDTH - ball.getRadius(), ball.getPosition().y));
        direction.x *= -1;
    }
    if (minY < 0) {
        ball.setPosition(ivec2(ball.getPosition().x, ball.getRadius()));
        direction.y *= -1;
    }
    if (maxY > WINDOW_HEIGHT) {
        ball.setPosition(ivec2(ball.getPosition().x, WINDOW_HEIGHT - ball.getRadius()));
        direction.y *= -1;
    }
}