require "fritomod/ListenerList";

Rainback = Rainback or {};

local listeners = ListenerList:New();

function Rainback.DispatchEvent(event, ...)
    listeners:Fire(event, ...);
end;

function Rainback.OnEvent(func, ...)
    return listeners:Add(func, ...);
end;

function Rainback.OnNamedEvent(name, func, ...)
    func = Curry(func, ...);

    return Rainback.OnEvent(function(eventName, ...)
        if eventName == name then
            func(...);
        end;
    end);
end;
