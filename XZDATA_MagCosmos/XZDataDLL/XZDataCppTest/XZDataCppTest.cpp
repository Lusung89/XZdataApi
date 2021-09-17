//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("XZDataCppTest.res");
USEFORM("UFmXZData.cpp", FmMain);
USEUNIT("..\src\ULOG.cpp");
USEUNIT("..\src\UReport.cpp");
USEUNIT("..\src\UXZDataClass.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFmMain), &FmMain);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
