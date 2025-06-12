# Sample

This repository contains a complete working sample.

To create your own plug-in from scratch, you will need to copy some files over from this project into yours, so you might want to clone this repo.

## Creating the project

You will need to create a “Bundle” project in Xcode.  The compiled bundle will become your RHP - but it is actually set of folders much like a package.

 * Open XCode and click “Create New Project”
 * Under “Framework & Library” click “Bundle” and click “Next”.
 * Name your Bundle.  Set the Organisation. Set the Bundle extension to “rhp”.
 * Select a location on the disk and click “Create”
 * Build settings using XCConfig

Copy the XCConfig folder from the sample above into the root folder of your bundle project.  It should be at the same level as MYPROJECT.xcodeproj.

In XCode, select the name of your project in the Project Navigator (the leftmost bar), and choose “Add files to MYPROJECT”.  Select the XCConfig folder and click “Add”  Keep the defaults and choose “Finish”.

In XCode, select the name of your project in the Project Navigator (the leftmost bar), and then select the name of your project under “PROJECT” slightly to the right (not TARGETS).

Under “Configurations” - further to the right.  Open the “Debug” configuration drop down and Click where it says “None” on the first line under this. Select PlugInDebug.xcconfig.

Do the same for the release section - selecting PlugInRelease.xcconfig.

## Adding the SDK headers and frameworks.

Copy the SDK directory from the Sample Repo onto the same level as the XCConfig directory.  This includes a complete set of headers for Rhino 9 and the frameworks from an early build.  This may be updated.  A final set will be published before Rhino 9 release.

You may want to add the SDK directory to the Project Navigator so you can easily inspect the header files.

In XCode, select the name of your project in the Project Navigator (the leftmost bar), and choose “Add files to MYPROJECT”.  Select the SDK folder and click “Add”  Keep the defaults and choose “Finish”.

Now select the name of your Bundle under “TARGETS”.

On the right, click “Frameworks and Libraries” and press the “+” button.  On the following dialog, click “Add Other…” and choose “Add Files”

Find the SDK folder in your bundle project, navigate to the “lib” folder and select the three frameworks inside (OpenNURBS, RhCore, RhMaterialEditor).  Click Open.  They should now appear as Frameworks.

## Adding the source files.

In XCode, select the name of your project in the Project Navigator (the leftmost bar), and choose New File from Template.

Add a C++ File and a Header file with the name of your Plugin (adding a C++ file will also give you the option to automatically add a header file).

Make sure it is included in the target.

Add the following lines to your header file:

#include "SDK/inc/rhinoSdkStdafxPreamble.h"
#include "SDK/inc/rhinoSdk.h"
#include "SDK/inc/RhRdkHeaders.h"
#include "SDK/inc/rhinoSdkChecks.h"

You are now ready to start setting up your boilerplate CRhinoPlugIn class in the CPP file.
    
    class MYPLUGIN : public CRhinoUtilityPlugIn
    {
        GUID PlugInID() const override
        {
            static const GUID guid = { 0xxxxxx, 0xc1d1, 0x4529, { 0x8e, 0x7e, 0x7b, 0x22, 0x9d, 0x6f, 0x5a, 0xa4 } };
            return guid;
        }
        
        const wchar_t *PlugInName() const override
        {
            return L"MYPLUGIN";
        }
        
        const wchar_t *PlugInVersion() const override
        {
            return L"1.0.0";
        }
        
        int OnLoadPlugIn(void) override
        {   
            return 1;
        }
    };
    static MYPLUGIN my_plug_in;




## Compiling your plugin.


Press the Go button.  Everything should compile.

To find the compiled RHP, Choose “Show Build Folder in Finder” from the Product menu of XCode.  Navigate into the Products/Debug folder.
Getting your plugin to load into Rhino.

Download Rhino 9.
Use the TestLoadPlugin command and select your compiled bundle.



## Debugging your plug-in

By default, Rhino cannot be debugged.  However, we have included a shell script make_rhino_debuggable.sh which you should run after you download Rhino to make the bundle debuggable.  Notice that the sample xcodeproj includes this script as a RunScript action meaning that you don't actually need to run the shell command.

To run Rhino when you press the "Run" button in Xcode, Edit the Scheme to change the executable to RhinoWIP.app (from the applications folder).  Rhino will now start running in the debugger.  You can alteratively attach to the Rhino process once it is running.  In both cases, your breakpoints in your C++ plug-in code should be activated and hit.

To run the shell script, open Terminal, navigate to the repo directory and type sh make_rhino_debuggable.sh

























