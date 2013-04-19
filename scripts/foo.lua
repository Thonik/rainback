require "fritomod/currying";
require "rainback/Graphics";

require "fritomod/Events";

-- This still needs to be global until Frame-Layout can refer to this object
-- in a better way.
gfx = Rainback.Graphics:New();
Events.RENDER(gfx, "Render");
