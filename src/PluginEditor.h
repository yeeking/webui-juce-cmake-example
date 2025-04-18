#pragma once

#include "PluginProcessor.h"
// #include <juce_gui_extra/juce_gui_extra.h> // Required for WebBrowserComponent
#include <JuceHeader.h>

//==============================================================================
class PluginEditor final : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void updateUIFromProcessor(const juce::String& msg);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    
    juce::WebBrowserComponent webView;

    // JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
