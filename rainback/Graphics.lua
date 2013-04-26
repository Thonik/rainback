require "fritomod/ListenerList";

Rainback = Rainback or {};

local boundsCache;
function Rainback.ClearCache()
    boundsCache = setmetatable({}, {
        __mode = "k"
    });
end;
Rainback.ClearCache();

function Rainback.GetCached(bound)
    return boundsCache[bound];
end;

function Rainback.SetCache(bound, value)
    boundsCache[bound] = value;
end;

local renderers = ListenerList:New();

function Rainback.AddRenderer(renderer, ...)
    return renderers:Add(renderer, ...);
end;

function Rainback.Render(painter)
    trace("FRAME start");

    Rainback.ClearCache();

    renderers:Fire(painter);

    painter:reset();
    painter:setPenColor(0, 0, 0, 255);
    painter:drawText("Rainback v1.0");

    trace("FRAME complete");
end;
