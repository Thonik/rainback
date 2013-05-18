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

    local elapsed = Rainback.GetTime() - start;
    if elapsed > 0 then
        local fps = 1000 / elapsed;

        local bar = "[";
        local SCALE = 2;
        local GOAL = 1000 / 60;
        for i=1, math.max(GOAL / SCALE, math.floor(elapsed / SCALE)) do
            if i * SCALE > elapsed then
                bar = bar .. " ";
            elseif i * SCALE > GOAL then
                bar = bar .. "!";
            else
                bar = bar .. ".";
            end;
        end;
        if elapsed < GOAL then
            bar = bar .. "]";
        end;

        local numRenderers = renderers:GetListenerCount();

        trace(("%3dms elapsed, %3dfps %3d renderers %s"):format(
            elapsed,
            fps,
            numRenderers,
            bar
        ));
    else
        trace(("  0ms elapsed, ---fps %4d renderers"):format(
            numRenderers
        ));
    end;
    trace("FRAME complete");
end;
