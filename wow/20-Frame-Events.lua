require "wow/Frame";

local Delegate = OOP.Class();

function Delegate:Constructor(frame)
    self.frame = frame;
    self.eventHandlers = {};
end;

function Delegate:HasScript(event)
    return event ~= "OnClick";
end;

function Delegate:FireEvent(event, ...)
    local handler = self.eventHandlers[event];
    if handler then
        handler(self.frame, ...);
    end;
end;

function Delegate:SetScript(event, handler)
    if not self:HasScript(event) then
        return;
    end;
    trace(event);
    self.eventHandlers[event] = handler;
end;

function Delegate:GetScript(event)
    return self.eventHandlers[event];
end;

function Delegate:HookScript(event, handler)
    error("Hooking is not supported");
end;

function Delegate:IsEventRegistered()
    error("Global events cannot be registered from frames");
end;

function Delegate:RegisterEvent(event)
    error("Global events cannot be registered from frames");
end;

function Delegate:UnregisterEvent(event)
    error("Global events cannot be registered from frames");
end;

WoW.SetFrameDelegate("Frame", "event", Delegate, "New");
