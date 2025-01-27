project "AEngine-Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
      "ThirdParty/stbi/include",
      "ThirdParty/assimp/include",
      "ThirdParty/SDL2.0/include",
      "ThirdParty/entt/include",
      "ThirdParty/Bullet/src",
      "ThirdParty/OpenCL",
      "ThirdParty/KTX/include"
   }

   links
   {
      "Bullet"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")



   filter "system:windows"
       systemversion "latest"
       defines { "D3D11_MODE", "BT_USE_SSE_IN_API", "KHRONOS_STATIC" }

        -- Handle x64 libraries for all
        filter { "architecture:x64" }
            libdirs {
                "ThirdParty/SDL2.0/lib/x64"
            }
            links { 
                "SDL2",
                "SDL2main"
            }

        -- Handle x64 libraries for Debug
        filter { "architecture:x64", "configurations:Debug" }
            libdirs {
                "ThirdParty/assimp/lib/Debug/x64",
                "ThirdParty/KTX/lib/Debug/x64" 
            }
            links { 
                "assimp-vc143-mtd",
                "zlibstaticd",
                "ktx-d"
            }

        -- Handle x64 libraries for Release
        filter { "architecture:x64", "configurations:Release" }
            libdirs {
                "ThirdParty/assimp/lib/Release/x64",
                "ThirdParty/KTX/lib/Release/x64"
            }
            links { 
                "assimp-vc143-mt",
                "zlibstatic",
                "ktx"
            }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"