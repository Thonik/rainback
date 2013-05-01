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
local pages = {};

function PageForName(name)
    for i=1, #pages do
        local page = pages[i];
        if page:GetName() == name then
            return page;
        end;
    end;
end;

function PageForScript(script)
    for i=1, #pages do
        local page = pages[i];
        if page:GetScript() == script then
            return page;
        end;
    end;
end;

Events.CLOSE(function()
    Lists.Each(pages, "Destroy");
end);

--[[toolbox:OnChange(function()
    if editor:Get() ~= nil then
        return;
    end;
    local first = next(toolbox:GetScripts());
    if first == nil then
        return;
    end;
    editor:SetScript(toolbox:GetScript(first));
    editor:SetName(first);
end);]]

local function Parser(page, command)
    local script = page:GetScript();

    local match, name = command:grep([[^use\s+(.+)$]]);
    if match then
        return script:AddConnector(Hack.Connectors.Use(function(env, dtor)
            local usedScript = toolbox:GetScript(name);
            assert(usedScript, "No script was available with name '" .. tostring(name) .. "'");
            dtor(usedScript:OnUpdate(script, "FireUpdate"));

            -- Pass env twice, once for the environment where the script
            -- is ran, and again to provide it as a parameter to the invoked script.
            return usedScript:Execute(env, env, dtor);
        end));
    end;
    local match, assetName, globalName = command:grep(
        [[^asset\s+([^\s]+)\s+([^\s]+)$]]
    );
    if match then
        return script:AddConnector(function(env)
            local assetScript = toolbox:GetScript(assetName);
            assert(assetScript, "No asset was available with name '" .. tostring(assetName) .. "'");
            env:AddDestructor(assetScript:OnUpdate(script, "FireUpdate"));
            return env:Change(globalName, assetScript:Execute(env));
        end);
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
            env:AddDestructor(page:OnUpdate(function()
                local success, msg = xpcall(Curry(page.Run, page), debug.traceback);
                if not success then
                    print(msg);
                end;
            end));
        end);
    end;
end;

local function NewScript(content)
    local script = Hack.Script:New();
    script:AddConnector("Set", "require", require);
    script:AddConnector(Hack.Connectors.Metadata(function(metadata)
        if not metadata.name then
            metadata.name = toolbox:NameFor(script);
        end;
    end));
    script:SetContent(content);

    return script;
end;

local listHeader = Frames.New();
Frames.WH(listHeader, 100, 20);
Frames.Color(listHeader, "grey");
Anchors.HFlip(listHeader, handle, "topleft", -1);

local listBG = Frames.New();
Frames.Width(listBG, 100);
Frames.Color(listBG, "white", .5);
Anchors.VFlip(listBG, listHeader, "bottomleft", -1);
Anchors.HFlip(listBG, editor.editorFrame, "bottomleft", -1);

local mapper = Mapper:New(function(page)
    local text = Frames.Text(UIParent, "default", 10);
    text:SetText(page:GetName());
    Callbacks.Click(text, function()
        editor:SetPage(page);
    end);
    return text;
end);

local frames = {};
mapper:AddDestination(frames);

mapper:AddDestination(function(name, frame)
    if frame == nil then
        Frames.Destroy(frames[name]);
    end;
end);
toolbox:OnUpdate(mapper, "Update");

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

Callbacks.PersistentValue("Toolbox", function(pagesData)
    pagesData = pagesData or {};
    pages = Lists.Map(pagesData, function(pageData)
        local script = NewScript(pageData.text);

        local page = Hack.ScriptPage:New(script);
        page:SetName(pageData.name);
        page:SetCommandParser(Parser);

        page:SetToolbox(toolbox);
        toolbox:AddScript(page:GetName(), script);

        if pageData.commands then
            Lists.Each(pageData.commands, page, "AddCommand");
        end;

        return page;
    end);
    mapper:AddSourceList(pages);
    return function()
        return Lists.Map(pages, function(page)
            return {
                name = page:GetName(),
                text = page:GetContent(),
                commands = page:GetCommands(),
            };
        end);
    end;
end);

Callbacks.PersistentValue("Toolbox-Selection", function(selection)
    if selection ~= nil then
        local page = PageForName(selection);
        if page then
            editor:SetPage(page);
        end;
    end;
    return function()
        local page = editor:GetPage();
        if page then
            return page:GetName();
        end;
    end;
end);

--[[Slash.scr = function(cmd)
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
end;]]
