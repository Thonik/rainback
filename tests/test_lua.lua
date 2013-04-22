local FRITOMOD = "/home/dafrito/src/fritomod";
local TOP_SRCDIR = "../.."
local SRCDIR = TOP_SRCDIR .. "/tests"

Rainback.AddModuleDirectory(FRITOMOD .. "/fritomod", "fritomod/");
Rainback.AddModuleDirectory(FRITOMOD .. "/wow", "wow/");
Rainback.AddModuleDirectory(TOP_SRCDIR .. "/rainback", "rainback/");

require "fritomod/CreateTestSuite";
Rainback.LoadDirectory(SRCDIR .. "/rainback", true);

return dofile(SRCDIR .. "/testrunner.lua");
