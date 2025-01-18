project "Bullet"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    includedirs {
        "src",
        "../OpenCL"
    }

    files {
        "src/**.cpp",
        "src/**.h",
    }

    targetdir ("../../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        architecture "x64"
        defines { "BT_USE_SSE_IN_API" }
        libdirs { "../OpenCL/lib" }
        links { "OpenCL" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
    defines { "RELEASE" }
        runtime "Release"
        optimize "On"