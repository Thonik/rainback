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
    for i, color in ipairs(colors) do
        local f = Frames.New(center);
        local name = direction[1] .. color .. i;
        Frames.Draggable(f);
        Frames.Size(f, 20);
        Frames.Color(f, color);

        frames[name] = f;
        Callbacks.Script(f, "DragStart", function()
            f.persisted = true;
        end);

        table.insert(directionFrames, f);

    end;
    local anchor = direction[1];
    local anchorFunc = direction[2];
    local gap = 3;
    local anchored = anchorFunc(anchor, gap, directionFrames);
    Anchors.FlipTo(anchored, center, anchor, gap);
end;

Callbacks.PersistentValue("StackedFrames", function(loaded)
    if loaded then
        for name, anchors in pairs(loaded) do
            local frame = frames[name];
            frame:ClearAllPoints();
            frame:SetParent(UIParent);
            Serializers.LoadAllPoints(anchors, frame);
            frame.persisted = true;
        end;
    end;
    return function()
        local saved = {};
        for name, frame in pairs(frames) do
            if frame.persisted then
                saved[name] = Serializers.SaveAllPoints(frame);
            end;
        end;
        return saved;
    end;
end);
