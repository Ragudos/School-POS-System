#pragma once

#include <sstream>

using namespace std;

class Renderer {
    public:
        static Renderer& getInstance();
    private:
        Renderer() {};
    public:
        Renderer(Renderer const&) = delete;
        void operator=(Renderer const&) = delete;
    // Note: Scott Meyers mentions in his Effective Modern
    //       C++ book, that deleted functions should generally
    //       be public as it results in better error messages
    //       due to the compilers behavior to check accessibility
    //

    public:
        ostringstream buf;

    public:
        void render() noexcept;
};
