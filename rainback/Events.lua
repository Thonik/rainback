require "fritomod/ListenerList";

Rainback = Rainback or {};

local listeners = ListenerList:New();

function Rainback.DispatchEvent(event, ...)
    listeners:Fire(event, ...);
end;

function Rainback.OnEvent(func, ...)
    return listeners:Add(func, ...);
end;
