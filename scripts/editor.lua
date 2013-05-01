require "fritomod/Strings";
require "fritomod/Lists";
require "fritomod/Frames-Position";
require "fritomod/Mapper";
require "fritomod/UI-List";
require "rainback/Editor";
require "hack/Toolbox";

local editor = UI.Editor:New(UIParent);
local handle = editor:Handle();
Frames.Position(handle, "EditorFrame", Anchors.Center);
Frames.Draggable(handle);

local toolbox = Hack.Toolbox:New();

Events.CLOSE(function()
    Lists.Each(toolbox:GetScripts(), "Reset");
end);

toolbox:OnChange(function()
    if editor:GetScript() ~= nil then
        return;
    end;
    local first = next(toolbox:GetScripts());
    if first == nil then
        return;
    end;
    editor:SetScript(toolbox:GetScript(first));
    editor:SetName(first);
end);

local function Parser(script, command)
    local match, name = command:grep([[^use\s+(.+)$]]);
    if match then
        return script:AddConnector(Hack.Connectors.Use(function(env, dtor)
            local usedScript = toolbox:GetScript(name);
            assert(usedScript, "No script was available with name '" .. tostring(name) .. "'");
            dtor(usedScript:OnChange(script, "FireUpdate"));
            return usedScript:Execute(env, dtor);
        end));
    end;
    local match, assetName, globalName = command:grep(
        [[^asset\s+([^\s]+)\s+([^\s]+)$]]
    );
    if match then
        return script:AddConnector(Hack.Connectors.Global(globalName, function(dtor)
            local assetScript = toolbox:GetScript(assetName);
            assert(assetScript, "No asset was available with name '" .. tostring(assetName) .. "'");
            dtor(assetScript, "Reset");
            dtor(assetScript:OnChange(script, "FireUpdate"));
            local asset = assetScript:Execute();
            return asset;
        end));
    end;
    local match, name, default = command:grep(
        [[^param\s+([^\s]+)(?:\s*=\s*([^\s].*))?$]]
    );
    if match then
        print(name, default);
        return Noop;
    end;
    local match = command:grep(
        [[^autorun$]]
    );
    if match then
        return script:AddConnector(function(env)
            print("Listening for change events");
            env:AddDestructor(script:OnChange(function()
                print("Running script due to autorun");
                local success, msg = xpcall(Curry(script.Execute, script), debug.traceback);
                if not success then
                    print(msg);
                end;
            end));
        end);
    end;
end;

local function NewScript(content, commands)
    local script = Hack.Script:New();
    script:AddConnector("Set", "require", require);
    script:AddConnector(Hack.Connectors.Metadata(function(metadata)
        metadata.name = toolbox:NameFor(script);
    end));
    script:SetContent(content);
    script:SetCommandParser(Parser);

    if commands then
        Lists.Each(commands, script, "AddCommand");
    end;

    return script;
end;

Callbacks.PersistentValue("Toolbox", function(toolboxData)
    if not toolboxData then
        toolboxData = {
            Demo = Persistence.EditorText
        };
    end;
    for name, content in pairs(toolboxData) do
        toolbox:AddScript(name, NewScript(content.text, content.commands));
    end;
    return function()
        local toolboxData = {};
        for name, script in pairs(toolbox:GetScripts()) do
            toolboxData[name] = {
                text = script:GetContent(),
                commands = script:GetCommands()
            };
        end;
        return toolboxData;
    end;
end);

Callbacks.PersistentValue("Toolbox-Selection", function(selection)
    if selection ~= nil then
        local script = toolbox:GetScript(selection);
        if script then
            editor:SetScript(script);
            editor:SetName(toolbox:NameFor(script));
        end;
    end;
    return function()
        local script = editor:GetScript();
        if script then
            return toolbox:NameFor(script);
        end;
    end;
end);

local listHeader = Frames.New();
Frames.WH(listHeader, 100, 20);
Frames.Color(listHeader, "grey");
Anchors.HFlip(listHeader, handle, "topleft", -1);

local listBG = Frames.New();
Frames.Width(listBG, 100);
Frames.Color(listBG, "white", .5);
Anchors.VFlip(listBG, listHeader, "bottomleft", -1);
Anchors.HFlip(listBG, editor.editorFrame, "bottomleft", -1);

local mapper = Mapper:New(function(script)
    local text = Frames.Text(UIParent, "default", 10);
    text:SetText(toolbox:NameFor(script));
    Callbacks.Click(text, function()
        editor:SetScript(script);
        editor:SetName(toolbox:NameFor(script));
    end);
    return text;
end);
mapper:AddSourceTable(toolbox:GetScripts());

local frames = {};
mapper:AddDestination(frames);

mapper:AddDestination(function(name, frame)
    if frame == nil then
        Frames.Destroy(frames[name]);
    end;
end);
toolbox:OnChange(mapper, "Update");

local list = UI.List:New();
list:SetLayout(Anchors.VJustify, "topleft");
list:SetContent(frames);
list:OnUpdate(function(ref)
    if not ref then
        return;
    end;
    Anchors.Share(ref, listBG, "topleft", 3);
end);
mapper:OnUpdate(list, "Update");

Slash.scr = function(cmd)
    local cmd, name = unpack(Strings.Split(" ", cmd, 2));
    if cmd == "new" then
        local script = NewScript();
        toolbox:AddScript(name, script);
        editor:SetScript(script);
        editor:SetName(toolbox:NameFor(script));
    elseif cmd == "rename" or cmd == "mv" then
        local old, new = unpack(Strings.Split(" ", name));
        toolbox:RenameScript(old, new);
        frames[new]:SetText(new);
    elseif cmd == "del" or cmd == "rm" then
        toolbox:RemoveScript(name);
    end;
end;
