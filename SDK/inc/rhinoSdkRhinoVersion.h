#pragma once

#if !defined(RHINO_SDK_CLASS)
// This file defines the RhinoVersion class which provides
// runtime access to Rhinoceros application version information.
// The RhinoVersion class is part ov the public SDK and
// is available for use in an C++ Rhino code, plug-in code
// or library code.
//
// In the rare case that you actually want the C++
// prerpocessor defines for this version information,
// you need to explicitly include rhinoSdkVersion.h.
//
#error If see the comment above this error message.
#endif

/*
Description:
  The static functions in the RhinoVersion class provide
  complete runtime version information about the Rhino.exe
  application that is running.
*/
class RHINO_SDK_CLASS RhinoVersion
{
public:

  /*
  Parameters:
    rhino_version_quartet - [out]
      rhino_version_quartet[0] = major version number
      rhino_version_quartet[1] = minor version number
      rhino_version_quartet[2] = (year-2000)*1000 + (day of year)
      rhino_version_quartet[3] = hour*1000 + minute*10 + branch
  Returns:
    RhinoVersion::AppVersionNumber()
  */
  static
  unsigned int GetVersionQuartet(
    unsigned int rhino_version_quartet[4]
    );

  /*
  Parameters:
    rhino_version_quartet - [out]
      rhino_version_quartet[0] = major version number
      rhino_version_quartet[1] = minor version number
      rhino_version_quartet[2] = (year-2000)*1000 + (day of year)
      rhino_version_quartet[3] = hour*1000 + minute*10 + branch
  Returns:
    RhinoVersion::AppVersionNumber()
  */
  static
  unsigned int GetVersionQuartet(
    unsigned short rhino_version_quartet[4]
    );

  /*
  Returns:
    Major version number (>=6)
    For example, 6 for Rhino 6.x.
  */
  static 
  unsigned int Major();

  /*
  Returns:
    Minor version number (>= 0)
    For example, 1 for Rhino 6.1
  */
  static 
  unsigned int Minor();

  /*
  Returns:
    Year this version was built. ( >= 2014)
  */
  static 
  unsigned int Year();

  /*
  Returns:
    Month of the year this version was built (1 to 12).
  */
  static 
  unsigned int Month();
  
  /*
  Returns:
    Day of the month this version was built (1 to 31).
  */
  static 
  unsigned int DayOfMonth();
  
  /*
  Returns:
    Day of the year this version was built (1 to 366).
  */
  static 
  unsigned int DayOfYear();
    
  /*
  Returns:
    Hour of the day this version was built (0 to 23).
  */
  static 
  unsigned int Hour();

    
  /*
  Returns:
    Minute of the hour this version was built (0 to 59).
  */
  static 
  unsigned int Minute();

  /*
  Returns:
    RHINO_VERSION_BRANCH
      0: developer build
      1: Windows Commercial build
      2: Mac Commercial build
      3: Windows BETA build
      4: Mac Beta build
      5: Windows WIP build
      6: Mac WIP build
  */
  static 
  unsigned int Branch();

  /*
  Returns:
    The version number of the Rhino.exe application in the
    form used to identify opennurbs and Rhino SDK versions.
  Remarks:
    - RhinoVersion::AppSdkVersionNumber() <= RhinoVersion::AppSdkServiceReleaseNumber()
    - RhinoVersion::AppSdkServiceReleaseNumber() <= RhinoVersion::AppVersionNumber()
      (typically == in public release builds)
    - RhinoVersion::AppVersionNumber() = ON_VersionNumberConstruct(
                                            RhinoVersion::Major(),
                                            RhinoVersion::Minor(),
                                            RhinoVersion::Year(),
                                            RhinoVersion::Month(),
                                            RhinoVersion::DayOfMonth(),
                                            RhinoVersion::Branch()
                                            );
  */
  static
  unsigned int AppVersionNumber();


  /*
  Returns:
    The version number of the 3dm file format the Rhino.exe 
    application writes by default.
  Remarks:
    - For public release, service release and public WIP versions of Rhino,
      this number is identical to RhinoVersion::Major().
    - When a new version of Rhino is in "alpha", this version
      number may be RhinoVersion::Major()-1 during the time it 
      takes to get all application and IO code synchronized.
  */
  static
  unsigned int App3dmVersionNumber();

  /*
  Returns:
    The version number of the Rhino.exe application sdk
    in the form used to identify opennurbs and Rhino SDK 
    versions.
  Remarks:
    - RhinoVersion::AppSdkVersionNumber() <= RhinoVersion::AppSdkServiceReleaseNumber()
    - RhinoVersion::AppSdkServiceReleaseNumber() <= RhinoVersion::AppVersionNumber()
      (typically == in public release builds)
    - In order for a plug-in to load, the values of
      plug-in's sdk version number and
      RhinoVersion::AppSdkVersionNumber()
      must be identical.
    - RhinoVersion::AppSdkVersionNumber() = ON_VersionNumberConstruct(
                                              RhinoVersion::Major(),
                                              minor,
                                              year,month,day_of_month,branch
                                              );
      where minor,year,month,day_of_month,branch are the values of the version
      information on the day the sdk for this version of Rhino was "frozen".
  */
  static
  unsigned int AppSdkVersionNumber();

  /*
  Returns:
    The version number of the Rhino.exe application 
    sdk service release in the form used to identify
    opennurbs and Rhino SDK 
    versions.
  Remarks:
    - RhinoVersion::AppSdkVersionNumber() <= RhinoVersion::AppSdkServiceReleaseNumber()
    - RhinoVersion::AppSdkServiceReleaseNumber() <= RhinoVersion::AppVersionNumber()
      (typically == in public release builds)
    - In order for a plug-in to load, the plug-ins
      sdk service release must be <= RhinoVersion::AppSdkServiceReleaseNumber().
      RhinoVersion::AppSdkServiceReleaseNumber() >= RhinoVersion::AppSdkVersionNumber().
  */
  static
  unsigned int AppSdkServiceReleaseNumber();


  /*
  Returns:
    The application name  this version.  
    For example, "Rhinoceros 6"
  */
  static
  const wchar_t* AppNameWideString();

  static
  const char* AppNameString();

  /*
  Returns:
    The value of VersionMajor() as a string.
  */
  static
  const wchar_t* MajorWideString();
  static
  const char* MajorString();

  /*
  Returns:
    "SRn" for service release n, where n is an integer.
    For example, the "SR1" will be returned for service release 1.
  */
  static
  const wchar_t* ServiceReleaseWideString();
  static
  const char* ServiceReleaseString();

 /*
  Returns:
    The version quartet as string of four values separated by periods.
    For example, "6,0,14065,18590" would be version 6, service release
    0, built on 18:19, day 65, year 2014.  The final digit identifies
    the branch.
  */
  static
  const wchar_t* VersionQuartetWideString();
  static
  const char* VersionQuartetString();


  /*
  Returns:
    The registry root for this version.  
    For example, "McNeel\\Rhinoceros\\6.0"
  */
  static
  const wchar_t* RegistryRootWideString();
  static
  const char* RegistryRootString();

  /*
  Returns:
    A string that describes the build.  For a public build, this is something
    like "Release".  For a developer build, this is something like
    "Private Developer Build".
  */
  static 
  const char* BuildDescriptionString();
  
private:
  // prohibit instantiation - no implementation
  RhinoVersion() = delete;
  ~RhinoVersion() = delete;
};

/*
Description:
The RhinoWindowsVersionInformation class provides
provide complete runtime version information about the version
of the Windows operating system that the Rhino.exe application
that is running on.
*/

#if defined(ON_RUNTIME_WIN)
class RHINO_SDK_CLASS RhinoWindowsVersionInformation
{
public:
  // The RhinoWindowsVersionInformation class
  // is intended to be used when code needs to know
  // what version of Windows is being run.
  ~RhinoWindowsVersionInformation();
  /*
  Description:
    Windows version information for the currently running device.
  */
  static const RhinoWindowsVersionInformation& ThisDevice();

  /*
  Returns:
    A RhinoWindowsVersionInformation created from the information in infoex.
  Remarks:
    Use RhinoWindowsVersionInformation::ThisDevice() when you need the Windows
    version information for the currently running device.
  */
  RhinoWindowsVersionInformation(const OSVERSIONINFOEXW& infoex, bool bIs64Bit = true);

    /*
  Description:
    Performs a dictionary compare of major version, minor version,
    service pack major, service pack minor and, optionally, build 
    number, in dictionary order.
  Parameters:
    bCompareBuildNumber - [in]
    true - the build number is used in the compare.
    false - the build number is ignored.
  */
  static int CompareVersionNumbers(const RhinoWindowsVersionInformation& a, const RhinoWindowsVersionInformation& b, bool bCompareBuildNumber = false);

  /*
  Use these functions when you need to decide which Windows services
  are available.
  */
  bool IsWindows7OrGreater() const;
  bool IsWindows7SP1OrGreater() const;
  bool IsWindows8OrGreater() const;
  bool IsWindows8Point1OrGreater() const;
  bool IsWindows10OrGreater() const;
  bool IsWindows11OrGreater() const;
  bool IsWindowsServer() const;
  bool IsWindows64Bit() const;
 
  /*
  Returns:
    A short description like "Windows 7SP1".
  */
  ON_String ShortDescription() const;

  /*
  Returns:
    A short description returned from an additional source.
  */
  ON_String AdditionalDescription() const;

  /*
  Returns:
    A detailed description like 
    "Windows 7SP1, 64-bit, platform 2, version 6.1 build 7601 sp 1.0, suite 256, product type 1, Service Pack 1"
  */
  ON_String DetailedDescription() const;

  ON_String MachineName() const;

  bool IsWindowsRunningInAVirtualMachine() const;
  ON_String VirtualMachineDescription() const;

  /*
  Returns:
     Windows major version number.
     (6 for WIndows 7, 7SP1, 8 and 8.1)
  Remarks:
    Always use one of the IsWindows...OrGreater() functions when deciding
    on which Windows services to use.
    This function is incluced to supply diagnostic information.
  */
  unsigned int MajorVersion() const;

  /*
  Returns:
    Windows minor version number.
  Remarks:
    Always use one of the IsWindows...OrGreater() functions when deciding
    on which Windows services to use.
    This function is included to supply diagnostic information.
  */
  unsigned int MinorVersion() const;

  /*
  Returns:
    Windows service pack major.
  Remarks:
    Always use one of the IsWindows...OrGreater() functions when deciding
    on which Windows services to use.
    This function is included to supply diagnostic information.
  */
  unsigned int ServicePackMajor() const;

  /*
  Returns:
    Windows service pack minor.
  Remarks:
    Always use one of the IsWindows...OrGreater() functions when deciding
    on which Windows services to use.
    This function is included to supply diagnostic information.
  */
  unsigned int ServicePackMinor() const;

  /*
  Returns:
    The build number.
  Remarks:
    Always use one of the IsWindows...OrGreater() functions when deciding
    on which Windows services to use.
    This function is included to supply diagnostic information.
  */
  unsigned int BuildNumber() const;

  /*
  Returns:
    One of the four values:
      0
      VER_NT_WORKSTATION
      VER_NT_DOMAIN_CONTROLLER
      VER_NT_SERVER
  Remarks:
    Always use one of the IsWindowsServer() function when deciding
    on which Windows services to use.
    This function is included to supply diagnostic information.
  */
  unsigned int ProductType() const;

private:
  //Use RhinoWindowsVersionInformation::ThisDevice() to get the version of Windows this instance of Rhino is running on
  RhinoWindowsVersionInformation();

  class Impl;
  Impl* m_pImpl = nullptr;
  friend class Impl;
};
#endif
