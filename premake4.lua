solution "Galaxy project"
   configurations { "Debug", "Release" }
   platforms {"native", "x64", "x32"}

   -- galaxy_project
   project "galaxy_project"
      kind "ConsoleApp"
      language "C++"
      files { "src/*.hpp", "src/*.cpp", "src/*.h", "src/physics/*.hpp", "src/physics/*.cpp", "src/gui/*.hpp", "src/gui/*.h", "src/gui/*.cpp" }
      includedirs { "lib/glfw/include", "src", "lib/" }
      links { "glew", "glfw", "stein" }
      defines { "GLEW_STATIC" }
     
      configuration { "linux" }
         links {"X11","Xrandr", "rt", "GL", "GLU", "pthread"}
       
      configuration { "windows" }
         links {"glu32","opengl32", "gdi32", "winmm", "user32"}

      configuration { "macosx" }
         linkoptions { "-framework OpenGL", "-framework Cocoa", "-framework IOKit" }
       
      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         targetdir "bin/debug/galaxy_project/"

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize"}    
         targetdir "bin/release/galaxy_project/"  

	-- GLFW Library
   project "glfw"
      kind "StaticLib"
      language "C"
      files {"lib/glfw/lib/*.h", "lib/glfw/lib/*.c", "lib/glfw/include/GL/glfw.h"}
      includedirs {"lib/glfw/lib", "lib/glfw/include"}

      configuration {"linux"}
         files {"lib/glfw/lib/x11/*.c", "lib/glfw/x11/*.h"}
         includedirs {"lib/glfw/lib/x11"}
         defines {"_GLFW_USE_LINUX_JOYSTICKS", "_GLFW_HAS_XRANDR", "_GLFW_HAS_PTHREAD" ,"_GLFW_HAS_SCHED_YIELD", "_GLFW_HAS_GLXGETPROCADDRESS"}
         buildoptions {"-pthread"}
       
      configuration {"windows"}
         files {"lib/glfw/lib/win32/*.c", "lib/glfw/win32/*.h"}
         includedirs {"lib/glfw/lib/win32"}
         defines {"_GLFW_USE_LINUX_JOYSTICKS", "_GLFW_HAS_XRANDR", "_GLFW_HAS_PTHREAD" ,"_GLFW_HAS_SCHED_YIELD", "_GLFW_HAS_GLXGETPROCADDRESS"}
       
      configuration {"Macosx"}
         files {"lib/glfw/lib/cocoa/*.c", "lib/glfw/lib/cocoa/*.h", "lib/glfw/lib/cocoa/*.m"}
         includedirs {"lib/glfw/lib/cocoa"}
         defines {}
         buildoptions {" -fno-common"}
         linkoptions {"-framework OpenGL", "-framework Cocoa", "-framework IOKit"}

      configuration "Debug"
         defines {"DEBUG"}
         flags {"Symbols"}
         targetdir "bin/debug"

      configuration "Release"
         defines {"NDEBUG"}
         flags {"Optimize"}    
         targetdir "bin/release"
		 
   -- GLEW Library         
   project "glew"
      kind "StaticLib"
      language "C"
      files {"lib/glew/*.c", "lib/glew/*.h"}
      defines { "GLEW_STATIC" }

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         targetdir "bin/debug"

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
         targetdir "bin/release"
		 
   -- Stein Library         
   project "stein"
      kind "StaticLib"
      language "C++"
      files {"lib/stein/*.cpp",	"lib/stein/*.hpp", "lib/stein/*.h", "lib/stein/math/*.cpp", "lib/stein/math/*.hpp", "lib/stein/math/*.h"}
	  includedirs { "lib/", "lib/glfw/include" }
	  links {"glew", "glfw" }
      defines { "GLEW_STATIC" }
     
      configuration { "linux" }
         links {"X11","Xrandr", "rt", "GL", "GLU", "pthread"}
       
      configuration { "windows" }
         links {"glu32","opengl32", "gdi32", "winmm", "user32"}

      configuration "Debug"
         defines { "DEBUG" }
         flags { "Symbols" }
         targetdir "bin/debug"

      configuration "Release"
         defines { "NDEBUG" }
         flags { "Optimize" }    
         targetdir "bin/release"