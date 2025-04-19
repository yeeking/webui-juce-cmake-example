cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(webui-example VERSION 0.0.1)

message(STATUS "CMAKE_CXX_COMPILER before juce: ${CMAKE_CXX_COMPILER}")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
   pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.1)
endif()

add_subdirectory(libs/JUCE)                    # If you've put JUCE in a subdirectory called JUCE

juce_add_plugin(webui-example
  PLUGIN_MANUFACTURER_CODE Mjyk               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Dem0                            # A unique four-character plugin id with exactly one upper-case character
    COPY_PLUGIN_AFTER_BUILD TRUE                                           # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "webui-example"
    NEEDS_WEB_BROWSER TRUE
    NEEDS_WEBVIEW2 TRUE    )

juce_generate_juce_header(webui-example)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
target_include_directories(webui-example
    PRIVATE
      ${GTK3_INCLUDE_DIRS}  
      ${WEBKIT2_INCLUDE_DIRS}
)
endif()


target_sources(webui-example
    PRIVATE
        ./src/PluginEditor.cpp
        ./src/PluginProcessor.cpp
        ./src/HTTPServer.cpp
        )

        
target_compile_definitions(webui-example
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=1  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
)

juce_add_binary_data(AudioPluginData SOURCES src/ui/index.html)

target_link_libraries(webui-example
PRIVATE
    AudioPluginData           # If we'd created a binary data target, we'd link to it here
    juce::juce_audio_utils
    juce::juce_gui_extra  

PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

## copy the files for the ui from the src/ui folder into the built directory 
add_custom_command(
    TARGET webui-example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:webui-example>/ui"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/ui"
        "$<TARGET_FILE_DIR:webui-example>/ui"
    COMMENT "Copying UI files to output folder"
)


# set this to ON and the http server will serve the UI files from disk instead of memory
# which makes it easier to do quick UI iterations
set(LOCAL_WEBUI ON) 

if(LOCAL_WEBUI)
    target_compile_definitions(webui-example PRIVATE LOCAL_WEBUI)
    message(STATUS "Serving UI from disk: LOCAL_WEBUI is ${LOCAL_WEBUI}")
else()
    message(STATUS "Serving UI from memory: LOCAL_WEBUI is ${LOCAL_WEBUI}")   
endif()

