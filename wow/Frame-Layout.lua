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
    Rainback.Update();
    return self.bound:SetWidth(width);
end;

function Delegate:GetHeight()
    return self.bound:GetHeight();
end;

function Delegate:SetHeight(height)
    Rainback.Update();
    return self.bound:SetHeight(height);
end;

function Delegate:ClearAllPoints()
    Rainback.Update();
    self.bound:Clear();
end;

function Delegate:SetPoint(anchor, ref, anchorTo, x, y)
    Rainback.Update();
    return self.bound:SetPoint(anchor, ref:GetDelegate("layout"):GetBounds(), anchorTo, x, -y);
end;

function Delegate:GetCenter()
    local x, y = self.bound:GetCenter();
    return x, Rainback.ScreenHeight() - y;
end;

function Delegate:GetLeft()
    return self.bound:GetLeft();
end;

function Delegate:GetRight()
    return self.bound:GetRight();
end;

function Delegate:GetTop()
    -- These are intentionally flipped to convert to WoW's coordinate system
    return self.bound:GetBottom();
end;

function Delegate:GetBottom()
    -- These are intentionally flipped to convert to WoW's coordinate system
    return self.bound:GetTop();
end;

function Delegate:GetBounds()
    return self.bound;
end;

function Delegate:HitTest(x, y)
    local left, bottom, right, top = self:GetLeft(), self:GetBottom(), self:GetRight(), self:GetTop();
    return
        x >= left and x <= right
        and y >= bottom and y <= top;
end;

function Delegate:ToString()
    return "[Rainback Layout Delegate]";
end;

WoW.SetFrameDelegate("Frame", "layout", Delegate, "New");
