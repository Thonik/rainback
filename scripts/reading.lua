require "fritomod/Strings";
require "fritomod/Lists";

local globals = Rainback.Globals;

local data = Rainback.ReadFile(globals.tmpdir .. "/persistence");

local lines = Lists.Map(Strings.Split("\n", data), function(line)
    local text = Frames.Text(UIParent, "inconsolata", 10, "black");
    text:SetText(line);
    return text;
end);

Anchors.Share(Anchors.VJustify("topright", 0, lines), "topright");
