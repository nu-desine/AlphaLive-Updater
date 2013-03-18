/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef __MAINCOMPONENT_H_ED384941__
#define __MAINCOMPONENT_H_ED384941__

#include "../JuceLibraryCode/JuceHeader.h"
#include "AlphaLiveLookandFeel.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
                                public Button::Listener,
                                public Thread
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();
    
    void buttonClicked (Button *button);
    void run();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    AlphaLiveLookandFeel lookAndFeel;
    Image backgroundImage;
    
    ScopedPointer <Label> infoLabel;
    ScopedPointer <TextButton> closeButton, cancelButton;
    
    ScopedPointer <ProgressBar> progressBar;
    double progress;
    
    File alphaLiveDirectory, updateDirectory;
};


#endif  // __MAINCOMPONENT_H_ED384941__
