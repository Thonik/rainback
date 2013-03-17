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

local function getPositionFromAnchor(ref, anchor)
    local x, y, width, height = getBounds(ref);
    assert(x ~= nil, "Ref did not return useful bounds");

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

function AnchoredBound:Constructor()
    self.anchors = {};
end;

function AnchoredBound:SetPoint(anchor, ref, anchorTo, x, y)
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
    end;

    self.anchors[anchor] = {ref, anchorTo, x, y};
end;

function AnchoredBound:GetBounds()
    local left, top, right, bottom;

    for anchor, params in pairs(self.anchors) do
        local ref, anchorTo, offsetX, offsetY = unpack(params);
        local refX, refY = getPositionFromAnchor(ref, anchorTo);
        local refWidth, refHeight = select(3, getBounds(ref));
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
                left = refX + refWidth / 2 - self.width / 2 + offsetX;
                top = refY + refHeight / 2 - self.height / 2 + offsetY;
            end;
        end;
    end;

    local x, y, width, height;

    if left ~= nil then
        x = left;
    elseif right ~= nil and self.width then
        x = right - self.width;
    else
        return;
    end;

    if top ~= nil then
        y = top;
    elseif bottom ~= nil and self.height then
        y = bottom - self.height;
    else
        return;
    end;

    if self.width then
        width = self.width;
    elseif left ~= nil and right ~= nil then
        width = right - left;
        trace("Coercing width from left (" .. left .. ") and right (" .. right .. ") anchors");
    else
        return;
    end;

    if self.height then
        height = self.height;
    elseif top ~= nil and bottom ~= nil then
        height = bottom - top;
    else
        return;
    end;

    return x, y, width, height;
end;

function AnchoredBound:SetWidth(width)
    self.width = width;
end;

function AnchoredBound:SetHeight(height)
    self.height = height;
end;
