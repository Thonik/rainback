print("hello!");

local FRITOMOD = "/home/dafrito/src/fritomod";
local TOP_SRCDIR = "../.."

Rainback.AddModuleDirectory(FRITOMOD .. "/fritomod", "fritomod/");
Rainback.AddModuleDirectory(FRITOMOD .. "/wow", "wow/");
Rainback.AddModuleDirectory(TOP_SRCDIR .. "/rainback", "rainback/");

-- Be sure to load the WoW stuff before scripts
Rainback.LoadDirectory(TOP_SRCDIR .. "/wow", true);

Rainback.LoadDirectory(TOP_SRCDIR .. "/scripts", true);
