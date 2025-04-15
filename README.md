# Minimal web interface / JUCE / CMake example project

This repo shows you a very simple example of how to make a JUCE plugin which uses a web technology interface instead of the standard JUCE UI library. 

The plugin includes an embedded web server which makes it easy to test the end points and such. 

The web interface code is in src/ui/index.html. It gets added to the plugin as a binary data blob. 

To build: 

```
git clone git@github.com:yeeking/webui-juce-cmake-example.git
cd libs 
# i find all this sub module jazz too complex so just do this: 
git clone git@github.com:juce-framework/JUCE.git
cd ..
cmake -B build .
cmake --build build --config Debug -j 12 # optionally set number of parallel processes with -j 
```

If you want to do rapid development of your UI, having the index.html compiled into the binary data blob can prove a bit slow. Instead, you can work on the standalone app and have it serve the index.html UI file from the file system. Have a look in HTTPServer.cpp - there is some commented out code that serves from the file system instead. 

The embedded web server is the header-only cpp-httplib by Yuji Hirose from here: https://github.com/yhirose/cpp-httplib

# TODO

* [DONE] web interface  -> cpp comms
* [DONE] cpp -> web interface comms
* test on mac and windows
