require "fritomod/Strings";
require "fritomod/Lists";
require "fritomod/Serializers-Data";
require "hack/Toolbox";
require "hack/Script";
require "rainback/ScriptPage";

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

local function RenamePage(page, newName)
    if PageForName(newName) then
        print("Refusing to overwrite a name in use: " .. tostring(newName));
        return;
    end;
    page:SetName(newName);
    toolbox:RenameScript(page:GetName(), newName);
end;

local function DeletePage(page)
    Lists.Remove(pages, page);
    toolbox:RemoveScript(page:GetName());
end;

Events.CLOSE(function()
    Lists.Each(pages, "Reset");
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

local function Receive(func, ...)
    func = Curry(func, ...);
    Remote["rainback.Editor"](function(msg)
        func(Serializers.ReadData(msg));
    end);
end;

Receive(function(command, name, ...)
    if command == "Publish" then
        print("Received published script: " .. name);
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
        -- TODO Handle commands
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
