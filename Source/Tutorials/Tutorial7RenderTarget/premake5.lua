project "Tutorial-07_RenderTarget"
	location "%{wks.location}/Local/"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	--staticruntime "on"

	targetdir ("%{wks.location}/Bin/")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{wks.location}/Temp/%{prj.name}/%{cfg.buildcfg}")

	debugdir "%{wks.location}/Bin/"

	files {
		"**.h",
		"**.cpp",
		"**.hlsl",
		"**.hlsli",
	}

	includedirs {
		"../../tga2dcore",
		"../TutorialCommon",
		"source/"
	}

	libdirs { "%{wks.location}/Lib/" }

	links {
		"tga2dcore"
	}

	defines {"_CONSOLE"}
	
	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"

	systemversion "latest"
	
	filter "system:windows"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		-- sdlchecks "true"
		--conformanceMode "On"
		--buildoptions { "/STACK:8000000" }
		flags { 
		--	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS"
		}

	shadermodel("5.0")
	local shader_dir = "%{wks.location}/Bin/Tutorial7/Shaders/"
	os.mkdir(shader_dir)

	filter("files:**.hlsl")
		flags("ExcludeFromBuild")
		shaderobjectfileoutput(shader_dir.."%{file.basename}"..".cso")

	filter("files:**PS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Pixel")

	filter("files:**VS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Vertex")

	filter("files:**GS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Geometry")