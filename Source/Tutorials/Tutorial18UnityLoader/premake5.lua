project "Tutorial-18_UnityLoader"
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
		"**.hpp",
		"**.cpp",
	}

	includedirs {
		"../../tga2dcore",
		"../TutorialCommon",
		"../../External",
		"source/**"
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