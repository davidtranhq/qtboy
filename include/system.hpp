#pragma once

#include <cstdint>
#include <istream>
#include <map>

#include "processor.hpp"
#include "memory.hpp"
#include "disassembler.hpp"
#include "ppu.hpp"
#include "renderer.hpp"
#include "debug_types.hpp"

namespace gameboy
{

class System
{
    public:

    explicit System(Renderer *r = nullptr);

    // system control
    void run();
    void reset();
    void step(size_t n);

    // system setup
    void load_cartridge(std::istream &is);
    bool load_cartridge(const std::string &path);
    void set_renderer(Renderer *r);

    // system debug
    friend class Debugger;

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
    Ppu ppu_
    {
        [this](uint16_t adr){ return this->memory_read(adr); },
        [this](uint8_t b, uint16_t adr){ this->memory_write(b, adr); }
    };
    // the renderer is an interface to facilitate the implementation of other
    // libraries for different platforms
    std::unique_ptr<Renderer> renderer_;
    bool memory_changed_ {false}; // true if memory was written to the last instruction
};
	
}
