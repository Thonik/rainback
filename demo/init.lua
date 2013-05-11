local globals = Rainback.globals;

-- The location of Fritomod
-- Example: /home/dafrito/src/fritomod"
local FRITOMOD = globals.fritomod;
if not FRITOMOD then
	error("Rainback.globals.fritomod must be defined");
end;

-- The directory that will contain persisted scripts, data, etc.
-- Example: /home/dafrito/etc/rainback"
local PERSISTENCE_DIR = globals.persistenceDir;
if not PERSISTENCE_DIR then
	error("Rainback.globals.persistenceDir must be defined");
end;

-- srcdir is provided automatically, so there's no need to provide
-- it in settings.
local SRCDIR = globals.srcdir;

Rainback.AddModuleDirectory(FRITOMOD .. "/fritomod", "fritomod/");
Rainback.AddModuleDirectory(FRITOMOD .. "/hack", "hack/");
Rainback.AddModuleDirectory(FRITOMOD .. "/wow", "wow/");
Rainback.AddModuleDirectory(SRCDIR .. "/rainback", "rainback/");

Rainback.SetBackgroundColor(255, 255, 221);

-- Load the WoW stuff before scripts
Rainback.LoadDirectory(SRCDIR .. "/wow", true);

Rainback.LoadPersistence(PERSISTENCE_DIR .. "/persistence");

Callbacks.PersistValue("DEBUG_TRACE", function(value)
    DEBUG_TRACE = value;
    return function()
        return DEBUG_TRACE;
    end;
end);

Rainback.OnNamedEvent("RENDER", Rainback.Render);

-- Finally, load the scripts themselves
Callbacks.Later(Rainback.LoadDirectory, SRCDIR .. "/scripts", true);

require "fritomod/Callbacks-Timing";

-- Flush any callbacks that we have lingering
Timing.Flush();
