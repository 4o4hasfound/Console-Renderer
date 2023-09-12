#include "Window.hpp"
#include "Font.hpp"
#include "Text.hpp"
#include "CircleShape.hpp"
#include "RectShape.hpp"
#include "Button.hpp"
#include "CallBack.hpp"
#include "KeyState.hpp"
#include "Debug/Debug.hpp"
#include "Debug/Time.hpp"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include<string>
#include<algorithm>
#include<stdlib.h>
#include<time.h>

const int WINDOW_WIDTH = 350;
const int WINDOW_HEIGHT = 200;

class HorrizontalMove : public Callback {
public:
    HorrizontalMove(Button* button, WindowBase* window) : m_button(button), m_window(window) {}
    virtual void onRClick() override {}
    virtual void onRRelease() override {}
    virtual void onLClick() override {}
    virtual void onLRelease() override {}
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

enum GameState {
    MENU,
    COUNTDOWN,
    START,
    END_WIN,
    END_LOSE
};
GameState state = GameState::MENU;

Time::Clock CountDownClock;

class StartGameCallback : public Callback {
public:
    StartGameCallback(Button* button) :
        m_button(button), m_width(button->getWidth()), m_height(button->getHeight()), m_pos(button->getPosition()) {}
    virtual void onRClick() override {}
    virtual void onRRelease() override {}
    virtual void onLClick() override {
        state = GameState::COUNTDOWN; 
        CountDownClock.reset();
    }
    virtual void onLRelease() override {}
    virtual void onHover() override {
        m_button->setColor(ivec3(150, 150, 150));
        m_button->setWidth(m_width + 6);
        m_button->setHeight(m_height + 6);
        m_button->setPosition(m_pos - ivec2(3, 3));
    }
    virtual void onNotHover() override {
        m_button->setColor(ivec3(255, 255, 255));
        m_button->setWidth(m_width);
        m_button->setHeight(m_height);
        m_button->setPosition(m_pos);
    }
    virtual void onDrag(const ivec2& drag) override {}
private:
    Button* m_button;
    uint32_t m_width, m_height;
    ivec2 m_pos;
};

class ExitGameCallback : public Callback {
public:
    ExitGameCallback(Button* button, WindowBase* window) : 
        m_button(button), m_window(window), 
        m_width(button->getWidth()), m_height(button->getHeight()), m_pos(button->getPosition()) {}
    virtual void onRClick() override {}
    virtual void onRRelease() override {}
    virtual void onLClick() override { m_window->Kill(); }
    virtual void onLRelease() override {}
    virtual void onHover() override { 
        m_button->setColor(ivec3(150, 150, 150));
        m_button->setWidth(m_width + 6);
        m_button->setHeight(m_height + 6);
        m_button->setPosition(m_pos - ivec2(3, 3));
    }
    virtual void onNotHover() override { 
        m_button->setColor(ivec3(255, 255, 255));
        m_button->setWidth(m_width);
        m_button->setHeight(m_height);
        m_button->setPosition(m_pos);
    }
    virtual void onDrag(const ivec2& drag) override {}
private:
    WindowBase* m_window;
    Button* m_button;
    uint32_t m_width, m_height;
    ivec2 m_pos;
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

bool gameEnd = 0;
int counter = 0;


Font font("OpenSans-Regular.ttf");
Text text(&font);
Text text2(&font);
Text text3(&font);
Text text4(&font);
Text scoreText(&font);
Text Title(&font);
Text startGameText(&font);
Text exitGameText(&font);
Text returnMenuText(&font);

Button *startGame, *exitGame, *returnMenu;

void SetupScene();
void SetupPlatform();
void SetupText();
void SetupButton();

void checkCollision();
void RenderBlocks();

void RunState();

void GameEnd();

template<typename RectOrButton>
bool checkSphereToRect(Circle& circle, const RectOrButton* rect);

class ReturnMenuCallback : public Callback {
public:
    ReturnMenuCallback(Button* button) :
        m_button(button), m_width(button->getWidth()), m_height(button->getHeight()), m_pos(button->getPosition()) {}
    virtual void onRClick() override {}
    virtual void onRRelease() override {}
    virtual void onLClick() override {
        state = GameState::MENU;
        for (auto& block : blocks) delete block.rect;
        blocks.clear();
        gameEnd = 0;
        counter = 0;
        SetupScene();
    }
    virtual void onLRelease() override {}
    virtual void onHover() override {
        m_button->setColor(ivec3(150, 150, 150));
        m_button->setWidth(m_width + 6);
        m_button->setHeight(m_height + 6);
        m_button->setPosition(m_pos - ivec2(3, 3));
    }
    virtual void onNotHover() override {
        m_button->setColor(ivec3(255, 255, 255));
        m_button->setWidth(m_width);
        m_button->setHeight(m_height);
        m_button->setPosition(m_pos);
    }
    virtual void onDrag(const ivec2& drag) override {}
private:
    Button* m_button;
    uint32_t m_width, m_height;
    ivec2 m_pos;
};

int main() {
    srand(time(NULL));
    window = new Window(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
    SetupScene();
    SetupPlatform();
    SetupText();
    SetupButton();
    while (window->Alive()) {
        KeyState::update();

        window->fill();

        RunState();

        window->render();
    }
    return 0;
}

void SetupScene() {
    direction = ivec2((rand() % 2) ? 1 : -1, 1);

    ball = Circle(4);
    ball.setPosition(ivec2(rand() % (WINDOW_WIDTH - 100) + 50, WINDOW_HEIGHT / 2));
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
}

void SetupPlatform() {
    platform = new Button(50, 10);
    platform->setPosition(ivec2(WINDOW_WIDTH / 2, 175));
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
        gameEnd = 1;
    }
}

void RenderBlocks() {
    bool hasBlock = 0;
    for (Block block : blocks) {
        if (!block.eliminate) {
            hasBlock = 1;
            window->draw(block.rect);
        }
    }
    if (!hasBlock) {
        state = END_WIN;
    }
}

void RunState() {
    switch (state) {
    case MENU: {
        platform->alive = false;
        startGame->alive = true;
        exitGame->alive = true;
        returnMenu->alive = false;

        window->draw(&Title);

        window->updateGadgets();
        window->renderGadgets();
        window->draw(&startGameText);
        window->draw(&exitGameText);

        break;
    }
    case COUNTDOWN: {
        platform->alive = true;
        startGame->alive = false;
        exitGame->alive = false;
        returnMenu->alive = false;
        
        text4.setColor(ivec3(0.0));
        text4.setScale(13);
        text4.setSpace(2);
        text4.setString("Start : " + std::to_string(3 - (int)(CountDownClock.getSec())));
        text4.setPosition(platform->getPosition() + ivec2(2, -2));
        RenderBlocks();

        if (CountDownClock.getSec() >= 3.0) state = START;

        window->updateGadgets();
        window->renderGadgets();
        window->draw(&text4);
        window->draw(&ball);

        break;
    }
    case START: {
        platform->alive = true;
        startGame->alive = false;
        exitGame->alive = false;
        returnMenu->alive = false;

        checkCollision();
        RenderBlocks();
        ball.setPosition(ball.getPosition() + direction);

        int score = 0;
        for (const auto& b : blocks) score += b.eliminate;
        scoreText.setString("Score: " + std::to_string(score));
        window->draw(&scoreText);

        if (score == blocks.size()) state = END_WIN;
        if (gameEnd) state = END_LOSE;

        window->updateGadgets();
        window->renderGadgets();
        window->draw(&ball);

        break;
    }
    case END_WIN: {
        platform->alive = true;
        startGame->alive = false;
        exitGame->alive = false;
        returnMenu->alive = true;

        text.setString("You've won! GG");
        text.setPosition(ivec2(100, 50));
        if (text.getScale() == 40.0) text.setScale(10.0);
        else if (++counter == 10) {
            text.setScale(text.getScale() + 1); counter = 0;
        }
        text.setColor(ivec3(255));

        window->updateGadgets();
        window->renderGadgets();
        window->draw(&returnMenuText);
        window->draw(&text);
        window->draw(&ball);

        break;
    }
    case END_LOSE: {
        platform->alive = true;
        startGame->alive = false;
        exitGame->alive = false;
        returnMenu->alive = true;

        text.setPosition(ivec2(100, 15));
        text2.setPosition(ivec2(100, 45));
        text3.setPosition(ivec2(100, 75));
        text.setScale(30.0);
        text2.setScale(30.0);
        text3.setScale(30.0);

        platform->alive = false;

        window->draw(&ball);
        RenderBlocks();
        window->draw(platform);

        window->draw(&text);
        window->draw(&text2);
        window->draw(&text3);

        window->updateGadgets();
        window->renderGadgets();
        window->draw(&returnMenuText);

        break;
    }

    }
}
void SetupText() {
    text.setString("Game End!!!");
    text.setColor(ivec3(139, 0, 0));
    text.setSpace(1);

    text2.setString("You are so bad :(");
    text2.setColor(ivec3(139, 0, 0));
    text2.setSpace(1);

    text3.setString("Get Better :)");
    text3.setColor(ivec3(139, 0, 0));
    text3.setSpace(1);

    text4.setColor(ivec3(0, 0, 0));
    text4.setSpace(1);
    text4.setScale(13);

    scoreText.setColor(ivec3(255, 255, 255));
    scoreText.setSpace(1);
    scoreText.setScale(20);
    scoreText.setPosition(ivec2(20, 20));

    Title.setString("Brick Breaker GAME !!!");
    Title.setColor(ivec3(255, 0, 0));
    Title.setSpace(2);
    Title.setScale(30);
    Title.setPosition(ivec2(50, 30));

    startGameText.setString("Start Game");
    startGameText.setColor(ivec3(0, 0, 0));
    startGameText.setSpace(3);
    startGameText.setScale(25);
    startGameText.setPosition(ivec2(115, 70));

    exitGameText.setString("Exit Game");
    exitGameText.setColor(ivec3(0, 0, 0));
    exitGameText.setSpace(3);
    exitGameText.setScale(25);
    exitGameText.setPosition(ivec2(120, 110));

    returnMenuText.setString("Return Menu");
    returnMenuText.setColor(ivec3(0, 0, 0));
    returnMenuText.setSpace(3);
    returnMenuText.setScale(25);
    returnMenuText.setPosition(ivec2(105, 110));
}

void SetupButton() {
    startGame = new Button(150, 25);
    startGame->setPosition(ivec2(100, 70));
    startGame->setColor(ivec3(255, 255, 255));
    startGame->addCallback(new StartGameCallback(startGame));
    window->addGadget(startGame);

    exitGame = new Button(150, 25);
    exitGame->setPosition(ivec2(100, 110));
    exitGame->setColor(ivec3(255, 255, 255));
    exitGame->addCallback(new ExitGameCallback(exitGame, window));
    window->addGadget(exitGame);

    returnMenu = new Button(150, 25);
    returnMenu->setPosition(ivec2(100, 110));
    returnMenu->setColor(ivec3(255, 255, 255));
    returnMenu->addCallback(new ReturnMenuCallback(returnMenu));
    window->addGadget(returnMenu);
}

void GameEnd() { 

}
