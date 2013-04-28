require "fritomod/Strings";
require "fritomod/Lists";
require "fritomod/Frames-Position";
require "hack/Script";

local globals = Rainback.Globals;

local frame = Frames.New(UIParent, "Widget");

local editor = Rainback.TextEdit();
frame:SetWidget(editor);

local script = Hack.Script:New();
editor:connect("textChanged", function()
    script:SetContent(editor.plainText);
end);

Callbacks.PersistentValue("EditorText", function(text)
    editor.plainText = text;
    script:SetContent(editor.plainText);
    return function()
        return editor.plainText;
    end;
end);
editor.tabStopWidth = 30;
editor.styleSheet = [[
font-family: Inconsolata;
font-size: 13px;
border: 1px solid black;
color: black;
]];

local title = Frames.New(UIParent, "Texture");
Frames.WH(title, 300, 20);
Frames.Draggable(title);
Frames.Position(title, "Editor Anchor", Anchors.Center);
Frames.Color(title, "grey");

Anchors.VFlipFromTop(frame, title, -1);
Frames.Height(frame, 300);

local run = Frames.Text(title, "default", 10);
Frames.Color(run, "white");
run:SetText("Run");
Anchors.Share(run, title, "left", 3, 3);

Callbacks.Click(run, script, "Execute");

local continual = Frames.Text(title, "default", 10);
Frames.Color(continual, "white");
continual:SetText("Run On Change");
Anchors.Share(continual, title, "right", 2);

local function Run()
    local success, msg = pcall(script.Execute, script);
    if not success then
        print(msg);
    end;
end;

local remover;
Callbacks.Click(continual, function()
    if remover then
        remover();
        remover=nil;
        Frames.Color(title, "grey");
        return;
    end;
    remover = script:OnChange(Run);
    Frames.Color(title, 0, .5, 0);
    Run();
end);
