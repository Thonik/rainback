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
