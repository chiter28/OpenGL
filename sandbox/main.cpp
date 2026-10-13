#include "Core/Application.h"

#include <exception>
#include <iostream>

#include "Barrel.h"
#include "Cube.h"
#include "CubeInstancing.h"

  



int main()
{
    try
    {
        App app;
        app.SetDemo<Cube>();
        app.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Application failed: " << e.what() << '\n';
        return 1;
    }

  
    return 0;
}