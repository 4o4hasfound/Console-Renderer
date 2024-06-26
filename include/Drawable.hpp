#ifndef DRAWABLE_INCLUDED
#define DRAWABLE_INCLUDED

#include<vector>
#include "Math/Vector.hpp"

class WindowBase;

class Drawable {
	friend class WindowBase;
public:
	Drawable() = default;
	virtual ~Drawable() = default;
protected:
	virtual void draw(WindowBase* window) = 0;
};


#endif