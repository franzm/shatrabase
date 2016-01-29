; Shatrabase InnoSetupScript

[Setup]
AppName=Shatrabase
AppVersion=0.97
DefaultDirName={pf}\Shatrabase
UninstallDisplayIcon={app}\bin\shatrabase.exe
ShowLanguageDialog=yes
DefaultGroupName=Shatrabase

[Languages]
Name: ru; MessagesFile: "compiler:Languages\Russian.isl"
Name: en; MessagesFile: "compiler:Default.isl"
Name: de; MessagesFile: "compiler:Languages\German.isl"


[Files]
Source: "bin\*"; DestDir: "{app}\bin"
Source: "bin\platforms\*"; DestDir: "{app}\bin\platforms"
;Source: "bin\engines\*"; DestDir: "{app}\bin\engines"
;Source: "Readme.txt"; DestDir: "{app}"; Flags: isreadme

[Icons]
Name: "{group}\Shatrabase"; Filename: "{app}\bin\shatrabase.exe"
Name: "{commondesktop}\Shatrabase"; Filename: "{app}\bin\shatrabase.exe"
Name: "{group}\Uninstall Shatrabase"; Filename: "{uninstallexe}"