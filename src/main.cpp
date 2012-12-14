#include "GalaxyApp.hpp"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    try {
        GalaxyApp app;
        app.loop();
        return EXIT_SUCCESS;
    } catch (exception &e) {
        cerr << e.what() << endl;
    } catch (...) {
        cerr << "Something very bad happened..." << endl;
    }
    return EXIT_FAILURE;
}

