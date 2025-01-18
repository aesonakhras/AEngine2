-- premake5.lua
workspace "AEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "AEngine-App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Bullet-Physics"
   include "AEngine-Core/ThirdParty/Bullet/Build-Bullet.lua"
group ""

group "Core"
	include "AEngine-Core/Build-Core.lua"
group ""

include "AEngine-App/Build-App.lua"