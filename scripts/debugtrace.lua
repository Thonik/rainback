require "fritomod/Frames";
require "fritomod/Frames-Mouse";
require "fritomod/Media-Font";
require "fritomod/Anchors";
require "fritomod/Slash";

local traceOutput = Frames.Text(UIParent, "default", 10, "black");
traceOutput:SetText("Debug trace");

local saveButton = Frames.Text(UIParent, "default", 10, "black");
saveButton:SetText("Save");

Anchors.Share(Anchors.Justify("right", 3, {traceOutput, saveButton}), "bottomright", 3);

local r;
Callbacks.Click(saveButton, function()
    if r then
        r();
    end;
    Rainback.Save();
    saveButton:SetText("Saved.");
    r=Timing.After("3s", saveButton, "SetText", "Save");
end);

local function DrawTrace()
    if DEBUG_TRACE then
        traceOutput:SetText("Debug trace is on");
    else
        traceOutput:SetText("Debug trace is off");
    end;
end;

Callbacks.Click(traceOutput, function()
    DEBUG_TRACE = not DEBUG_TRACE;
    DrawTrace();
end);
DrawTrace();

local editorFrame = Frames.New(UIParent, "Widget");
Anchors.Share(editorFrame, UIParent, "bottomleft");
Anchors.HFlip(editorFrame, traceOutput, "left");

local editor = Rainback.LineEdit();
editor.styleSheet = [[
font-size: 14px;
font-family: Inconsolata;
padding: 2px;
]];
editorFrame:SetWidget(editor);

editor:connect("returnPressed", function()
    local loadstring = loadstring;
    if not loadstring then
        loadstring = load;
    end;
    if editor.text == "exit" then
        Rainback.Close();
    elseif editor.text:match("^lua%s+") then
        local command = editor.text:gsub("^lua%s+", "");
        local runner, msg = loadstring(command);
        if runner then
            local success, rv = pcall(runner);
            print(rv);
        else
            print(msg);
        end;
    elseif editor.text:match("^eval%s+") then
        local command = editor.text:gsub("^eval%s+", "");
        local runner, msg = loadstring("return " .. command);
        if runner then
            local success, rv = pcall(runner);
            print(rv);
        else
            print(msg);
        end;
    else
        Slash.Run(unpack(Strings.Split(" ", editor.text, 2)));
    end;
    editor.text = "";
end);
