#ifndef CALLBACK_INCLUDED
#define CALLBACK_INCLUDED

class Callback {
public:
	Callback() = default;
	virtual void onRClick() {};
	virtual void onRRelease() {};
	virtual void onLClick() {};
	virtual void onLRelease() {};
	virtual void onHover() {};
	virtual void onNotHover() {};
	virtual void onDrag(const ivec2& drag) {};
};

#endif