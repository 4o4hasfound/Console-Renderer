# Console Renderer
## Description
This application utilize the native window console as a render window.  
It will decrease the character size to make each character act like an ordinary pixel.

## Basic usage
```c++
Window window = new Window(windowWidth, windowHeight, characterSize);
while(window->Alive()) {
        KeyState::update();
        window->fill();

        // Do some calculation
        // Render some things
        window->updateGadgets();
        window->renderGadgets();

        window->render();
}
```
## Shapes
```c++
// Basically any class that is inherited from the Drawable class
Rect rect(rectWidth, rectHeight);
rect.setPosition(ivec2(yourPosition));
rect.setColor(ivec3(yourColor));
window->draw(&rect);

Circle circle(circleRadius);
circle.setPosition(ivec2(yourPosition));
circle.setColor(ivec3(yourColor));
window->draw(&circle);
```
## Gadgets
```c++
// Basically any class that is inherited from Gadget class
Button* button = new Button(buttonWidth, buttonHeight);
button->setPosition(ivec2(yourPosition));
button->setColor(ivec3(yourColor));
button->addCallback(new YourCallBackClass(button));
window->addGadget(button);

// Callbacks looks like this
class ButtonCallback : public Callback {
public:
    StartGameCallback(Button* button) : m_button(button) {}
    virtual void onRClick() override {}
    virtual void onRRelease() override {}
    virtual void onLClick() override {}
    virtual void onLRelease() override {}
    virtual void onHover() override {}
    virtual void onNotHover() override {}
    virtual void onDrag(const ivec2& drag) override {}
private:
    Button* m_button;
};
```