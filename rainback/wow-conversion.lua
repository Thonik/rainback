WoW = WoW or {};
local Convert = {};
WoW.Convert = Convert;

function WoW.Convert.ToRainbackColor(r, g, b, a)
    r = r * 255;
    g = g * 255;
    b = b * 255;
    if a ~= nil then
        a = a * 255;
    else
        a = 255
    end;
    return r, g, b, a;
end;

