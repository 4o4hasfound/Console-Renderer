#ifndef SOUND_INCLUDED
#define SOUND_INCLUDED

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <fstream>
#include <vector>

class Sound {
public:
	Sound(const std::string& filename);

private:
	std::vector<char> m_buffer;
};



#endif