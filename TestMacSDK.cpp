//
//  TestMacCPPPlugIn.cpp
//  TestMacCPPPluginCocoa
//
//  Created by Andrew Le Bihan on 1.11.2024.
//

#include "TestMacSDK.h"


class TestMacPlugIn : public CRhinoUtilityPlugIn
{
    GUID PlugInID() const override
    {
        static const GUID guid =
        { 0xfc563bb1, 0xc1d1, 0x4529, { 0x8e, 0x7e, 0x7b, 0x22, 0x9d, 0x6f, 0x5a, 0xa4 } };
        return guid;
    }
    
    const wchar_t *PlugInName() const override
    {
        return L"TestMacPlugIn";
    }
    
    const wchar_t *PlugInVersion() const override
    {
        return L"1.0.0";
    }
    
    int OnLoadPlugIn(void) override
    {
        RhinoApp().Print(L"This is coming from a third party C++ plug-in on Mac\n");
        
        return 1;
    }
};

static TestMacPlugIn my_plug_in;


static class MyCPPCommand : public CRhinoCommand
{
    UUID CommandUUID() override
    {
        static const GUID guid =
        { 0xac563bb1, 0xa1d1, 0x4529, { 0x8e, 0x7e, 0x7b, 0x22, 0xad, 0x6f, 0x5a, 0xa4 } };
        return guid;
    }
    
    const wchar_t *EnglishCommandName() override
    {
        return L"MyCPPCommand";
    }
    
    CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override
    {
        const ON_Line line(ON_origin, ON_3dPoint(4.0, 5.0, 6.0));
        
        context.m_doc.AddCurveObject(line);
        
        return success;
    }
    
} mycommand;
