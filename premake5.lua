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
IncludeDir["GLFW"] = "%{wks.location}/BunnyEngine/SDK/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/BunnyEngine/SDK/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/BunnyEngine/SDK/imgui"
IncludeDir["glm"] = "%{wks.location}/BunnyEngine/SDK/glm"
IncludeDir["stb_image"] = "%{wks.location}/BunnyEngine/SDK/stb_image"
IncludeDir["entt"] = "%{wks.location}/BunnyEngine/SDK/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/BunnyEngine/SDK/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/BunnyEngine/SDK/ImGuizmo"
IncludeDir["imgui_node_editor"] = "%{wks.location}/BunnyEngine/SDK/imgui-node-editor"
IncludeDir["assimp"] = "%{wks.location}/BunnyEngine/SDK/assimp/include"

group "Dependencise"
	include "BunnyEngine/SDK/GLFW"
	include "BunnyEngine/SDK/Glad"
	include "BunnyEngine/SDK/imgui"
	include "BunnyEngine/SDK/assimp"
	include "BunnyEngine/SDK/yaml-cpp"
	include "BunnyEngine/SDK/imgui-node-editor"
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

	buildoptions "/MTd"

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/SDK/stb_image/**.h",
		"%{prj.name}/SDK/stb_image/**.cpp",
		"%{prj.name}/SDK/glm/glm/**.hpp",
		"%{prj.name}/SDK/glm/glm/**.inl",
		"%{prj.name}/SDK/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/SDK/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.imgui_node_editor}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGuizmo}"
	}

	links{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"imgui-node-editor",
		"assimp",
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
		buildoptions "/MTd"

	filter "configurations:Release"
		defines "BE_RELEASE"
		runtime "Release"
		optimize "on"
		buildoptions "/MT"

	filter "configurations:Dist"
		defines "BE_DIST"
		runtime "Release"
		optimize "on"
		buildoptions "/MT"


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
		"%{IncludeDir.assimp}",
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