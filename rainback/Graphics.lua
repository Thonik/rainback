require "fritomod/ListenerList";

Rainback = Rainback or {};

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
