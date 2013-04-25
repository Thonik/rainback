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

function AnchoredBound:GetAnchors()
    return self.anchors;
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

function AnchoredBound:Clear()
    Rainback.Update();
    self.anchors = {};
    self.hasAnchors = false;
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

    -- These center values are used as fallbacks if absolute values cannot be determined
    local centerX, centerY;

    for anchor, params in pairs(self.anchors) do
        local ref, anchorTo, offsetX, offsetY = unpack(params);
        trace("Working with " .. anchor .. ":" .. anchorTo .. " " .. offsetX .. ", " .. offsetY);
        local rx, ry, refWidth, refHeight = getBounds(ref);
        if rx ~= nil then
            local refX, refY = getPositionFromAnchor(
                anchorTo, rx, ry, refWidth, refHeight
            );
            if anchor == "TOP" then
                top = refY + offsetY;
                centerX = refX;
            elseif anchor == "LEFT" then
                left = refX + offsetX;
                centerY = refY;
            elseif anchor == "RIGHT" then
                right = refX + offsetX;
                centerY = refY;
            elseif anchor == "BOTTOM" then
                bottom = refY + offsetY;
                centerX = refX;
            elseif anchor == "CENTER" then
                centerX = refX + offsetX;
                centerY = refY + offsetY;
            end;
        else
            trace("Reference '" .. tostring(ref) .. "' has no bounds, ignoring '" .. anchor .. "' anchor");
        end;
    end;

    local x, y, width, height = left, top, self.width, self.height;

    if width == nil and left ~= nil and right ~= nil then
        width = right - left;
    end;

    if height == nil and top ~= nil and bottom ~= nil then
        height = bottom - top;
    end;

    if width == nil then
        width = self:GetNaturalWidth(height);
    end;

    if height == nil then
        height = self:GetNaturalHeight(width);
    end;

    if width == nil or height == nil then
        trace("Dimensions (w:" .. tostring(width) .. ", h:" .. tostring(height) ..") " ..
            "could not be deduced for bound '" .. tostring(self) .. "'");
        return;
    end;

    if x == nil then
        if right ~= nil then
            x = right - width;
        elseif centerX then
            x = centerX - width / 2;
        end;
    end;

    if y == nil then
        if bottom ~= nil then
            y = bottom - height;
        elseif centerY then
            y = centerY - height / 2;
        end;
    end;

    if x == nil or y == nil then
        trace("Position (x:" .. tostring(x) .. ", y:" .. tostring(y) .. ") " ..
            "could not be deduced for bound '" .. tostring(self) .. "'");
        return;
    end;

    Rainback.SetCache(self, {x, y, width, height});

    trace(tostring(self) .. " finished calculated bounds. "
        .. "(x: " .. x .. ", y: " .. y .. ", width: " .. width .. ", height: " .. height .. ")");

    return x, y, width, height;
end;

function AnchoredBound:GetExtents()
    return self:GetLeft(),
        self:GetBottom(),
        self:GetRight(),
        self:GetTop();
end;

function AnchoredBound:GetWidth()
    return self.width;
end;

function AnchoredBound:SetWidth(width)
    self.width = width;
end;

function AnchoredBound:GetNaturalWidth(height)
    -- Feel free to implement this method for bounds that can provide natural dimensions
end;

function AnchoredBound:GetNaturalHeight(width)
    -- Feel free to implement this method for bounds that can provide natural dimensions
end;

function AnchoredBound:GetHeight()
    return self.height;
end;

function AnchoredBound:SetHeight(height)
    self.height = height;
end;

function AnchoredBound:GetCenter()
    local x, y, width, height = self:GetBounds();
    return x + width / 2, y + height / 2;
end;

function AnchoredBound:GetLeft()
    local x, y, width, height = self:GetBounds();
    return x;
end;

function AnchoredBound:GetRight()
    local x, y, width, height = self:GetBounds();
    return x + width;
end;

function AnchoredBound:GetTop()
    local x, y, width, height = self:GetBounds();
    return y;
end;

function AnchoredBound:GetBottom()
    local x, y, width, height = self:GetBounds();
    return y + height;
end;

function AnchoredBound:ToString()
    return self.name;
end;
