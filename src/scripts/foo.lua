require "fritomod/currying";
require "rainback/Graphics";

local gfx = Rainback.Graphics:New();

paint = Curry(gfx, "Render");
