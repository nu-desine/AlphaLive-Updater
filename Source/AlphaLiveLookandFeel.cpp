/*
 *  AlphaLiveLookandFeel.cpp
 *  AlphaLive
 *
 *  Created by Sam Davies on 12/06/2012.
 *  Copyright 2012 nu desine. All rights reserved.
 *
 */

#include "AlphaLiveLookandFeel.h"

namespace LookAndFeelHelpers
{
    static void createRoundedPath (Path& p,
                                   const float x, const float y,
                                   const float w, const float h,
                                   const float cs,
                                   const bool curveTopLeft, const bool curveTopRight,
                                   const bool curveBottomLeft, const bool curveBottomRight) noexcept
    {
        const float cs2 = 2.0f * cs;
		
        if (curveTopLeft)
        {
            p.startNewSubPath (x, y + cs);
            p.addArc (x, y, cs2, cs2, float_Pi * 1.5f, float_Pi * 2.0f);
        }
        else
        {
            p.startNewSubPath (x, y);
        }
		
        if (curveTopRight)
        {
            p.lineTo (x + w - cs, y);
            p.addArc (x + w - cs2, y, cs2, cs2, 0.0f, float_Pi * 0.5f);
        }
        else
        {
            p.lineTo (x + w, y);
        }
		
        if (curveBottomRight)
        {
            p.lineTo (x + w, y + h - cs);
            p.addArc (x + w - cs2, y + h - cs2, cs2, cs2, float_Pi * 0.5f, float_Pi);
        }
        else
        {
            p.lineTo (x + w, y + h);
        }
		
        if (curveBottomLeft)
        {
            p.lineTo (x + cs, y + h);
            p.addArc (x, y + h - cs2, cs2, cs2, float_Pi, float_Pi * 1.5f);
        }
        else
        {
            p.lineTo (x, y + h);
        }
		
        p.closeSubPath();
    }
	
    static Colour createBaseColour (const Colour& buttonColour,
                                    const bool hasKeyboardFocus,
                                    const bool isMouseOverButton,
                                    const bool isButtonDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour (buttonColour.withMultipliedSaturation (sat));
		
        if (isButtonDown)
            return baseColour.contrasting (0.2f);
        else if (isMouseOverButton)
            return baseColour.contrasting (0.1f);
		
        return baseColour;
    }
	
    static TextLayout layoutTooltipText (const String& text, const Colour& colour) noexcept
    {
        const float tooltipFontSize = 13.0f;
        const int maxToolTipWidth = 400;
		
        AttributedString s;
        s.setJustification (Justification::centred);
        s.append (text, Font (tooltipFontSize, Font::bold), colour);
		
        TextLayout tl;
        tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
        return tl;
    }
}

AlphaLiveLookandFeel::AlphaLiveLookandFeel()

{
    
    jassert (Colours::white == Colour (0xffffffff));
	
    // set up the standard set of colours..
    const uint32 textButtonColour      = 0xff000000;
    const uint32 textHighlightColour   = 0xffcccccc;
    const uint32 standardOutlineColour = 0xff3c3c3c;
	
    static const uint32 standardColours[] =
    {
        TextButton::buttonColourId,                 textButtonColour,
        TextButton::buttonOnColourId,               0xff1a56ab,
        TextButton::textColourOnId,                 0xffffffff,
        TextButton::textColourOffId,                0xffffffff,
        
        DrawableButton::backgroundColourId,         0x00000000,
        DrawableButton::backgroundOnColourId,       AlphaColours::blue_,

		
        ComboBox::buttonColourId,                   0xffcccccc,
        ComboBox::outlineColourId,                  standardOutlineColour,
		
        ToggleButton::textColourId,                 0xffffffff,
		
        TextEditor::backgroundColourId,             0x00000000,
        TextEditor::textColourId,                   0xffffffff,
        TextEditor::highlightColourId,              AlphaColours::blue_,
        TextEditor::highlightedTextColourId,        0xffffffff,
        TextEditor::outlineColourId,                0x00000000,
        TextEditor::focusedOutlineColourId,         textButtonColour,
        TextEditor::shadowColourId,                 0x38000000,
		
        CaretComponent::caretColourId,              0xff000000,
		
        Label::backgroundColourId,                  0x00000000,
        Label::textColourId,                        0xffffffff,
        Label::outlineColourId,                     0x00000000,
		
        ScrollBar::backgroundColourId,              0x00000000,
        ScrollBar::thumbColourId,                   0xffffffff,
		
        TreeView::linesColourId,                    0x4c000000,
        TreeView::backgroundColourId,               0xff000000,
        TreeView::dragAndDropIndicatorColourId,     0x80ff0000,
		
        PopupMenu::backgroundColourId,              0x00000000,
        PopupMenu::textColourId,                    0xffffffff,
        PopupMenu::headerTextColourId,              0xff000000,
        PopupMenu::highlightedTextColourId,         0xffffffff,
        PopupMenu::highlightedBackgroundColourId,   AlphaColours::blue_,
		
        ComboBox::textColourId,                     0xffffffff,
        ComboBox::backgroundColourId,               0x0fcccccc,
        ComboBox::arrowColourId,                    0xffcccccc,
		
        ListBox::backgroundColourId,                0xff000000,
        ListBox::outlineColourId,                   standardOutlineColour,
        ListBox::textColourId,                      0xffffffff,
		
        Slider::backgroundColourId,                 0x00000000,
        Slider::thumbColourId,                      textButtonColour,
        Slider::trackColourId,                      AlphaColours::blue_,
        Slider::rotarySliderFillColourId,           AlphaColours::blue_,
        Slider::rotarySliderOutlineColourId,        0xff2c2c2c, //0xff3c3c3c
        Slider::textBoxTextColourId,                0xffffffff,
        Slider::textBoxBackgroundColourId,          0x00000000,
        Slider::textBoxHighlightColourId,           textHighlightColour,
        Slider::textBoxOutlineColourId,             standardOutlineColour,
		
        ResizableWindow::backgroundColourId,        0xff777777,
        //DocumentWindow::textColourId,               0xff000000, // (this is deliberately not set)
		
        AlertWindow::backgroundColourId,            0xffededed,
        AlertWindow::textColourId,                  0xff000000,
        AlertWindow::outlineColourId,               0xff666666,
		
        ProgressBar::backgroundColourId,            0xffeeeeee,
        ProgressBar::foregroundColourId,            0xffaaaaee,
		
        TooltipWindow::backgroundColourId,          0x00000000,
        TooltipWindow::textColourId,                0xffffffff,
        TooltipWindow::outlineColourId,             0x4c000000,
		
        TabbedComponent::backgroundColourId,        0x00000000,
        TabbedComponent::outlineColourId,           0xff000000,
        TabbedButtonBar::tabOutlineColourId,        0x80000000,
        TabbedButtonBar::frontOutlineColourId,      0x90000000,
		
        Toolbar::backgroundColourId,                0xfff6f8f9,
        Toolbar::separatorColourId,                 0x4c000000,
        Toolbar::buttonMouseOverBackgroundColourId, 0x4c0000ff,
        Toolbar::buttonMouseDownBackgroundColourId, 0x800000ff,
        Toolbar::labelTextColourId,                 0xf0000000,
        Toolbar::editingModeOutlineColourId,        0xffff0000,
		
        HyperlinkButton::textColourId,              0xcc1111ee,
		
        GroupComponent::outlineColourId,            0x66000000,
        GroupComponent::textColourId,               0xffffffff,
		
        DirectoryContentsDisplayComponent::highlightColourId,   AlphaColours::blue_, // <-- used for fileList and fileTree item's
        DirectoryContentsDisplayComponent::textColourId,        0xffffffff,
		
        0x1000440, /*LassoComponent::lassoFillColourId*/        0x66dddddd,
        0x1000441, /*LassoComponent::lassoOutlineColourId*/     0x99111111,
		
        0x1005000, /*MidiKeyboardComponent::whiteNoteColourId*/               0xffffffff,
        0x1005001, /*MidiKeyboardComponent::blackNoteColourId*/               0xff000000,
        0x1005002, /*MidiKeyboardComponent::keySeparatorLineColourId*/        0x66000000,
        0x1005003, /*MidiKeyboardComponent::mouseOverKeyOverlayColourId*/     0x80ffff00,
        0x1005004, /*MidiKeyboardComponent::keyDownOverlayColourId*/          0xffb6b600,
        0x1005005, /*MidiKeyboardComponent::textLabelColourId*/               0xff000000,
        0x1005006, /*MidiKeyboardComponent::upDownButtonBackgroundColourId*/  0xffd3d3d3,
        0x1005007, /*MidiKeyboardComponent::upDownButtonArrowColourId*/       0xff000000,
		
        0x1004500, /*CodeEditorComponent::backgroundColourId*/                0xffffffff,
        0x1004502, /*CodeEditorComponent::highlightColourId*/                 textHighlightColour,
        0x1004503, /*CodeEditorComponent::defaultTextColourId*/               0xffffffff,
		
        0x1007000, /*ColourSelector::backgroundColourId*/                     0xff000000,
        0x1007001, /*ColourSelector::labelTextColourId*/                      0xffffffff,
		
        0x100ad00, /*KeyMappingEditorComponent::backgroundColourId*/          0x00000000,
        0x100ad01, /*KeyMappingEditorComponent::textColourId*/                0xff000000,
		
        FileSearchPathListComponent::backgroundColourId,        0x00000000,
		
        FileChooserDialogBox::titleTextColourId,                0xffffffff,
		
        DrawableButton::textColourId,                           0xffffffff,
    };
	
    for (int i = 0; i < numElementsInArray (standardColours); i += 2)
        setColour (standardColours [i], Colour ((uint32) standardColours [i + 1]));
	
}

AlphaLiveLookandFeel::~AlphaLiveLookandFeel()
{
   
}

void AlphaLiveLookandFeel::drawButtonBackground (Graphics& g,
                                        Button& button,
                                        const Colour& backgroundColour,
                                        bool isMouseOverButton,
                                        bool isButtonDown)
{
    
	switch (button.getToggleState() ? (isButtonDown ? 5 : (isMouseOverButton ? 4 : 3))
			: (isButtonDown ? 2 : (isMouseOverButton ? 1 : 0)))
    {
		case 0:
        {
            g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.6), Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
            break;
        }
			
		case 1:
        {
            g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
            ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.9), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
			
            break;
        }
			
		case 2:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.4), backgroundColour, (button.getWidth()*0.5), 0, false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
			
            break;
        }
			
		case 3:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.7), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
			
			
            break;
        }
			
		case 4:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.9), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));			
			
            break;
        }
			
		case 5:
        {
			g.setColour(backgroundColour);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient fillGradient(Colours::white.withAlpha(0.1f), (button.getWidth()*0.5), (button.getHeight()*0.8), backgroundColour, (button.getWidth()*0.5), (button.getHeight()*0.3), false);
			g.setGradientFill(fillGradient);
			g.fillEllipse(0, 0, button.getWidth(), button.getHeight());
			
			ColourGradient highlightGradient(Colours::transparentWhite, (button.getWidth()*0.5), (button.getHeight()*0.8), Colours::silver.withAlpha(0.3f), (button.getWidth()*0.5), 0, false);
			g.setGradientFill(highlightGradient);
			g.drawEllipse((button.getWidth()* 0.03), (button.getHeight()* 0.03), (button.getWidth()*0.94), (button.getHeight()*0.94), (button.getHeight()*0.025));
			
            break;
        }
			
		default:
			break;
    }
	
}

Font AlphaLiveLookandFeel::getTextButtonFont (TextButton& button)
{
    return button.getFont();
}

void AlphaLiveLookandFeel::drawButtonText (Graphics& g, TextButton& button,
                                  bool isMouseOverButton, bool isButtonDown)
{
    Font font (getTextButtonFont (button));
    g.setFont (10);
    g.setColour (button.findColour (button.getToggleState() ? TextButton::textColourOnId
									: TextButton::textColourOffId)
				 .withMultipliedAlpha (button.isEnabled() ? 1.0f : 0.5f));
	
    const int yIndent = jmin (4, button.proportionOfHeight (0.3f));
    const int cornerSize = jmin (button.getHeight(), button.getWidth()) / 2;
	
    const int fontHeight = roundToInt (font.getHeight() * 0.3f);
    const int leftIndent  = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
    const int rightIndent = jmin (fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
	
    g.drawFittedText (button.getButtonText(),
                      leftIndent,
                      yIndent,
                      button.getWidth() - leftIndent - rightIndent,
                      button.getHeight() - yIndent * 2,
                      Justification::centred, 2);
}




