#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), 
    webView{
        // no idea if this stuff is needed... but it looks useful
        juce::WebBrowserComponent::Options{}
        .withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
        .withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}
        .withBackgroundColour(juce::Colours::blue)
            // this may be necessary for some DAWs; include for safety
        .withUserDataFolder(juce::File::getSpecialLocation(
            juce::File::SpecialLocationType::tempDirectory)))
    }
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize (400, 500);

    // Load the webpage from the local server
    webView.goToURL("http://127.0.0.1:8080/index.html");  // Adjust port if needed

    // Add WebView component to the editor
    addAndMakeVisible(webView);
    
}

PluginEditor::~PluginEditor()
{
}

//==============================================================================
void PluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
}

void PluginEditor::resized()
{
    webView.setBounds(getLocalBounds()); // Make web view fill entire UI
}


void PluginEditor::updateUIFromProcessor(const juce::String& msg)
{  
    // create an object and convert it to a json string 
    juce::DynamicObject::Ptr obj = new juce::DynamicObject();
    obj->setProperty("msg", msg);
    juce::var data(obj);
    juce::String json = juce::JSON::toString(data);
    
    webView.evaluateJavascript("handleCppMessage(" + json + ");", nullptr);
    
    // std::string msg_as_json = "{\"msg\":\""+ msg + "\"}";
    // juce::JSON::toString({"msg":msg});
    // webView.evaluateJavascript("backendCall('"+msg_as_json+"')");
}
