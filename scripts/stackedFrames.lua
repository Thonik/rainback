require "fritomod/Frames";
require "fritomod/Frames-Mouse";
require "fritomod/Frames-Position";
require "fritomod/Anchors";

local colors = { "red", "orange", "yellow", "green", "blue", "purple" };

local directions = {
    {"TOPLEFT", Anchors.HStackFrom},
    {"TOPRIGHT", Anchors.JustifyFrom},
    {"BOTTOMRIGHT", Anchors.VJustifyFrom},
    {"BOTTOMLEFT", Anchors.StackFrom},
};

local center = Frames.New();
Frames.Size(center, 40);
Frames.Color(center, "white");
Frames.Draggable(center);
Frames.Position(center, "Center", Anchors.Center);

local frames = {};
for _, direction in ipairs(directions) do
    local directionFrames = {}
    for _, color in ipairs(colors) do
        local f = Frames.New(center);
        Frames.Draggable(f);
        Frames.Size(f, 20);
        Frames.Color(f, color);
        table.insert(frames, f);
        table.insert(directionFrames, f);
    end;
    local anchor = direction[1];
    local anchorFunc = direction[2];
    local gap = 3;
    local anchored = anchorFunc(anchor, gap, directionFrames);
    Anchors.FlipTo(anchored, center, anchor, gap);
end;
