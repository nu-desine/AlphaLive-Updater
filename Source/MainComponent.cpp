/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "BinaryData.h"

//Dimensions for centre text box
#define BOX_X getWidth()/10
#define BOX_Y getHeight()/10
#define BOX_W (getWidth()/10)*8
#define BOX_H (getHeight()/10)*6



//==============================================================================
MainContentComponent::MainContentComponent(JUCEApplication *juceApplication_) 
                                        :   Thread ("installerThread"),
                                            juceApplication (juceApplication_)
{
    //load binary data into Image
    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    
    addAndMakeVisible (infoLabel = new Label());
    infoLabel->setJustificationType(Justification::topLeft);
    Font newFont(12);
    infoLabel->setFont (newFont);
    infoLabel->setText("\n\n" +
                       translate("This application has been designed to be launched directly from AlphaLive. To update AlphaLive, close this application and in AlphaLive go to \"Help -> Update Software\".") + 
                       "\n\n" + 
                       translate("If you would like to update AlphaLive but this computer does not have an internet connection, follow these steps:") + 
                       "\n" +  
                       translate("- On a networked computer, manually download the update from http://www.alphasphere.com/AlphaLive_update.") +
                       "\n" + 
                       translate("- Unzip the downloaded folder.") +
                       "\n" +
                       translate("- Move it to the AlphaLive directory in \"Applications\" on OS X or \"Program Files\" on Windows on this computer.") +
                       "\n" +
                       translate("- If the download contains a version of this \"AlphaLive Updater\" application in either \"Mac Files\", \"Win32 Files\" or \"Win64 Files\", close this application, move the relevant version to \"AlphaLive/Application Data\" to replace the current version, and relaunch this application."), 
                       dontSendNotification);
    
    addAndMakeVisible (closeButton = new TextButton(translate("Close")));
    closeButton->addListener(this);
    closeButton->setColour(TextButton::buttonColourId, AlphaColours::verydarkgrey.withAlpha(0.7f));
    
    addChildComponent (cancelButton = new TextButton(translate("Cancel")));
    cancelButton->addListener(this);
    cancelButton->setColour(TextButton::buttonColourId, AlphaColours::verydarkgrey.withAlpha(0.7f));
    
    progress = 0;
    addChildComponent (progressBar = new ProgressBar (progress));
    progressBar->setColour (ProgressBar::backgroundColourId, AlphaColours::verydarkgrey.withAlpha(1.0f));
    progressBar->setColour (ProgressBar::foregroundColourId, Colours::grey);
    
    setSize (backgroundImage.getWidth(), backgroundImage.getHeight());
    
    //Look-and-feel stuff
    LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);
    LookAndFeel::getDefaultLookAndFeel().setUsingNativeAlertWindows(true);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.drawImage(backgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, backgroundImage.getWidth(), backgroundImage.getHeight());
    
    g.setColour(Colours::darkgrey.withAlpha(0.7f));
    g.fillRoundedRectangle(BOX_X, BOX_Y, BOX_W, BOX_H, 10);
    
    g.setColour(Colours::grey.withAlpha(0.6f));
    int border = 5;
    g.fillRoundedRectangle(BOX_X+border, 
                           BOX_Y+border, 
                           BOX_W-(border*2), 
                           BOX_H-(border*2), 
                           10);
}

void MainContentComponent::resized()
{
    infoLabel->setBounds(BOX_X+5, BOX_Y+5, BOX_W-10, BOX_H-10);
    
    closeButton->setBounds((BOX_X + BOX_W) - 55,
                           (BOX_Y + BOX_H) - 55,
                           45,
                           45);
    cancelButton->setBounds((BOX_X + BOX_W) - 55,
                            (BOX_Y + BOX_H) - 55,
                            45,
                            45);
    
    progressBar->setBounds((getWidth()/2)-75, (BOX_Y + BOX_H) - 40, 150, 20);
}

void MainContentComponent::buttonClicked (Button *button)
{
    if (button == closeButton)
    {
        juceApplication->quit();
    }
    else if (button == cancelButton)
    {
        bool userSelection = AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, 
                                                          translate("Are you sure?"), 
                                                          translate("Are you sure you want to cancel? The update may not be fully applied if so and may cause problems when running AlphaLive."));
        if (userSelection == true)
        {
            signalThreadShouldExit();
        }
    }
}

void MainContentComponent::run()
{
    // Do the following in a seperate thread so the GUI can be updated correctly.
    
    {
        const MessageManagerLock mmLock;
        closeButton->setVisible(false);
        cancelButton->setVisible(true);
        
        progressBar->setVisible(true);
    }
    
    bool installationCancelled = false;
    
    while (! threadShouldExit())
    {
        
        signalThreadShouldExit();
    }
    
    if (installationCancelled == true)
    {
        const MessageManagerLock mmLock;
        infoLabel->setText(translate("Installation cancelled!"), dontSendNotification);
        
        closeButton->setVisible(true);
        cancelButton->setVisible(false);
        progressBar->setVisible(false);
    }
    else
    {
        const MessageManagerLock mmLock;
        infoLabel->setText(translate("Installation complete!"), dontSendNotification);
        
        closeButton->setVisible(true);
        cancelButton->setVisible(false);
        progressBar->setVisible(false);
        
    }
}
