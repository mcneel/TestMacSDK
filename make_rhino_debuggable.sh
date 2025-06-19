#!/usr/bin/env bash
#First command-line arg should be full path to app bundle, e.g.: ./make_rhino_debuggable.sh ~/Desktop/RhinoWIP.app
#Shell script to allow Rhino to be debugged for Rhino for Mac SDK plugins.

TEMP_ENTITLEMENTS=$HOME/rhino.entitlements

#Get rid of any previous detritus
unlink $TEMP_ENTITLEMENTS

#Extract the entitlements for app bundle to an XML file
codesign --display --xml --entitlements $TEMP_ENTITLEMENTS $1

if grep -wq "get-task-allow" $TEMP_ENTITLEMENTS; then 
	echo "Rhino is already debuggable"
else
	#Replace a portion of the XML to inject the get-task-allow entitlement
	sed -i.bak "s%<key>com.apple.security.cs.allow-jit</key><true/>%<key>com.apple.security.cs.allow-jit</key><true/><key>com.apple.security.get-task-allow</key><true/>%" $TEMP_ENTITLEMENTS

	#cat $TEMP_ENTITLEMENTS
	
	#Inject the modified entitlements into the app bundle.
	codesign -s - --deep --force --options=runtime --entitlements $TEMP_ENTITLEMENTS $1

	echo "Rhino is now debuggable"
fi

#Clean up the temporary working file
unlink $TEMP_ENTITLEMENTS
