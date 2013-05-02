Pipe = Pipe or {};

function Pipe.File()
    local savedContent;
    local watcher;
    return function(path, pipeline)
        watcher = Rainback.WatchFile(path);
        local content = Rainback.ReadFile(path);
        pipeline:AddCleaner(watcher:connect("fileChanged", function()
            local content = Rainback.ReadFile(path);
            if content ~= savedContent then
                pipeline:Update();
            end;
        end));
        return content, function(content)
            savedContent = content;
            Rainback.WriteFile(path, content);
        end;
    end;
end;
