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

-- Load the WoW stuff before scripts
Rainback.LoadDirectory(SRCDIR .. "/wow", true);
assert(Rainback.CreateDirectory(PERSISTENCE_DIR),
    "Persistence directory could not be created"
);
Rainback.LoadPersistence(PERSISTENCE_DIR .. "/persistence");

Callbacks.PersistValue("DEBUG_TRACE", function(value)
    DEBUG_TRACE = value;
    return function()
        return DEBUG_TRACE;
    end;
end);

local profiles = {};

if not globals.remotePort then
    globals.remotePort = 28123;
end;
if not globals.remoteHostname then
    globals.remoteHostname = "dafrito.com";
end;

function profiles:client()
    print("Running Rainback client");
    Rainback.SetBackgroundColor(255, 255, 221);
    Rainback.OnNamedEvent("RENDER", Rainback.Render);
    Rainback.LoadRemote(globals.remoteHostname, globals.remotePort);
    Callbacks.Later(Rainback.LoadDirectory, SRCDIR .. "/scripts", true);
end;

function profiles:server()
    print("Running Rainback server on port " .. globals.remotePort);
    Rainback.ServeRemote(globals.remotePort);

    Remote["rainback.control"](function(msg, who)
        msg = msg:lower();
        if msg == "quit" or msg == "exit" or msg == "close" then
            Rainback.Exit();
        elseif msg == "debug on" then
            DEBUG_TRACE = true;
        elseif msg == "debug off" then
            DEBUG_TRACE = false;
        elseif msg:grep("^run") then
            local _, cmd = unpack(Strings.Split(" ", msg, 2));
            local loadstring = loadstring or load;
            local func = assert(loadstring(cmd));
            func();
        end;
    end);

    Callbacks.Later(Rainback.LoadDirectory, SRCDIR .. "/server", true);
end;

profiles[globals.profile]();

require "fritomod/Callbacks-Timing";

-- Flush any callbacks that we have lingering
Timing.Flush();
