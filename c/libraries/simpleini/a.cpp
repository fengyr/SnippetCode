#include "SimpleIni.h"
#include <stdio.h>

int main(int argc, const char *argv[])
{
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile("myfile.ini");
    const char * pVal = ini.GetValue("section", "key", "default");
    ini.SetValue("section", "key", "newvalue");

    std::string strData;
    ini.Save(strData);    
    printf("strData.c_str(): %s\n", strData.c_str());

    return 0;
}

