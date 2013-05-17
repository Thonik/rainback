require "fritomod/Persistence";
require "fritomod/Serializers-Source";
require "rainback/Events";

local persisted = {};

function Rainback.LoadPersistence(persistenceFile)
    if Rainback.FileExists(persistenceFile) then
        persisted = Serializers.ReadSource(Rainback.ReadFile(persistenceFile)) or {};
    end;
    Persistence.Load(persisted);

    local index;
    local indexFile = persistenceFile .. ".index";

    local MAX_ENTRIES = 9;
    local AUTOSAVE_INTERVAL = "3m";

    local function GetIndex()
        if index ~= nil then
            return index;
        end;
        if not Rainback.FileExists(indexFile) then
            index = 1;
            return index;
        end;
        local indexData = Rainback.ReadFile(indexFile);
        index = tonumber(indexData);
        if index == nil then
            printf("Failed to read index %q", indexData);
            index = 1;
        end;
        return index;
    end;

    local function WriteIndex()
        Rainback.WriteFile(indexFile, index);
    end;

    local function BumpIndex()
        index = index + 1;
        if index > MAX_ENTRIES then
            index = 1;
        end;
        WriteIndex();
    end;

    function Rainback.Autosave()
        Persistence.Save();
        local persistedData = Serializers.WriteSource(persisted);

        local oldFilename = persistenceFile .. "." .. GetIndex();
        local savedData;
        if Rainback.FileExists(oldFilename) then
            savedData = Rainback.ReadFile(oldFilename);
            if savedData == persistedData then
                -- Nothing changed, so nothing to save.
                return;
            end;
            BumpIndex();
        else
            WriteIndex();
        end;

        print("Autosaving");
        Rainback.WriteFile(persistenceFile .. "." .. GetIndex(), persistedData);
    end;
    local remover = Timing.Every(AUTOSAVE_INTERVAL, function()
        local success, msg = xpcall(Rainback.Autosave, traceback);
        if not success then
            print("WARNING: Autosave failed: ".. msg);
        end;
    end);

    function Rainback.Save()
        trace("Saving persistence...");
        Persistence.Save();
        Rainback.WriteFile(persistenceFile, Serializers.WriteSource(persisted));
        trace("Persistence saved");
    end;

    Rainback.OnNamedEvent("CLOSE", function()
        Rainback.Save();
        remover();
    end);
end;
