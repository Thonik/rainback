require "wow/Texture";
require "rainback/AnchoredBound";
require "rainback/wow-conversion";

local Delegate = OOP.Class();

function Delegate:Constructor(frame)
    self.frame = frame;
end;

function Delegate:Draw(painter)
    painter:reset();

    local frame = self.frame;
    local bound = frame:GetDelegate("layout"):GetBounds();
    if type(bound) ~= "table" or not bound:HasBounds() then
        return;
    end;
    local x, y, width, height = bound:GetBounds();
    if x == nil then
        return;
    end;
    if frame:GetColor() == nil then
        return;
    end;
    painter.penWidth = 1;
    painter.joinStyle = "miter";

    painter:position(x + painter.penWidth / 2, y + painter.penWidth / 2);
    painter:setFillColor(WoW.Convert.ToRainbackColor(frame:GetColor()));
    painter:drawRect(width - painter.penWidth, height - painter.penWidth);
end;

function Delegate:ToString()
    return "[Rainback Texture-Draw Delegate]";
end;

WoW.SetFrameDelegate("Texture", "draw", Delegate, "New");
