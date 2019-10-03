#pragma once

#include <cstdint>
#include <istream>
#include <chrono>

#include "processor.hpp"

namespace gameboy
{
	
class System
{
	public:
	explicit System();
	
	void run();
	void load_cartridge(std::istream &is);
	void load_cartridge(const std::string &path);
	
	private:
	// callbacks to interface other components with memory
	uint8_t memory_read(uint16_t adr);
	void memory_write(uint8_t b, uint16_t adr);
	// components
	Memory memory_ {};
	Processor cpu_ 
	{
		[this](uint16_t adr){ return this->memory_read(adr); }, 
		[this](uint8_t b, uint16_t adr){ this->memory_write(b, adr); }
	};
	Display display_
	{
		[this](uint16_t adr){ return this->memory_read(adr); }, 
		[this](uint8_t b, uint16_t adr){ this->memory_write(b, adr); }
	};
	std::chrono::time_point<std::chrono::high_resolution_clock> last_vblank_;
	bool quit_ {false};
};
	
}
