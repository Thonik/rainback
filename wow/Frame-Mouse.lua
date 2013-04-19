require "wow/Frame";

require "fritomod/Lists";
require "fritomod/Events";

require "rainback/Events";

local Delegate = OOP.Class();

function Delegate:Constructor(frame)
    self.frame = frame;
    self.enabled = false;
end;

function Delegate:EnableMouse(enabled)
    if self.enabled == enabled then
        return;
    end;
    self.enabled = enabled;
    if enabled then
        self.removers = {
            Events.MOUSEPRESS(function(button, x, y)
                if self.frame:GetDelegate("layout"):HitTest(x, y) then
                    self.frame:FireEvent("OnMouseDown", button);
                end;
            end),
            Events.MOUSERELEASE(function(button, x, y)
                if self.frame:GetDelegate("layout"):HitTest(x, y) then
                    self.frame:FireEvent("OnMouseUp", button);
                end;
            end),
        };
    else
        Lists.CallEach(self.removers);
        self.removers = nil;
    end;
end;

WoW.SetFrameDelegate("Frame", "mouse", Delegate, "New");

local buttons = {};

Rainback.OnEvent(function(event, button)
    if event == "MOUSEPRESS" then
        buttons[button] = true;
    elseif event == "MOUSERELEASE" then
        buttons[button] = false;
    end;
end);

function IsMouseButtonDown(button)
    return buttons[button];
end;

function GetCursorPosition()
    local x, y = Rainback.GetCursorPosition();
    return x, Rainback.ScreenHeight() - y;
end;
