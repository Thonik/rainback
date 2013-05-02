require "wow/Frame";
require "rainback/AnchoredBound";
require "rainback/wow-conversion";

local Widget = OOP.Class(WoW.Frame);
WoW.Widget = Widget;
WoW.RegisterFrameType("Widget", Widget);

Widget:AddDestructor(function(self)
    if self:GetWidget() then
        self:GetWidget():hide();
    end;
end);

function Widget:GetWidget()
    return self.widget;
end;

function Widget:SetWidget(widget)
    self.widget = widget;
end;

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
    local widget = frame:GetWidget();
    if not widget then
        return;
    end;
    local x, y, width, height = bound:GetBounds();
    if x ~= nil then
        width = math.min(width, 10000);
        height = math.min(height, 10000);
        widget:show();
        widget:setGeometry(x, y, width, height);
    else
        widget:hide();
    end;
end;

function Delegate:ToString()
    return "[Rainback Widget-Draw Delegate]";
end;

WoW.SetFrameDelegate("Widget", "draw", Delegate, "New");
