@echo off

set "MAIN_CMAKE=.\CMakeLists.txt"

::Used for both Engine and Editor
set "BASE_SOURCES_FOLDER_NAME=Sources" 

set "EDITOR_CMAKE=.\TonicEditor\CMakeLists.txt"
set "EDITOR_SOURCES_FOLDER_PATH=.\TonicEditor\Sources"

set "ENGINE_CMAKE=.\TonicEngine\CMakeLists.txt"
set "ENGINE_SOURCES_FOLDER_PATH=.\TonicEngine\Sources"

::Check if the path are valid
if not exist "%EDITOR_SOURCES_FOLDER_PATH%" (
    echo Folder "%EDITOR_SOURCES_FOLDER_PATH%" does not exist.
    exit /b
)

if not exist "%ENGINE_SOURCES_FOLDER_PATH%" (
    echo Folder "%ENGINE_SOURCES_FOLDER_PATH%" does not exist.
    exit /b
)

::The main CMakeLists.txt it is the equivalent of Tonic.sln
(
    echo cmake_minimum_required^(VERSION 3.8^)
    echo project^(TonicEngine^)
    echo. 
	echo add_subdirectory^("TonicEngine"^)
	echo add_subdirectory^("TonicEditor"^)
) > "%MAIN_CMAKE%"

::The Editor CMakeLists.txt it is the equivalent of TonicEditor/TonicEditor.vcxproj
(
	echo cmake_minimum_required^(VERSION 3.8^)
	echo project^(TonicEditor^)
	echo.
	echo include_directories^("Headers"^)
	echo.
	echo set ^(SOURCES
	
	for /r "%EDITOR_SOURCES_FOLDER_PATH%" %%F in (*.cpp) do (
		set "FILE=%%F"
		setlocal enabledelayedexpansion
		set "FILE=!FILE:\=/!"
		echo 	"!FILE:*%BASE_SOURCES_FOLDER_NAME%=%BASE_SOURCES_FOLDER_NAME%!"
		endlocal
	)
	
	echo ^)
	echo.
	echo add_executable^(${PROJECT_NAME} ${SOURCES}^)
	echo.
	echo target_include_directories^(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/TonicEngine/Headers^)
	echo target_link_directories^(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/TonicEngine^)
	echo target_link_libraries^(${PROJECT_NAME} TonicEngine^)
) > "%EDITOR_CMAKE%"

::The Engine CMakeLists.txt it is the equivalent of TonicEngine/TonicEngine.vcxproj
(
	echo cmake_minimum_required^(VERSION 3.8^)
	echo project^(TonicEngine^)
	echo set ^(CMAKE_CXX_STANDARD 17^)
	echo.
	echo include_directories^("Headers/TonicEngine"^)
	echo.
	echo set^(PRECOMPILED_HEADER "Headers/TonicEngine/pch.hpp"^)
	echo.
	echo set ^(SOURCES
	
	for /r "%ENGINE_SOURCES_FOLDER_PATH%" %%F in (*.cpp) do (
        set "FILE=%%F"
        setlocal enabledelayedexpansion
		set "FILE=!FILE:\=/!"
        echo 	"!FILE:*%BASE_SOURCES_FOLDER_NAME%=%BASE_SOURCES_FOLDER_NAME%!"
        endlocal
    )
	
	echo ^)
	echo.
	echo add_compile_definitions^(TONICENGINE_EXPORTS^)
	echo.
	echo add_library^(${PROJECT_NAME} SHARED ${SOURCES}^)
	echo.
	echo target_precompile_headers^(${PROJECT_NAME} PRIVATE ${PRECOMPILED_HEADER}^)
	echo.
	echo # Configuring Target Library Properties for Windows
	echo if ^(WIN32^)
	echo 	set_target_properties^(${PROJECT_NAME} PROPERTIES
	echo 		WINDOWS_EXPORT_ALL_SYMBOLS ON
	echo 		STATIC_LIBRARY_PREFIX ""
	echo 		STATIC_LIBRARY_SUFFIX ".lib"
	echo 	^)
	echo # Configuring Target Library Properties for macOS
	echo elseif ^(APPLE^)
	echo 	set_target_properties^(${PROJECT_NAME} PROPERTIES
	echo 		MACOSX_RPATH ON
	echo 	^)
	echo # Configuring Target Library Properties for Linux
	echo else^(^)
	echo 	set_target_properties^(${PROJECT_NAME} PROPERTIES
	echo 		VERSION ${PROJECT_VERSION}
	echo 		SOVERSION ${PROJECT_VERSION_MAJOR}
	echo 	^)
	echo endif^(^)
) > "%ENGINE_CMAKE%"

echo CmakeFiles created with sucess.