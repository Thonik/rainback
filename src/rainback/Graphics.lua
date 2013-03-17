require "fritomod/OOP-Class";

Rainback = Rainback or {};

local Graphics = OOP.Class();
Rainback.Graphics = Graphics;

function Graphics:Dump()
    print "Hello from Rainback.";
end;

function Graphics:RenderText(painter)
    painter:setFontSize(10);
    painter:setFontFamily("Inconsolata");
    painter:setPenColor(0, 0, 0);
    painter:position(5, 14);
    painter:setFontWeight(52);
    painter:drawText("for (int i = 1; i <= points.length(); ++i) {");
end;

function Graphics:Render(painter)
    self:RenderText(painter);

    if true then
        return;
    end;

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

    painter:drawEllipse(50, 50);

    painter:position(262, 150);
    painter:drawPoint();

    painter:position(288, 150);
    painter:drawPoint();

    painter:position(250, 145);

    painter:setCapStyle("round");

    local diff = 10;
    painter:drawArc(50, 50, -diff * 16, 16 * -(180 - diff * 2));

    painter:position(250, 250);
    painter:drawPixmap("/home/dafrito/tmp/warrior_icon1.png");
end;
