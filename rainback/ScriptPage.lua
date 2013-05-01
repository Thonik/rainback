require "fritomod/OOP-Class";
require "fritomod/ListenerList";
require "hack/Script";

Hack = Hack or {};
Hack.ScriptPage = OOP.Class();
local ScriptPage = Hack.ScriptPage;

function ScriptPage:Constructor(script)
    self.script = script;
    self.script:OnChange(self, "FireUpdate");

    self.listeners = ListenerList:New();

    self.commands = {};
    self.commandRemovers = {};
end;

function ScriptPage:GetName()
    return self.name;
end;

function ScriptPage:SetName(name)
    self.name = name;
    self:FireUpdate()
end;

function ScriptPage:SetToolbox(toolbox)
    self.toolbox = toolbox;
    self:FireUpdate()
end;

function ScriptPage:GetScript()
    return self.script;
end;

function ScriptPage:GetContent()
    return self:GetScript():GetContent();
end;

function ScriptPage:SetContent(content)
    return self:GetScript():SetContent(content);
end;

function ScriptPage:SetCommandParser(parser, ...)
    self.parser = Curry(parser, ...);
end;

function ScriptPage:AddCommand(command)
    if Lists.ContainsValue(self.commands, command) then
        return;
    end;
    self.commandRemovers[command] = self.parser(self, command);
    table.insert(self.commands, command);
    self:FireUpdate();
    return Functions.OnlyOnce(self, "RemoveCommand", command);
end;

function ScriptPage:RemoveCommand(command)
    local r = self.commandRemovers[command];
    if r then
        r();
        self.commandRemovers[command] = nil;
    end;
    Lists.Remove(self.commands, command);
    self:FireUpdate();
end;

function ScriptPage:GetCommands()
    return self.commands;
end;

function ScriptPage:Run(...)
    self:Reset();
    if not self.environment then
        self.environment = LuaEnvironment:New();
    end;
    return self.script:Execute(self.environment, ...);
end;

function ScriptPage:Reset()
    if self.environment then
        self.environment:Destroy();
        self.environment = nil;
    end;
end;

function ScriptPage:OnChange(func, ...)
    return self.listeners:Add(func, ...);
end;

function ScriptPage:FireUpdate()
    self.listeners:Fire();
end;
