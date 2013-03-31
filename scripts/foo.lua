require "fritomod/currying";
require "rainback/Graphics";

gfx = Rainback.Graphics:New();

paint = Curry(gfx, "Render");
