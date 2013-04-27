require "wow/Texture";
require "fritomod/OOP-Class";

local Delegate = OOP.Class();

function Delegate:SetTexture(...)
    if select("#", ...) > 1 then
        self.color = {...};
        Rainback.Update();
        return;
    end;
    trace("STUB SetTexture");
end;

function Delegate:GetTexture()
    if self.color then
        return self:GetColor();
    end;
end;

function Delegate:GetColor()
    if self.color then
        return unpack(self.color);
    end;
end;


WoW.SetFrameDelegate("Texture", "texture", Delegate, "New");

