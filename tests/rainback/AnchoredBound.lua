local Suite = CreateTestSuite("rainback.AnchoredBound");

local function directBound()
    return 25, 50, 100, 150;
end;

function Suite:TestAnchoredBound()
    local bound = Rainback.AnchoredBound:New();

    bound:SetPoint("topleft", directBound);
end;

function Suite:TestAnchoredBoundThrowsOnBadNames()
    local bound = Rainback.AnchoredBound:New();
    Assert.Throws(bound, "SetPoint", "not a valid anchor name");
end;

function Suite:TestAnchoredBoundReturnsAUsefulBound()
    local bound = Rainback.AnchoredBound:New();
    bound:SetPoint("topleft", directBound);
    bound:SetWidth(50);
    bound:SetHeight(50);

    local x, y, width, height = bound:GetBounds();
    Assert.Equals(25, x);
    Assert.Equals(50, y);
end;

function Suite:TestAnchoredBoundWithFlippedAnchor()
    local bound = Rainback.AnchoredBound:New();
    bound:SetPoint("topleft", directBound, "topright");
    bound:SetWidth(50);
    bound:SetHeight(50);

    local x, y, width, height = bound:GetBounds();
    Assert.Equals(125, x);
    Assert.Equals(50, y);
end;

function Suite:TestAnchoredBoundWithImplicitWidth()
    local bound = Rainback.AnchoredBound:New();
    bound:SetPoint("topleft", directBound);
    bound:SetPoint("topright", directBound);
    bound:SetHeight(30);

    local x, y, width, height = bound:GetBounds();
    Assert.Equals(25, x);
    Assert.Equals(50, y);
    Assert.Equals(100, width);
    Assert.Equals(30, height);
end;

function Suite:TestImplicitWidthAndHeight()
    local bound = Rainback.AnchoredBound:New();
    bound:SetPoint("topleft", directBound);
    bound:SetPoint("bottomright", directBound);

    local x, y, width, height = bound:GetBounds();
    Assert.Equals(25, x);
    Assert.Equals(50, y);
    Assert.Equals(100, width);
    Assert.Equals(150, height);
end;
