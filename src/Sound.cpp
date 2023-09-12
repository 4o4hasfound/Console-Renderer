#include "Sound.hpp"

Sound::Sound(const std::string& filename) {
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	m_buffer.resize(size);
	file.read(m_buffer.data(), size);
}
