require "wow/Frame";
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
    painter:setFont(frame:GetDelegate("text"):GetInternalFont());
    painter:setPenColor(WoW.Convert.ToRainbackColor(frame:GetTextColor()));
    painter:drawText(frame:GetText());
end;

function Delegate:ToString()
    return "[Rainback FontString-Draw Delegate]";
end;

WoW.SetFrameDelegate("FontString", "draw", Delegate, "New");
