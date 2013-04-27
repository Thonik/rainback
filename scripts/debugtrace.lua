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

