require "fritomod/currying";
require "rainback/Graphics";

require "fritomod/Events";

local gfx = Rainback.Graphics:New();
Events.RENDER(gfx, "Render");
