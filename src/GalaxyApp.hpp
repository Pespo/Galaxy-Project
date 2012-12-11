#pragma once

#include <stein/Application.hpp>

class GalaxyApp : public stein::Application {
public:
    GalaxyApp();
    virtual void animate();
};