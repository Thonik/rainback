require "fritomod/Persistence";
require "fritomod/Serializers-Source";
require "rainback/Events";

local persisted = {};

function Rainback.LoadPersistence(persistenceFile)
    if Rainback.FileExists(persistenceFile) then
        persisted = Serializers.ReadSource(Rainback.ReadFile(persistenceFile));
    end;
    Persistence.Load(persisted);

    Rainback.OnNamedEvent("CLOSE", function()
        Persistence.Save();
        Rainback.WriteFile(persistenceFile, Serializers.WriteSource(persisted));
    end);
end;
