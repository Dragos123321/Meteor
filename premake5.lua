builddir = "build"
outputdir = (builddir .. "/bin/%{cfg.buildcfg}/%{prj.name}/%{cfg.system} %{cfg.architecture}")
interdir = (builddir .. "/bin-int/%{cfg.buildcfg}/%{prj.name}/%{cfg.system} %{cfg.architecture}")

workspace "Meteor"
	architecture "x64"
	startproject "Sandbox"
	configurations { "Debug", "Release" }

	defines {
		"GLFW_INCLUDE_NONE"
	}

	filename "Meteor"
	location (builddir)
	
	filter "configurations:Debug"
		optimize "off"
		
	filter "configurations:Release"
		optimize "on"

project "Meteor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	libdirs "Libraries"
	targetdir(outputdir)
	objdir(interdir)
	
	links {
		"assimp.lib",
		"glfw3.lib",
		"OpenAL32.lib",
		"opengl32.lib"
	}
	
	files {
		"meteor/include/meteor/**.h",
		"meteor/src/meteor/**.cpp",
		"meteor/src/vendor/**.c",
		"meteor/src/vendor/**.cc",
		"meteor/src/vendor/**.cpp"
	}
	includedirs { 
		"meteor/include",
		"meteor/include/vendor"
	}
	
	vpaths {
		["Headers/Audio"] = "meteor/include/meteor/audio/*.h",
		["Headers/Core"] = "meteor/include/meteor/core/*.h",
		["Headers/Graphics"] = "meteor/include/meteor/graphics/*.h",
		["Headers/Graphics/OpenGL"] = "meteor/include/meteor/graphics/opengl/*.h",
		["Headers/Graphics/Renderers"] = "meteor/include/meteor/graphics/renderers/*.h",
		["Headers/Gui"] = "meteor/include/meteor/gui/*.h",
		["Headers/Input"] = "meteor/include/meteor/input/*.h",
		["Headers/Math"] = "meteor/include/meteor/math/*.h",
		
		["Sources/Audio"] = "meteor/src/meteor/audio/*.cpp",
		["Sources/Core"] = "meteor/src/meteor/core/*.cpp",
		["Sources/Graphics"] = "meteor/src/meteor/graphics/*.cpp",
		["Sources/Graphics/OpenGL"] = "meteor/src/meteor/graphics/opengl/*.cpp",
		["Sources/Graphics/Renderers"] = "meteor/src/meteor/graphics/renderers/*.cpp",
		["Sources/Gui"] = "meteor/src/meteor/gui/*.cpp",
		["Sources/Input"] = "meteor/src/meteor/input/*.cpp",
		["Sources/Math"] = "meteor/src/meteor/math/*.cpp",
		
		["_Vendor"] = {
			"meteor/src/vendor/**.c",
			"meteor/src/vendor/**.cc",
			"meteor/src/vendor/**.cpp"
		}
	}
	
project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir(outputdir)
	objdir(interdir)
	
	links "Meteor"
	
	files {
		"sandbox/src/**.h",
		"sandbox/src/**.cpp"
	}
	includedirs {
		"meteor/include",
		"meteor/include/vendor",
		"sandbox/src"
	}
	
	vpaths {
		["Headers"] = "sandbox/src/**.h",
		["Sources"] = "sandbox/src/**.cpp"
	}
	
project "BrickBreaker"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir(outputdir)
	objdir(interdir)
	
	links "Meteor"
	
	files {
		"BrickBreaker/src/**.h",
		"BrickBreaker/src/**.cpp"
	}
	includedirs {
		"meteor/include",
		"meteor/include/vendor",
		"BrickBreaker/src"
	}
	
	vpaths {
		["Headers"] = "BrickBreaker/src/**.h",
		["Sources"] = "BrickBreaker/src/**.cpp"
	}
	
project "Fractals"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir(outputdir)
	objdir(interdir)
	
	links "Meteor"
	
	files {
		"Fractals/src/**.h",
		"Fractals/src/**.cpp"
	}
	includedirs {
		"meteor/include",
		"meteor/include/vendor",
		"Fractals/src"
	}
	
	vpaths {
		["Headers"] = "Fractals/src/**.h",
		["Sources"] = "Fractals/src/**.cpp"
	}
	
project "HyperJump"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir(outputdir)
	objdir(interdir)
	
	links "Meteor"
	
	files {
		"HyperJump/src/**.h",
		"HyperJump/src/**.cpp"
	}
	includedirs {
		"meteor/include",
		"meteor/include/vendor",
		"HyperJump/src"
	}
	
	vpaths {
		["Headers"] = "HyperJump/src/**.h",
		["Sources"] = "HyperJump/src/**.cpp"
	}
	
project "Chess"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir(outputdir)
	objdir(interdir)
	
	links "Meteor"
	
	files {
		"Chess/src/**.h",
		"Chess/src/**.cpp"
	}
	includedirs {
		"meteor/include",
		"meteor/include/vendor",
		"Chess/src"
	}
	
	vpaths {
		["Headers"] = "Chess/src/**.h",
		["Sources"] = "Chess/src/**.cpp"
	}