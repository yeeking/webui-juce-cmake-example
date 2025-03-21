#pragma once 

// #include "httplib.hpp"
#include "../libs/httplib.h"
#include <JuceHeader.h>
#include "Utils.h"

// #include <juce_core/juce_core.h>

class HttpServerThread : public juce::Thread {
public:
    HttpServerThread();
 
    void run() override;

    void stopServer();
private:
    httplib::Server svr;

    
};