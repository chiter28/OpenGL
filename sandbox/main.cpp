#include "Core/Application.h"

#include "Barrel.h"
#include "Cube.h"
#include "CubeInstancing.h"

  

int main()
{
    App app;

     
    app.SetSandbox<Cube>();
      
    app.Run();

    return 0;
}