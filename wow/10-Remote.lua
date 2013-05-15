require "fritomod/Remote";
require "fritomod/Strings";

function Remote:Install(channel)
    -- Do nothing, no need to listen for a specific channel
end;

function Remote:Send(channel, msg)
    error("No remote communication medium was configured");
end;

local function InitializeProtocol(socket)
    local protocol = Rainback.Network.pascalProtocol();
    protocol:listen(socket);

    protocol:connect("lineRead", function(data)
        local channel, msg = unpack(Strings.Split(":", data, 2));
        if msg == nil then
            print("Malformed data: " .. tostring(data));
            return;
        end;
        Remote:Dispatch(channel, msg, socket);
    end);

    return protocol;
end;

Callbacks = Callbacks or {};

local connected = false;
local callbacks = {};
function Callbacks.RemoteReady(func, ...)
    func = Curry(func, ...);
    if connected then
        return Callbacks.Later(func);
    end;
    return Lists.InsertFunction(callbacks, func);
end;

local clientProtocol;
function Rainback.LoadRemote(hostname, port)
    local socket = Rainback.Network.connectTCP(hostname, port);
    clientProtocol = InitializeProtocol(socket);

    function Remote:Send(channel, msg)
        clientProtocol:write(channel .. ":" .. msg);
    end;

    socket:connect("connected", function()
        connected = true;
        Lists.CallEach(callbacks);
        callbacks = {};
    end);

    socket:connect("disconnected", function()
        connected = false;
    end);
end;

local server;
local connections = {};
function Rainback.ServeRemote(port)
    if server then
        server:close();
        connections = {};
    end;
    server = Rainback.Network.serveTCP(port);

    server:connect("newConnection", function()
        local socket = server:nextPendingConnection();
        assert(socket, "Spurious newConnection");
        print("New connection from: " .. socket:address() ..":".. socket:port());

        local protocol = InitializeProtocol(socket);

        table.insert(connections, protocol);

        socket:connect("disconnected", function()
            print("Lost connection from: " .. socket:address() ..":".. socket:port());
            Lists.Remove(connections, protocol);
        end);
    end);
end;
