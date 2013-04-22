require "fritomod/Frames";
require "fritomod/Frames-Mouse";
require "fritomod/Media-Font";
require "fritomod/Anchors";
require "fritomod/Chat";

local colors = {
    "white",
    "red",
    "orange",
    "yellow",
    "green",
    "blue",
    "purple",
};

local frames = {};

for _, color in ipairs(colors) do
    local icon = Frames.New();
    Frames.Color(icon, color);
    Frames.Size(icon, 20);
    table.insert(frames, icon);

    local text = Frames.Text(icon, "default", 10, "black");
    text:SetText(color);
    Callbacks.Click(text, Chat.debug, color);
    Anchors.Flip(text, icon, "right", 5);
end;

local ref = Anchors.VCJustify("topleft", 5, frames);
Anchors.Share(ref, "left", 5);

local traceOutput = Frames.Text(UIParent, "default", 10, "black");
traceOutput:SetText("Debug trace");
Anchors.Share(traceOutput, "bottomleft");

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
