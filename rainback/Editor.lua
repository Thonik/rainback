require "fritomod/Strings";
require "fritomod/Lists";
require "fritomod/Frames-Position";
require "hack/Script";

UI = UI or {};
UI.Editor = OOP.Class();
local Editor = UI.Editor;

local WIDTH = 400;
local HEIGHT = 500;

function Editor:Constructor(parent)
	parent = Frames.AsRegion(parent);
	assert(parent, "Parent frame must be provided");
	assert(parent.CreateTexture, "Provided parent must be a real frame");

    self.editor = Rainback.TextEdit();
    self.editor.tabStopWidth = 30;
    self.editor.styleSheet = [[
        font-family: Inconsolata;
        font-size: 13px;
        color: black;
        border: 1px solid black;
        border-top: none;
    ]];

    self.title = Frames.New(parent, "Texture");
    Frames.WH(self.title, WIDTH, 20);
    Frames.Color(self.title, "grey");

    self.commandFrame = Frames.New(parent, "Widget");
    Anchors.VFlip(self.commandFrame, self.title, "bottomleft");
    Anchors.VFlip(self.commandFrame, self.title, "bottomright");

    self.command = Rainback.LineEdit();
    self.command.styleSheet = [[
        background: rgb(255, 255, 245);
        color: black;
        border: 1px solid grey;
        border-top: none;
        font-family: Inconsolata;
    ]];
    self.commandFrame:SetWidget(self.command);

    self.editorFrame = Frames.New(parent, "Widget");
    Frames.Height(self.editorFrame, HEIGHT);
    Anchors.VFlipFromTop(self.editorFrame, self.commandFrame);
    self.editorFrame:SetWidget(self.editor);

    local continual = Frames.Text(self.title, "default", 10);
    Frames.Color(continual, "white");
    continual:SetText("Autorun");

    local run = Frames.Text(self.title, "default", 10);
    Frames.Color(run, "white");
    run:SetText("Run");
    Callbacks.Click(run, self, "Run");

    local reset = Frames.Text(self.title, "default", 10);
    Frames.Color(reset, "white");
    reset:SetText("Reset");
    Callbacks.Click(reset, function()
        if self.script then
            self.script:Reset();
        end;
    end);

    Callbacks.Click(continual, function()
        if self.autorun then
            self.autorun = false;
            Frames.Color(self.title, "grey");
            return;
        end;
        Frames.Color(self.title, 0, .5, 0);
        self.autorun = true;
        self:Run();
    end);

    local links = {run, continual, reset};
    Anchors.Share(Anchors.HJustify("right", 6, links), "right", self.title, 2);

    local scriptName = Frames.Text(self.title, "default", 10);
    Frames.Color(scriptName, "white");
    scriptName:SetText("<untitled>");
    Anchors.Share(scriptName, self.title, "left", 3, 3);

    function Editor:SetName(name)
        scriptName:SetText(name or "<untitled>");
    end;

    self.commands = {};

    Anchors.VFlipFromTop(self.editorFrame, self.commandFrame);
    self.command:connect("returnPressed", function()
        local cmd = self.command.text;
        local remover = self:AddCommand(cmd);
        if not remover then
            return;
        end;
        self.command.text = "";
    end);
end;

function Editor:LayoutCommands()
    Anchors.Clear(self.editorFrame);
    if #self.commands == 0 then
        Anchors.VFlipFromTop(self.editorFrame, self.commandFrame);
        return;
    end;
    Anchors.Clear(self.commands);
    Anchors.VFlip(Anchors.VJustifyFrom("bottomleft", -1, self.commands), "bottomleft", self.commandFrame, -1);
    Anchors.VFlipFromTop(self.editorFrame, self.commands[#self.commands]);
end;

function Editor:BuildCommand(command)
    local asset = Frames.New(parent);
    Frames.Color(asset, "blue", .2);
    Frames.WH(asset, WIDTH, 20);

    local assetText = Frames.Text(asset, "Inconsolata", 10);
    assetText:SetText(command);
    Anchors.Share(assetText, "left", 5);

    local close = Frames.Text(asset, "default", 8);
    close:SetText("Remove");
    Anchors.Share(close, asset, "right", 3);
    Callbacks.Click(close, function()
        self.script:RemoveCommand(command);
        Frames.Destroy(asset);
        Lists.Remove(self.commands, asset);

        self:LayoutCommands();
    end);
    return asset;
end;

function Editor:AddCommand(command)
    if not self.script:AddCommand(command) then
        return false;
    end;

    local ui = self:BuildCommand(command);
    table.insert(self.commands, ui);

    self:LayoutCommands();
    return true;
end;

function Editor:Reset()
    if self.scriptRemover then
        self.scriptRemover();
        self.scriptRemover = nil;
    end;
    self.editor.plainText = "";
    self:SetName("<None>");
    if #self.commands then
        Frames.Destroy(self.commands);
    end;
    self.commands = {};
end;

function Editor:SetScript(script)
    self:Reset();
    self.script = script;
    if not self.script then
        return;
    end;
    self.editor.plainText = self.script:GetContent();

    local commands = self.script:GetCommands();
    for i=1, #commands do
        local ui = self:BuildCommand(commands[i]);
        table.insert(self.commands, ui);
    end;
    self:LayoutCommands();

    self.scriptRemover = Seal(Lists.CallEach, {
        self.editor:connect("textChanged", function()
            if self.script then
                self.script:SetContent(self.editor.plainText);
            end;
        end),
        self.script:OnChange(function()
            if self.autorun then
                self:Run();
            end;
            local content = self.script:GetContent();
            if self.editor.plainText == content then
                return;
            end;
            self.editor.plainText = content;
        end)
    });
end;

function Editor:GetScript()
    return self.script;
end;

function Editor:Run()
    local success, msg = xpcall(Curry(self.script, "Execute"), debug.traceback);
    if success then
        if self.autorun then
            Frames.Color(self.title, 0, .5, 0);
        else
            Frames.Color(self.title, "grey");
        end;
    end;
    if not success then
        Frames.Color(self.title, "orange");
        print(msg);
    end;
end;

function Editor:Handle()
    return self.title;
end;

function Editor:Destroy()
    self:Reset();
    Frames.Destroy(
        self.title,
        self.editorFrame
    );
    Editor.super.Destroy(self);
end;
