require "wow/Frame";
require "rainback/AnchoredBound";
require "rainback/Graphics";

local Delegate = OOP.Class();

function Delegate:Constructor(frame)
    self.frame = frame;
    self.text = "";

    self.font = Rainback.Font();

    self.color = {0, 0, 0, 1};

    frame:OnDelegateSet(function(category, delegate)
        if category ~= "layout" then
            return;
        end;
        self:AttachLayout(delegate);
    end);
    local delegate = frame:GetDelegate("layout");
    if delegate then
        frame:AttachLayout(delegate);
    end;
end;

function Delegate:AttachLayout(delegate)
    if not delegate.GetBounds then
        return;
    end;
    local bounds = delegate:GetBounds();
    if type(bounds) ~= "table" then
        return;
    end;
    if not bounds.GetNaturalHeight or not bounds.GetNaturalWidth then
        return;
    end;
    bounds.GetNaturalHeight = Override(self, "GetNaturalHeight");
    bounds.GetNaturalWidth = Override(self, "GetNaturalWidth");
end;

function Delegate:GetText()
    return self.text;
end;

function Delegate:SetText(text)
    self.text = tostring(text);
    Rainback.Update();
end;

function Delegate:GetNaturalWidth(height)
    return self.font:width(self.text);
end;

function Delegate:GetNaturalHeight(width)
    return self.font:height(self.text, width);
end;

function Delegate:GetWrappedWidth()
    return self:GetNaturalWidth();
end;

function Delegate:GetStringWidth()
    return self:GetNaturalWidth();
end;

function Delegate:GetStringHeight()
    return self:GetNaturalHeight();
end;

function Delegate:SetFont(family, size)
    self.font.family = family;
    if size then
        self.font.pointSize = size;
    end;
    Rainback.Update();
end;

function Delegate:GetFont()
    return self.font.family, self.font.pointSize;
end;

function Delegate:SetTextColor(r, g, b, a)
    self.color = {r, g, b, a};
    Rainback.Update();
end;

function Delegate:GetTextColor()
    return unpack(self.color);
end;

function Delegate:GetInternalFont()
    return self.font;
end;

function Delegate:ToString()
    return "[Rainback Text Delegate]";
end;

WoW.SetFrameDelegate("FontString", "text", Delegate, "New");
