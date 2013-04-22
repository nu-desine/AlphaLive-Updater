/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

//
//  MainComponent.h
//  AlphaLive Updater
//
//  Created by Liam Meredith-Lacey on 18/03/2013.
//  Copyright 2013 nu desine.
//
//  This file is part of AlphaLive Updater.
//
//  AlphaLive Updater is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2, 
//  as published by the Free Software Foundation.
//  
//  AlphaLive Updater is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

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
    
    void setLocalisation();

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
    
    LocalisedStrings *trans;
    String currentLanguage;
};


#endif  // __MAINCOMPONENT_H_ED384941__
