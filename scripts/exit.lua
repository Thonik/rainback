require "fritomod/Events";

local focused = false;
Events.FOCUSIN(function()
    focused = true;
end);

Events.FOCUSOUT(function()
    focused = false;
end);

Events.KEYPRESS(function(key)
    if focused and key == "ESCAPE" then
        Rainback.Close();
    end;
end);
