require "wow/Frame";

WoW.Delegate(WoW.Frame, "draw", {
    "Draw"
});

WoW.Frame:AddConstructor(function(frame)
    return Rainback.AddRenderer(frame, "Draw");
end);

function WoW.Frame:Draw(...)
    local delegate = self:GetDelegate("draw");
    if delegate then
        return delegate:Draw(...);
    end;
end;
