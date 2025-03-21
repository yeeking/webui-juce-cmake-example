#include "HTTPServer.h"

HttpServerThread::HttpServerThread()  : juce::Thread("HTTP Server Thread")

{
 
    svr.set_logger([](const httplib::Request& req, const httplib::Response& res) {
        std::cout << req.method << " " << req.path 
                  << " -> " << res.status << std::endl;
    });


    for (int i = 0; i < BinaryData::namedResourceListSize; ++i)
    {
        std::cout << "Got binary file " << BinaryData::originalFilenames[i] << std::endl;

    }
    svr.Get("/index.html", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << "processing / " << std::endl;
        int size = 0;
        const char* data = BinaryData::getNamedResource(BinaryData::namedResourceList[0], size);
    
        if (data != nullptr) {
            res.set_content(data, size, "text/html");
        } else {
            res.status = 404;
            res.set_content("File not found", "text/plain");
        }
    });
    
    // this code can be used in the standalone app 
    // to serve files from its 'ui' subdirectory
    // for easy testing where you can edit the 
    // std::string workingDir = getBinary().string();
    // std::cout << "Serving from " << workingDir << "/../ui" << std::endl;
    // auto ret = svr.set_mount_point("/", workingDir + "/../ui");

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
    
