#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), 
    webView{
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
