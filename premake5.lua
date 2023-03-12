workspace "BunnyEngine"
	architecture "x64"
	startproject "BunnyEngineEditor"

	configurations{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "BunnyEngine/SDK/GLFW/include"
IncludeDir["Glad"] = "BunnyEngine/SDK/Glad/include"
IncludeDir["ImGui"] = "BunnyEngine/SDK/imgui"
IncludeDir["glm"] = "BunnyEngine/SDK/glm"
IncludeDir["stb_image"] = "BunnyEngine/SDK/stb_image"
IncludeDir["entt"] = "BunnyEngine/SDK/entt/include"

group "Dependencise"
	include "BunnyEngine/SDK/GLFW"
	include "BunnyEngine/SDK/Glad"
	include "BunnyEngine/SDK/imgui"
group ""

project "BunnyEngine"
	location "BunnyEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "BEpch.h"
	pchsource "BunnyEngine/src/BEpch.cpp"


	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/SDK/stb_image/**.h",
		"%{prj.name}/SDK/stb_image/**.cpp",
		"%{prj.name}/SDK/glm/glm/**.hpp",
		"%{prj.name}/SDK/glm/glm/**.inl"
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/SDK/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"BE_PLATFORM_WINDOWS",
			"BE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}




	filter "configurations:Debug"
		defines "BE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BE_DIST"
		runtime "Release"
		optimize "on"


project "BunnyEngineEditor"
	location "BunnyEngineEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs{
		"BunnyEngine/SDK/spdlog/include",
		"BunnyEngine/src",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links{
		"BunnyEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines{
			"BE_PLATFORM_WINDOWS",
			"BE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}
		
	filter "configurations:Debug"
		defines "BE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BE_DIST"
		runtime "Release"
		optimize "on"