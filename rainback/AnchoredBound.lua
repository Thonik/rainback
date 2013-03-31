require "fritomod/OOP-Class";

Rainback = Rainback or {};

local AnchoredBound = OOP.Class();
Rainback.AnchoredBound = AnchoredBound;

local validAnchors = {
    "TOPLEFT",
    "TOP",
    "TOPRIGHT",
    "RIGHT",
    "BOTTOMRIGHT",
    "BOTTOM",
    "BOTTOMLEFT",
    "LEFT",
    "CENTER"
};
for i=1,#validAnchors do
    validAnchors[validAnchors[i]] = true;
end;

local convenienceAnchors = {
    TOPLEFT = {"TOP", "LEFT"},
    TOPRIGHT = {"TOP", "RIGHT"},
    BOTTOMLEFT = {"BOTTOM", "LEFT"},
    BOTTOMRIGHT = {"BOTTOM", "RIGHT"},
};

local function canonicalAnchorName(anchor)
    local originalName = anchor;
    anchor = tostring(anchor):upper();
    assert(validAnchors[anchor],
        "Anchor name must be valid. Given: '" .. originalName .. "'"
    );
    return anchor;
end;

local function getBounds(ref)
    if type(ref) == "function" then
        return ref();
    end;
    return ref:GetBounds();
end;

local function getPositionFromAnchor(anchor, x, y, width, height)
    if anchor == "TOPLEFT" then
        return x, y;
    elseif anchor == "TOP" then
        return x + width / 2, y;
    elseif anchor == "TOPRIGHT" then
        return x + width, y;
    elseif anchor == "RIGHT" then
        return x + width, y + height / 2;
    elseif anchor == "BOTTOMRIGHT" then
        return x + width, y + height;
    elseif anchor == "BOTTOM" then
        return x + width / 2, y + height;
    elseif anchor == "BOTTOMLEFT" then
        return x, y + height;
    elseif anchor == "LEFT" then
        return x, y + height / 2;
    elseif anchor == "CENTER" then
        return x + width / 2, y + height / 2;
    end;

    error("Invalid anchor name: " .. tostring(anchor));
end;

local boundId = 1;
function AnchoredBound:Constructor()
    self.anchors = {};
    self.name = "[AnchoredBound " .. boundId .. "]";
    boundId = boundId + 1;
end;

function AnchoredBound:SetPoint(anchor, ref, anchorTo, x, y)
    self.hasAnchors = true;

    anchor = canonicalAnchorName(anchor);
    anchorTo = canonicalAnchorName(anchorTo or anchor);

    x = x or 0;
    y = y or 0;

    if convenienceAnchors[anchor] then
        -- These anchors are corners, so they can be reduced to
        -- two separate calls to SetPoint
        for i=1, #convenienceAnchors[anchor] do
            self:SetPoint(convenienceAnchors[anchor][i], ref, anchorTo, x, y);
        end;
        return;
    end;

    trace("SetPoint", self, anchor, ref, anchorTo, x, y);
    self.anchors[anchor] = {ref, anchorTo, x, y};
end;

function AnchoredBound:HasBounds()
    return self.hasAnchors;
end;

local boundsCache;
function Rainback.ClearCache()
    boundsCache = setmetatable({}, {
        __mode = "k"
    });
end;
Rainback.ClearCache();

function Rainback.GetCached(bound)
    return boundsCache[bound];
end;

function Rainback.SetCache(bound, value)
    boundsCache[bound] = value;
end;

function AnchoredBound:GetBounds()
    if not self:HasBounds() then
        return;
    end;

    local cached = Rainback.GetCached(self);
    if cached then
        return unpack(cached);
    end;

    trace(tostring(self) .. " is calculating its bounds");

    local left, top, right, bottom;

    for anchor, params in pairs(self.anchors) do
        if not hasAnchors then
        end;
        local ref, anchorTo, offsetX, offsetY = unpack(params);
        local rx, ry, refWidth, refHeight = getBounds(ref);
        local refX, refY = getPositionFromAnchor(
            anchorTo, rx, ry, refWidth, refHeight
        );
        if anchor == "TOP" then
            top = refY + offsetY;
        elseif anchor == "LEFT" then
            left = refX + offsetX;
        elseif anchor == "RIGHT" then
            right = refX + offsetX;
        elseif anchor == "BOTTOM" then
            bottom = refY + offsetY;
        elseif anchor == "CENTER" then
            if self.width and self.height then
                left = refX - self.width / 2 + offsetX;
                top = refY - self.height / 2 + offsetY;
            end;
        end;
    end;

    local x, y, width, height;

    if left ~= nil then
        x = left;
    elseif right ~= nil and self.width then
        x = right - self.width;
    else
        print("No X");
        return;
    end;

    if top ~= nil then
        y = top;
    elseif bottom ~= nil and self.height then
        y = bottom - self.height;
    else
        print("No Y");
        return;
    end;

    if self.width then
        width = self.width;
    elseif left ~= nil and right ~= nil then
        width = right - left;
    else
        print("No width");
        return;
    end;

    if self.height then
        height = self.height;
    elseif top ~= nil and bottom ~= nil then
        height = bottom - top;
    else
        print("No height");
        return;
    end;

    Rainback.SetCache(self, {x, y, width, height});

    return x, y, width, height;
end;

function AnchoredBound:GetWidth()
    return self.width;
end;

function AnchoredBound:SetWidth(width)
    self.width = width;
end;

function AnchoredBound:GetHeight()
    return self.height;
end;

function AnchoredBound:SetHeight(height)
    self.height = height;
end;

function AnchoredBound:ToString()
    return self.name;
end;
