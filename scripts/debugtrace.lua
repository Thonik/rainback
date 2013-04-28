require "fritomod/Frames";
require "fritomod/Frames-Mouse";
require "fritomod/Media-Font";
require "fritomod/Anchors";

local traceOutput = Frames.Text(UIParent, "default", 10, "black");
traceOutput:SetText("Debug trace");
Anchors.Share(traceOutput, "bottomright", 3);

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
editorFrame:SetWidget(editor);

editor:connect("returnPressed", function()
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
    end;
    editor.text = "";
end);
