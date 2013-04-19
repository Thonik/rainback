require "fritomod/currying";
require "rainback/Graphics";

require "fritomod/Events";

-- This still needs to be global until Frame-Layout can refer to this object
-- in a better way.
gfx = Rainback.Graphics:New();
Events.RENDER(gfx, "Render");

Events.MOUSEPRESS(function(button, x, y)
    print("Mouse press: " .. button .. " at " .. x .. ", " .. y);
end);

Events.MOUSERELEASE(function(button, x, y)
    print("Mouse release: " .. button .. " at " .. x .. ", " .. y);
end);

Events.MOUSEMOVE(function(x, y)
    --print("Mouse move: " .. x .. ", " .. y);
end);

Events.MOUSEWHEEL(function(delta)
    --print("Wheel delta is: " .. delta)
end);
