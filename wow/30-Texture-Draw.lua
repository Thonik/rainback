require "wow/Texture";
require "rainback/AnchoredBound";
require "rainback/wow-conversion";

local Delegate = OOP.Class();

function Delegate:Constructor(frame)
    self.frame = frame;
end;

function Delegate:Draw(painter)
    local frame = self.frame;
    local bound = frame:GetDelegate("layout"):GetBounds();
    if type(bound) ~= "table" or not bound:HasBounds() then
        return;
    end;
    local x, y, width, height = bound:GetBounds();
    if x == nil then
        return;
    end;
    painter:position(x, y);
    painter:setPenColor(0, 0, 0);
    painter.joinStyle = "miter";
    painter.penWidth = 1;
    painter:setFillColor(WoW.Convert.ToRainbackColor(frame:GetColor()));
    painter:drawRect(width, height);
end;

function Delegate:ToString()
    return "[Rainback Texture-Draw Delegate]";
end;

WoW.SetFrameDelegate("Texture", "draw", Delegate, "New");

