require "fritomod/ListenerList";
require "fritomod/Lists";

Rainback = Rainback or {};

local listeners = ListenerList:New();

local queue = {};

function Rainback.DispatchEvent(event, ...)
    if listeners:IsFiring() then
        table.insert(queue, Seal(Rainback.DispatchEvent, event, ...));
        return;
    end;
    local result, msg = xpcall(Curry(listeners, "Fire", event, ...), traceback);
    if not result then
        print(msg);
    end;
    while #queue > 0 do
        local firstQueued = Lists.ShiftOne(queue);
        firstQueued();
    end;
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
