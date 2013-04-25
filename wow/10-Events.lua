require "fritomod/Events";
require "fritomod/OOP-Class";

require "rainback/Events";

local Delegate = OOP.Class();

function Delegate:Constructor()
    Rainback.OnEvent(Events.Dispatch);
end;

function Delegate:RegisterEvent(event)
    -- Do nothing; all events are implicitly registered
end;

function Delegate:UnregisterEvent(event)
    -- Do nothing; all events are implicitly registered
end;

Events.delegate = Functions.OnlyOnce(Seal(Delegate, "New"));
