project "AEngine-App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
    
	  -- Include Core
	  "../AEngine-Core/Source",
      "../AEngine-Core/ThirdParty/entt/include",

      --Todo remove this later
      "../AEngine-Core/ThirdParty/Bullet/src"
   }

   links
   {
      "AEngine-Core"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

      -- move the assets into the build folder
      postbuildcommands {
        "{COPYDIR} \"../AEngine-App/Assets\" \"%{cfg.targetdir}/Assets\"",
        "{COPY} \"../AEngine-Core/ThirdParty/SDL2.0/bin/x64/SDL2.dll\" \"%{cfg.targetdir}\"",
        "{COPY} \"../AEngine-Core/ThirdParty/OpenCL/bin/x64/OpenCL.dll\" \"%{cfg.targetdir}\""
     }

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

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