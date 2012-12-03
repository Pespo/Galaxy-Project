#include <string.h>
#include <iostream>
#include "common/Application.hpp"

#ifndef DEBUG_PRINT
#define DEBUG_PRINT 1
#endif

#if DEBUG_PRINT == 0
#define debug_print(FORMAT, ...) ((void)0)
#else
#ifdef _MSC_VER
#define debug_print(FORMAT, ...) \
    fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
        __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#else
#define debug_print(FORMAT, ...) \
    fprintf(stderr, "%s() in %s, line %i: " FORMAT "\n", \
        __func__, __FILE__, __LINE__, __VA_ARGS__)
#endif 
#endif 

int main( int argc, char **argv )
{
    size_t width = 1024, height = 768 ;
    char* title = "galaxy project";

    // Initialise GLFW
    Application* galaxy = new Application(width, height, title);

	do{

	} while(true==true);

    exit( EXIT_SUCCESS );
}

