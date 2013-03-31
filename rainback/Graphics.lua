require "fritomod/OOP-Class";
require "fritomod/Lists";
require "rainback/AnchoredBound";

Rainback = Rainback or {};

local Graphics = OOP.Class();
Rainback.Graphics = Graphics;

function Graphics:Constructor()
    self.delegates = {};
end;

function Graphics:HasBounds()
    return true;
end;

function Graphics:GetBounds()
    return 0, 0, self.painter:width(), self.painter:height();
end;

function Graphics:RenderText(painter)
    painter:setFontSize(10);
    painter:setFontFamily("Inconsolata");
    painter:setPenColor(0, 0, 0);
    painter:position(5, 14);
    painter:setFontWeight(52);
end;

function Graphics:RenderLines(painter)
    local alpha = 50;
    painter:setPenAlpha(alpha);
    painter:setFillAlpha(alpha);

    for i=1, 50 do
        painter:setPenColor(
            math.random(0, 255),
            255,
            0
        );
        painter:setPenWidth(math.random(5));
        painter:position(
            math.random(painter:width()),
            math.random(painter:height())
        );
        painter:drawLine(
            math.random(0, painter:width()),
            math.random(0, painter:height())
        );
    end;
end;

function Graphics:RenderRects(painter)
    local alpha = 50;
    painter:setPenAlpha(alpha);
    painter:setFillAlpha(alpha);


    for i=1, 50 do
        painter:setFillColor(255, 255, 255);
        painter:setPenColor(
            0,
            math.random(0, 255),
            math.random(0, 255)
        );
        painter:setPenWidth(math.random(25));
        painter:position(
            math.random(painter:width()),
            math.random(painter:height())
        );
        painter:drawRoundedRect(
            math.random(50, 150),
            math.random(50, 150),
            5,
            25
        );
    end;
end;

function Graphics:RenderTriangle(painter)
    painter:setJoinStyle("miter");

    painter:setPenColor(255, 255, 255, 255);

    painter:position(50, 50);

    painter:setPenWidth(10);

    painter:setFillColor(255, 0, 0, 255);
    painter:drawPolygon({
        {0, 0},
        {50, 0},
        {50, 50},
    });
end;

function Graphics:RenderPolyline(painter)
    painter:position(150, 50);

    painter:setCapStyle("round");
    painter:setJoinStyle("round");
    painter:drawPolyline({
        {0, 0},
        {50, 0},
        {50, 50},
        {0, 50},
        {0, 100},
        {50, 100},
    });

    painter:position(50, 150);
end;

function Graphics:RenderSmiley(painter)
    painter:position(262, 150);
    painter:drawPoint();

    painter:position(288, 150);
    painter:drawPoint();

    painter:position(250, 145);

    painter:setCapStyle("round");

    local diff = 10;
    painter:drawArc(50, 50, -diff * 16, 16 * -(180 - diff * 2));
end;

function Graphics:RenderPixmap(painter)
    painter:position(250, 250);
    painter:drawPixmap("/home/dafrito/tmp/warrior_icon1.png");
end;

function Graphics:DrawEllipse(painter)
    painter:drawEllipse(50, 50);
end;

function Graphics:AddDelegate(delegate)
    return Lists.Insert(self.delegates, delegate);
end;

function Graphics:Render(painter)
    trace("FRAME start");
    self.painter = painter;

    Rainback.ClearCache();

    painter:setFillColor(255, 0, 0);
    for _, delegate in ipairs(self.delegates) do
        local bound = delegate:GetBounds();
        if type(bound) == "table" and bound:HasBounds() then
            local x, y, width, height = bound:GetBounds();
            painter:position(x, y);
            if delegate.frame.GetColor then
                local r, g, b, a = delegate.frame:GetColor();
                r = r * 255;
                g = g * 255;
                b = b * 255;
                if a ~= nil then
                    a = a * 255;
                else
                    a = 255
                end;
                painter:setFillColor(r, g, b, a);
                painter:drawRect(width, height);
            end;
        end;
    end;
    self.painter = nil;

    painter:reset();
    painter:setPenColor(0, 0, 0, 255);
    painter:position(2, 12);
    painter:drawText("Rainback v1.0");

    trace("FRAME complete");
end;
