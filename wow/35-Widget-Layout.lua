require "wow/Frame";
require "rainback/AnchoredBound";
require "rainback/wow-conversion";

local Widget = WoW.Widget;

Widget:AddConstructor(function(widget)
    widget:OnDelegateSet(function(category, delegate)
        if category ~= "layout" then
            return;
        end;
        widget:AttachLayout(delegate);
    end);
    local delegate = widget:GetDelegate("layout");
    if delegate then
        widget:AttachLayout(delegate);
    end;
end);

function Widget:AttachLayout(delegate)
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

function Widget:GetNaturalWidth(height)
    if self:GetWidget() then
        return self.widget.sizeHint.width;
    end;
end;

function Widget:GetNaturalHeight(width)
    if self:GetWidget() then
        return self.widget.sizeHint.height;
    end;
end;
