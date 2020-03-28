#include <istream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <thread>
#include <chrono>

#include "system.hpp"
#include "exception.hpp"
#include "disassembler.hpp"

using std::chrono::duration_cast;
using std::chrono::nanoseconds;

namespace gameboy
{

System::System(Renderer *r)
    : renderer_ {r}
{}

// system control

[[ noreturn ]] void System::run()
{
    for (;;)
    {
        auto start = std::chrono::high_resolution_clock::now();
        size_t cycles = step(1); // arbitrary number to keep sleep times higher bc nanosecond precision is unreliable
        auto finish = std::chrono::high_resolution_clock::now();
        double expected = NANOSECONDS_PER_CYCLE * cycles;
        auto duration = duration_cast<nanoseconds>(finish-start).count();
        if (duration < expected)
        {
            auto t = std::chrono::duration<double, std::nano>(expected-duration);
            std::this_thread::sleep_for(t);
        }
    }
}

void System::run_concurrently()
{
    auto run_fn = [this]{ this->run(); };
    std::thread t(run_fn);
    t.detach();
}

void System::reset()
{
    memory_.reset();
    cpu_.reset();
    ppu_.reset();
}

size_t System::step(size_t n)
{
    size_t cycles_passed = 0;
    for (size_t i {0}; i < n; ++i)
    {
        size_t old_cycles {cpu_.cycles()};
        cpu_.step();
        cycles_passed += (cpu_.cycles() - old_cycles);
        ppu_.step(cycles_passed);
        timer_.update(cycles_passed);
    }
    return cycles_passed;
}

void System::press(Joypad::Input i)
{
    joypad_.press(i);
}

void System::release(Joypad::Input i)
{
    joypad_.release(i);
}

// system setup

void System::load_cartridge(std::istream &is)
{
	memory_.load_cartridge(is);
}

bool System::load_cartridge(const std::string &path)
{
	std::ifstream rom {path, std::ios::binary};
	if (!rom.good())
        return false;
	memory_.load_cartridge(rom);
    return true;
}

void System::set_renderer(Renderer *r)
{
    renderer_.reset(r);
    ppu_.set_renderer(r);
}

// system debug



// callbacks to interface other components with memory

uint8_t System::memory_read(uint16_t adr)
{
	return memory_.read(adr);
}

void System::memory_write(uint8_t b, uint16_t adr)
{
	memory_.write(b, adr);
}

}
