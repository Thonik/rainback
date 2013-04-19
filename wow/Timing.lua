require "fritomod/Timing";

local Delegate = OOP.Class();

local FRAMERATE = 60;

function Delegate:Start()
    local elapsed = Rainback.GetTime();
    return Functions.OnlyOnce(Lists.CallEach, {
        Rainback.StartTick(FRAMERATE),
        Events.UPDATE(function()
            local current = Rainback.GetTime();
            Timing.Tick(current - elapsed);
            elapsed = current;
        end)
    });
end;

Timing.delegate = Functions.OnlyOnce(Seal(Delegate, "New"));
