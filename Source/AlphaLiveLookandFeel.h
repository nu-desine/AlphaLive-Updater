/*
 *  AlphaLiveLookandFeel.h
 *  AlphaLive
 *
 *  Created by Sam Davies on 12/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
 *
 */


#ifndef ALPHALIVELOOKANDFEEL_H
#define ALPHALIVELOOKANDFEEL_H

#include "../JuceLibraryCode/JuceHeader.h"

namespace AlphaColours
{
    //Create a uint32 and a Juce::Colour of any 
    //custom colours for the application here.
    
    //colours as uint32
    const uint32 blue_ = 0xff1a54ab;
    const uint32 lightblue_ = 0xff3c76c5;
	const uint32 nearlyblack_ = 0xff181818;
	const uint32 verydarkgrey_ = 0xff202020;
    
    //colours as Juce::Colour
    static Colour blue(blue_);
    static Colour lightblue(lightblue_);
	static Colour nearlyblack(nearlyblack_);
	static Colour verydarkgrey(verydarkgrey_);
    
}

class AlphaLiveLookandFeel : public LookAndFeel


{
public:
	
	AlphaLiveLookandFeel();
	
	~AlphaLiveLookandFeel();
	
	void drawButtonBackground (Graphics& g,
                                       Button& button,
                                       const Colour& backgroundColour,
                                       bool isMouseOverButton,
                                       bool isButtonDown);
	
	Font getTextButtonFont (TextButton& button);
	
	void drawButtonText (Graphics& g, TextButton& button,
						 bool isMouseOverButton, bool isButtonDown);
	
	
                                    
   	
private:
    //==============================================================================
   	
    Array <int> colourIds;
    Array <Colour> colours;
	
};    


#endif   // ALPHALIVELOOKANDFEEL_H
