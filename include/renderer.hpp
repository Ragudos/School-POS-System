#pragma once

#include <sstream>

using namespace std;

class Renderer {
   public:
    ostringstream buf;

   public:
    void render() noexcept;
};

Renderer& getRenderer();
void initializeRenderer();
