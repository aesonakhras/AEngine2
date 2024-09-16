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
      "ThirdParty/assimp/include"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "D3D11_MODE" }

        -- Handle x64 libraries for Debug and Release
        filter { "architecture:x64", "configurations:Debug" }
        libdirs { "ThirdParty/assimp/libs/Debug/x64" }
        links { 
            "assimp-vc143-mtd",
            "zlibstaticd" 
        }

        filter { "architecture:x64", "configurations:Release" }
        libdirs { "ThirdParty/assimp/libs/Release/x64" }
        links { 
            "assimp-vc143-mt",
            "zlibstatic"
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