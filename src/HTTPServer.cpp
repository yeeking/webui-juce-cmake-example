#include "HTTPServer.h"
#include "PluginProcessor.h"

HttpServerThread::HttpServerThread(PluginProcessor& _pluginProc)  : 
   juce::Thread("HTTP Server Thread"), 
   pluginProc{_pluginProc}
{
    initAPI();
}


void HttpServerThread::initAPI()
{
    svr.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        DBG("HttpServerThread::log " << req.method << " " << req.path 
                  << " -> " << res.status);
    });

    for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
    {
        DBG("Got binary file " << BinaryData::originalFilenames[i]);

    }
    // route for the main index file
    svr.Get("/index.html", [](const httplib::Request& req, httplib::Response& res) {
        int size = 0;
        const char* data = BinaryData::getNamedResource(BinaryData::namedResourceList[0], size);
    
        if (data != nullptr) {
            res.set_content(data, static_cast<size_t>(size), "text/html");
        } else {
            res.status = 404;
            res.set_content("404: File not found", "text/plain");
        }
    });

    ///** start of server from file system code 
    // this code can be used in the standalone app 
    // to serve files from its 'ui' subdirectory
    // for easy testing where you can edit the 
    // std::string workingDir = getBinary().string();
    // std::cout << "Serving from " << workingDir << "/../ui" << std::endl;
    // auto ret = svr.set_mount_point("/", workingDir + "/../ui");
    ///**  end of server from file system code 

    // 'live' responders for button presses - call to the pluginprocessor 
    svr.Get("/button1", [this](const httplib::Request& req, httplib::Response& res) {
        this->pluginProc.messageReceivedFromWebAPI("Button 1 clicked");
    });
    svr.Get("/button2", [this](const httplib::Request& req, httplib::Response& res) {
        this->pluginProc.messageReceivedFromWebAPI("Button 2 clicked");
    });

    

}


void HttpServerThread::run()
{

    DBG("API server starting");

    // Run the server in a blocking loop until stopThread() is called
    svr.listen("0.0.0.0", 8080);

}

void HttpServerThread::stopServer()
{
    DBG("API server shutting down");

    svr.stop();
    stopThread(1000); // Gracefully stop thread
}
    
