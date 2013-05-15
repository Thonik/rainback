require "fritomod/Strings";
require "fritomod/Lists";
require "fritomod/Frames-Position";
require "fritomod/Mapper";
require "fritomod/UI-List";
require "rainback/Editor";
require "fritomod/Serializers-Data";
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

local mapper = Mapper:New();

local function RenamePage(page, newName)
    if PageForName(newName) then
        print("Refusing to overwrite a name in use: " .. tostring(newName));
        return;
    end;
    page:SetName(newName);
    toolbox:RenameScript(page:GetName(), newName);
    mapper:Update();
end;

local function DeletePage(page)
    if editor:GetPage() == page then
        -- We're currently viewing this page, so try to find another page to look at
        if #pages == 1 then
            -- No pages left, so just view nothing
            editor:Clear();
        else
            local i = Lists.KeyFor(pages, page);
            if i == #pages then
                editor:SetPage(pages[i - 1]);
            else
                editor:SetPage(pages[i + 1]);
            end;
        end;
    end;
    Lists.Remove(pages, page);
    toolbox:RemoveScript(page:GetName());
    mapper:Update();
end;

Events.CLOSE(function()
    Lists.Each(pages, "Reset");
end);

toolbox:OnUpdate(function()
    if editor:GetPage() ~= nil then
        return;
    end;
    local first = next(toolbox:GetScripts());
    if first == nil then
        return;
    end;
    editor:SetPage(PageForScript(first));
end);

local function Parser(page, command)
    local script = page:GetScript();

    if command == "delete" then
        DeletePage(page);
        return true;
    end;
    local match, newName = command:grep([[^rename\s+(.+)$]]);
    if match then
        RenamePage(page, newName);
        return true;
    end;
    local match, name = command:grep([[^use\s+(.+)$]]);
    if match then
        return script:AddConnector(Hack.Connectors.Use(function(env, dtor)
            local usedScript = toolbox:GetScript(name);
            assert(usedScript, "No script was available with name '" .. tostring(name) .. "'");
            dtor(usedScript:OnCompilingUpdate(script, "FireUpdate"));

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
            env:AddDestructor(assetScript:OnCompilingUpdate(script, "FireUpdate"));
            return env:Change(globalName, assetScript:Execute(env));
        end);
    end;
    local match, name, default = command:grep(
        [[^param\s+([^\s]+)(?:\s*=\s*([^\s].*))?$]]
    );
    if match then
        print(name, default);
        return true;
    end;
    if command == "autorun" then
        return script:AddConnector(function(env)
            if env:Metadata().mainScript ~= script then
                return;
            end;
            env:AddDestructor(script:OnCompilingUpdate(function()
                local success, msg = xpcall(Curry(page.Run, page), traceback);
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
        if not metadata.mainScript then
            metadata.mainScript = script;
            metadata.name = toolbox:NameFor(script);
        end;
    end));
    script:AddConnector(Hack.Connectors.Global("Undoer", Hack.Assets.Undoer()));
    script:SetContent(content);

    return script;
end;

local function NewPage(name, content)
    local script = NewScript(content);

    local page = Hack.ScriptPage:New(script);
    page:SetName(name);
    page:SetCommandParser(Parser);

    toolbox:AddScript(name, script);
    table.insert(pages, page);

    return page;
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

function mapper:CanReuseContent(text, page)
    text:SetText(page:GetName());
    return true;
end;

mapper:SetMapper(function(page)
    local text = Frames.Text(UIParent, "default", 10);
    text:SetText(page:GetName());
    Callbacks.Click(text, function()
        editor:SetPage(page);
    end);
    return text;
end);

mapper:AddSourceList(pages);

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

local function Receive(func, ...)
    func = Curry(func, ...);
    Remote["rainback.Editor"](function(msg)
        func(Serializers.ReadData(msg));
    end);
end;

Receive(function(command, name, ...)
    if command == "Publish" then
        local page = PageForName(name);
        if not page then
            page = NewPage(name);
            page:Sync();
        end;

        if not page:IsSynced() then
            return;
        end;

        local content, commands = ...;
        page:SetContent(content);

        if commands then
            self:RemoveAllCommands();
            Lists.Each(commands, page, "AddCommand");
        end;
    end;
end);

Callbacks.PersistentValue("Toolbox", function(pagesData)
    pagesData = pagesData or {};
    Lists.Each(pagesData, function(pageData)
        local page = NewPage(pageData.name, pageData.text);

        if pageData.sync then
            page:Sync();
        end;

        if pageData.commands then
            Lists.Each(pageData.commands, page, "AddCommand");
        end;
    end);
    mapper:Update();
    return function()
        return Lists.Map(pages, function(page)
            return {
                name = page:GetName(),
                text = page:GetContent(),
                commands = page:GetCommands(),
                sync = page:IsSynced()
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

function Slash.scr(cmd)
    local cmd, name = unpack(Strings.Split(" ", cmd, 2));
    if cmd == "new" or cmd == "add" then
        local page = NewPage(name);
        editor:SetPage(page);
        mapper:Update();
    elseif cmd == "rename" or cmd == "mv" then
        local old, new = unpack(Strings.Split(" ", name));
        if not old or not new then
            return;
        end;
        if old == new then
            -- Nothing to do, so just return
            return;
        end;
        local page = PageForName(old);
        if not page then
            print("No page with name: " .. tostring(old));
            return;
        end;
    elseif cmd == "del" or cmd == "rm" then
        local page = PageForName(name);
        if not page then
            return;
        end;
        DeletePage(page);
    end;
end;
