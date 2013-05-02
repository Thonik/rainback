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
    local start = Rainback.GetTime();

    Rainback.ClearCache();

    renderers:Fire(painter);

    painter:reset();
    painter:setPenColor(0, 0, 0, 255);
    painter:drawText("Rainback v1.0");

    local elapsed = Rainback.GetTime() - start;
    if elapsed > 0 then
        local fps = 1000 / elapsed;

        local bar = "";
        local SCALE = 2;
        local GOAL = 1000 / 60;
        for i=1, math.floor(elapsed / SCALE) do
            if i * SCALE > GOAL then
                bar = bar .. "!";
            else
                bar = bar .. ".";
            end;
        end;

        printf("%3dms elapsed, %2dfps %s", elapsed, fps, bar);
    else
        printf("  0ms elapsed");
    end;
    trace("FRAME complete");
end;
