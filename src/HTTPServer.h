#pragma once 

#include "../libs/httplib.h"
#include <JuceHeader.h>
#include "Utils.h" 

class PluginProcessor; // forward declaration to avoid circular include 

class HttpServerThread : public juce::Thread {
public:
    HttpServerThread() = delete;

    HttpServerThread(PluginProcessor& _pluginProc);
    void run() override;

    void stopServer();
private:

    void initAPI();
    PluginProcessor& pluginProc; 
    httplib::Server svr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HttpServerThread)

};