#Shell script to allow Rhino to be debugged for Rhino for Mac SDK plugins.

#Get rid of any previous detritus
unlink ~/rhino.entitlements

#Extract the entitlements for RhinoWIP to an XML file
codesign --display --xml --entitlements ~/rhino.entitlements /Applications/RhinoWIP.app

if grep -wq "get-task-allow" ~/rhino.entitlements; then 

	echo "Rhino is already debuggable"

else

	#Replace a portion of the XML to inject the get-task-allow entitlement
	sed -i.bak "s%<key>com.apple.security.cs.allow-jit</key><true/>%<key>com.apple.security.cs.allow-jit</key><true/><key>com.apple.security.get-task-allow</key><true/>%" ~/rhino.entitlements

	#cat ~/rhino.entitlements

	#Inject the modified entitlements into the RhinoApp bundle.
	codesign -s - --deep --force --options=runtime --entitlements ~/rhino.entitlements /Applications/RhinoWIP.app

	echo "Rhino is now debuggable"

fi

#Clean up the working file
unlink ~/rhino.entitlements

