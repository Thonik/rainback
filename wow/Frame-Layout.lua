require "wow/Frame";
require "rainback/AnchoredBound";
require "rainback/Graphics";

local Delegate = OOP.Class();

function Delegate:Constructor(frame)
    self.frame = frame;
    self.bound = Rainback.AnchoredBound:New();
    gfx:AddDelegate(self);
end;

function Delegate:GetWidth()
    return self.bound:GetWidth();
end;

function Delegate:SetWidth(width)
    return self.bound:SetWidth(width);
end;

function Delegate:GetHeight()
    return self.bound:GetHeight();
end;

function Delegate:SetHeight(height)
    return self.bound:SetHeight(height);
end;

function Delegate:SetPoint(anchor, ref, anchorTo, x, y)
    return self.bound:SetPoint(anchor, ref:GetDelegate("layout"):GetBounds(), anchorTo, x, -y);
end;

function Delegate:GetBounds()
    return self.bound;
end;

function Delegate:ToString()
    return "[Rainback Layout Delegate]";
end;

WoW.SetFrameDelegate("Frame", "layout", Delegate, "New");
