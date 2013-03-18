/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "BinaryData.h"

//#if JUCE_MAC || JUCE_LINUX
////#include <sys/types.h>
//#include <sys/stat.h>
//#endif

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
    // Load binary data into Image
    backgroundImage = ImageCache::getFromMemory(BinaryData::background_png, BinaryData::background_pngSize);
    
    addAndMakeVisible (infoLabel = new Label());
    
    alphaLiveDirectory = File::getSpecialLocation (File::currentApplicationFile).getParentDirectory().getParentDirectory();
    updateDirectory = alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive_update";
    
    // Make sure this application is being launch from the correct place
    #if JUCE_MAC || JUCE_LINUX
    File alphaLiveApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive.app");
    #endif
    #if JUCE_WINDOWS
    File alphaLiveApp (alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive.exe");
    #endif
    
    bool shouldUpdate;
    
    // If the app seems to be launched from the wrong location,
    // or the update directory doesn't exist, display instructions to the user
    if ((! alphaLiveApp.exists()) || (! updateDirectory.isDirectory()))
    {
        shouldUpdate = false;
        
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
                           translate("- If the download contains a version of this \"AlphaLive Updater\" application in either \"Mac Files\", \"Win32 Files\" or \"Win64 Files\", close this application, move the relevant version to \"AlphaLive/Application Data\" to replace the current version") +
                           "\n" +
                           translate("- Make sure AlphaLive is closed, and relaunch \"AlphaLive Updater\" from \"AlphaLive/Application Data\"."), 
                           dontSendNotification);
    }
    else
    {
        shouldUpdate = true;
        
        infoLabel->setJustificationType(Justification::centred);  
    }
    
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
    
    if (shouldUpdate)
    {
        startThread();
    }
}

MainContentComponent::~MainContentComponent()
{
    stopThread(1000);
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
        
        progress = -1;
        progressBar->setVisible(true);
        
        infoLabel->setText(translate("Starting update installation..."), dontSendNotification);
        
    }
    
    wait(1000);
    
    bool installationCancelled = false;
    
    while (! threadShouldExit())
    {
        //    #if JUCE_MAC
        //    //when uncompressed using zlib, the executable bit of any files have for some been removed,
        //    //meaning that any application files can't be opened, as found here:
        //    //http://www.rawmaterialsoftware.com/viewtopic.php?f=2&t=5727
        //    //To fix the file permissions, you can use the command chmod 775 in terminal
        //    //or use th chmod() function as documented here:
        //    //http://www.manpagez.com/man/2/chmod/osx-10.4.php
        //    
        //    File exe1 (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/firmwareUpdater");
        //    File exe2 (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/AlphaLive.app/Contents/MacOS/AlphaLive");
        //    
        //    chmod (exe1.getFullPathName().toUTF8(), S_IRWXO | S_IRWXU | S_IRWXG);
        //    chmod (exe2.getFullPathName().toUTF8(), S_IRWXO | S_IRWXU | S_IRWXG);
        //    #endif 
        //    
        //    //what about on windows?
        
        
        Array<File> filesToCopy;
        File applicatDataDir (updateDirectory.getFullPathName() + File::separatorString + "Application Data");
        File libraryDir (updateDirectory.getFullPathName() + File::separatorString + "Library");
        File documentationDir (updateDirectory.getFullPathName() + File::separatorString + "Documentation");
        
        applicatDataDir.findChildFiles(filesToCopy, 2, true);
        libraryDir.findChildFiles(filesToCopy, 2, true);
        documentationDir.findChildFiles(filesToCopy, 2, true);
        
        int totalSize = 0;
        int extractedSize = 0;
        
        // === Work out the total size of the files being copied,
        // so we can work out a percentage for the progress bar ===
        
        for (int i = 0; i < filesToCopy.size(); i++)
            totalSize += filesToCopy[i].getSize();
        
        #if JUCE_MAC
        File newAppFile (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/AlphaLive.app");
        
        Array<File> allFiles;
        newAppFile.findChildFiles(allFiles, 3, true);
        for (int i = 0; i < allFiles.size(); i++)
            totalSize += allFiles[i].getSize();
        allFiles.clear();
        
        File newFirmwareFile (updateDirectory.getFullPathName() + File::separatorString + "Mac Files/firmwareUpdater");
        if (newFirmwareFile.exists())
            totalSize += newFirmwareFile.getSize();

        #endif 
        
        #if JUCE_WINDOWS
        if (SystemStats::isOperatingSystem64Bit())
        {
            File newAppFile (updateDirectory.getFullPathName() + File::separatorString + "Win64 Files/AlphaLive.exe");
            totalSize += newAppFile.getSize();
            File newFirmwareFile (updateDirectory.getFullPathName() + File::separatorString + "Win64 Files/firmwareUpdater.exe");
            if (newFirmwareFile.exists())
                totalSize += newFirmwareFile.getSize();
        }
        else
        {
            File newAppFile (updateDirectory.getFullPathName() + File::separatorString + "Win32 Files/AlphaLive.exe");
            totalSize += newAppFile.getSize();
            File newFirmwareFile (updateDirectory.getFullPathName() + File::separatorString + "Win32 Files/firmwareUpdater.exe");
            if (newFirmwareFile.exists())
                totalSize += newFirmwareFile.getSize();
        }
        #endif
        
        
        //==== move new files into place, replacing the older ones but not removing any unreplaced files ====
        
        // For the non platform-specific files, they should be organised in the same folder structure
        // Therefore we can just use relative paths
        
        for (int i = 0; i < filesToCopy.size(); i++)
        {
            File oldFile (alphaLiveDirectory.getFullPathName() + 
                          File::separatorString + 
                          filesToCopy[i].getRelativePathFrom(updateDirectory));
            
            bool doesParentExist = oldFile.getParentDirectory().isDirectory();
            
            if (doesParentExist == false)
            {
                File parentDir (oldFile.getParentDirectory());
                parentDir.createDirectory();
            }
            
            oldFile.deleteRecursively();
            
            // Increase the extracted size to we can work out the current progress bar value
            extractedSize += filesToCopy[i].getSize();
            // Get progress to a value between 0 and 1 (NOT 0 and 100) to update the progress bar correctly
            progress = ((extractedSize * 0.0001)/(totalSize * 0.0001));
            {
                const MessageManagerLock mmLock;
                String string (translate("Extracting files...") + "\n" + filesToCopy[i].getFileName());
                infoLabel->setText(string, dontSendNotification);
            }
            
            if (threadShouldExit() == true)
            {
                installationCancelled = true;
                break;
            }
            
            filesToCopy[i].copyFileTo(oldFile);
    
        }
        
        if (threadShouldExit() == true)
        {
            installationCancelled = true;
            break;
        }
        
        
        //platform specific files - can't use relative paths here
        #if JUCE_MAC
        File oldAppFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive.app");
        oldAppFile.deleteRecursively();
        
        newAppFile.findChildFiles(allFiles, 3, true);
        for (int i = 0; i < allFiles.size(); i++)
        {
            // Increase the extracted size to we can work out the current progress bar value
            extractedSize += allFiles[i].getSize();
            // Get progress to a value between 0 and 1 (NOT 0 and 100) to update the progress bar correctly
            progress = (extractedSize * 0.0001)/(totalSize * 0.0001);
            //sleep the thread here so the progress bar GUI is updated in a noticable manner
            wait(2);
        }
        {
            const MessageManagerLock mmLock;
            String string (translate("Extracting files...") + "\n" + newAppFile.getFileName());
            infoLabel->setText(string, dontSendNotification);
        }
        
        if (threadShouldExit() == true)
        {
            installationCancelled = true;
            break;
        }
        
        newAppFile.copyFileTo (oldAppFile);
        
        if (newFirmwareFile.exists())
        {
            File oldFirmwareFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/firmwareUpdater");
            oldFirmwareFile.deleteRecursively();
            
            // Increase the extracted size to we can work out the current progress bar value
            extractedSize += newFirmwareFile.getSize();
            // Get progress to a value between 0 and 1 (NOT 0 and 100) to update the progress bar correctly
            progress = ((extractedSize * 0.0001)/(totalSize * 0.0001));
            {
                const MessageManagerLock mmLock;
                String string (translate("Extracting files...") + "\n" + newFirmwareFile.getFileName());
                infoLabel->setText(string, dontSendNotification);
            }
            
            newFirmwareFile.copyFileTo(oldFirmwareFile);
        }
        #endif 
        
        #if JUCE_WINDOWS
        if (SystemStats::isOperatingSystem64Bit())
        {
            File newAppFile (updateDirectory.getFullPathName() + File::separatorString + "Win64 Files/AlphaLive.exe");
            File oldAppFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive.exe");
            oldAppFile.deleteRecursively();
            
            {
                const MessageManagerLock mmLock;
                String string (translate("Extracting files...") + "\n" + newAppFile.getFileName());
                infoLabel->setText(string, dontSendNotification);
            }
            
            if (threadShouldExit() == true)
            {
                installationCancelled = true;
                break;
            }
            
            newAppFile.copyFileTo (oldAppFile);
            
            File newFirmwareFile (updateDirectory.getFullPathName() + File::separatorString + "Win64 Files/firmwareUpdater.exe");
            if (newFirmwareFile.exists())
            {
                File oldFirmwareFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/firmwareUpdater.exe");
                oldFirmwareFile.deleteRecursively();
                
                // Increase the extracted size to we can work out the current progress bar value
                extractedSize += newFirmwareFile.getSize();
                // Get progress to a value between 0 and 1 (NOT 0 and 100) to update the progress bar correctly
                progress = ((extractedSize * 0.0001)/(totalSize * 0.0001));
                {
                    const MessageManagerLock mmLock;
                    String string (translate("Extracting files...") + "\n" + newFirmwareFile.getFileName());
                    infoLabel->setText(string, dontSendNotification);
                }
                
                newFirmwareFile.copyFileTo(oldFirmwareFile);
            }
        }
        else
        {
            File newAppFile (updateDirectory.getFullPathName() + File::separatorString + "Win32 Files/AlphaLive.exe");
            File oldAppFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive.exe");
            oldAppFile.deleteRecursively();
            
            {
                const MessageManagerLock mmLock;
                String string (translate("Extracting files...") + "\n" + newAppFile.getFileName());
                infoLabel->setText(string, dontSendNotification);
            }
            
            if (threadShouldExit() == true)
            {
                installationCancelled = true;
                break;
            }
            
            newAppFile.copyFileTo (oldAppFile);
            
            File newFirmwareFile (updateDirectory.getFullPathName() + File::separatorString + "Win32 Files/firmwareUpdater.exe");
            if (newFirmwareFile.exists())
            {
                File oldFirmwareFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "Application Data/firmwareUpdater.exe");
                oldFirmwareFile.deleteRecursively();
                
                // Increase the extracted size to we can work out the current progress bar value
                extractedSize += newFirmwareFile.getSize();
                // Get progress to a value between 0 and 1 (NOT 0 and 100) to update the progress bar correctly
                progress = ((extractedSize * 0.0001)/(totalSize * 0.0001));
                {
                    const MessageManagerLock mmLock;
                    String string (translate("Extracting files...") + "\n" + newFirmwareFile.getFileName());
                    infoLabel->setText(string, dontSendNotification);
                }
                
                newFirmwareFile.copyFileTo(oldFirmwareFile);
            }
        }
        #endif
        
        
        // ==== Delete downloaded files ====
        updateDirectory.deleteRecursively();
        
        #if JUCE_MAC
        File deleteMe = alphaLiveDirectory.getChildFile("__MACOSX");
        deleteMe.deleteRecursively();
        #endif 
        
        #if JUCE_MAC
        File appFile (alphaLiveDirectory.getFullPathName() + File::separatorString + "AlphaLive.app");
        #endif 
        #if JUCE_WINDOWS
        File appFile (updateDirectory.getFullPathName() + File::separatorString + "AlphaLive.exe");
        #endif 
        
        progress = -1;
        
        {
            const MessageManagerLock mmLock;
            infoLabel->setText(translate("Finishing installation..."), dontSendNotification);
        }
            wait(1000);
        {
            const MessageManagerLock mmLock;
            infoLabel->setText(translate("Relaunching AlphaLive..."), dontSendNotification);
            
        }
        
        wait(500);
        
        appFile.startAsProcess();
        
        wait(500);
        
        signalThreadShouldExit();
    }
    
    if (installationCancelled == true)
    {
        const MessageManagerLock mmLock;
        infoLabel->setText(translate("Update cancelled!"), dontSendNotification);
        closeButton->triggerClick();
    }
    else
    {
        const MessageManagerLock mmLock;
        infoLabel->setText(translate("Update complete!"), dontSendNotification);
        closeButton->triggerClick();
        
    }
}
