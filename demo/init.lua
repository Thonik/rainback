local HOME = "/home/dafrito"
local FRITOMOD = HOME .. "/src/fritomod";
local TMPDIR = HOME .. "/tmp";
local TOP_SRCDIR = "../.."

local globals = {};
globals.home = HOME;
globals.top_srcdir = TOP_SRCDIR;
globals.tmpdir = TMPDIR;
Rainback.Globals = globals;

Rainback.AddModuleDirectory(FRITOMOD .. "/fritomod", "fritomod/");
Rainback.AddModuleDirectory(FRITOMOD .. "/wow", "wow/");
Rainback.AddModuleDirectory(TOP_SRCDIR .. "/rainback", "rainback/");

Rainback.SetBackgroundColor(255, 255, 221);

-- Be sure to load the WoW stuff before scripts
Rainback.LoadDirectory(TOP_SRCDIR .. "/wow", true);

Rainback.LoadPersistence(TMPDIR .. "/persistence");

Callbacks.PersistValue("DEBUG_TRACE", function(value)
    DEBUG_TRACE = value;
    return function()
        return DEBUG_TRACE;
    end;
end);

Rainback.OnNamedEvent("RENDER", Rainback.Render);

-- Finally, load the scripts themselves
Callbacks.Later(Rainback.LoadDirectory, TOP_SRCDIR .. "/scripts", true);
