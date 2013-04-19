require "wow/Frame";

function WoW.CreateUIParent()
    local root = WoW.Frame:New();

    local ScreenBound = Rainback.AnchoredBound:New();

    function ScreenBound:GetBounds()
        return 0, 0, Rainback.ScreenWidth(), Rainback.ScreenHeight();
    end;

    function ScreenBound:HasBounds()
        return true;
    end;

    root:GetDelegate("layout").bound = ScreenBound;
    return root;
end;
