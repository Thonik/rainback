require "fritomod/Frames";
require "fritomod/Frames-Mouse";
require "fritomod/Anchors";

local colors = {
   "red",
   "orange",
   "yellow",
   "green",
   "blue",
   "purple"
};

local directions = {
   {"TOPLEFT", Anchors.HStackFrom},
   {"TOPRIGHT", Anchors.JustifyFrom},
   {"BOTTOMRIGHT", Anchors.VJustifyFrom},
   {"BOTTOMLEFT", Anchors.StackFrom},
};

UIParent:GetDelegate("layout").bound = gfx;

local center = Frames.New();
Frames.Size(center, 40);
Frames.Color(center, "white");
Frames.Draggable(center);
Anchors.Center(center);

local frames = {};
for _, direction in ipairs(directions) do
   local directionFrames={}
   for _, color in ipairs(colors) do
      local f = Frames.New(center);
      Frames.Size(f, 20);
      Frames.Color(f, color);
      table.insert(frames, f);
      table.insert(directionFrames, f);
   end;
   local anchored = direction[2](direction[1], 5, directionFrames);
   Anchors.FlipTo(anchored, center, direction[1]);
end;
