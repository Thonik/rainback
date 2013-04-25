require "fritomod/Media-Font";

Media.font.wowDelegate = nil;

Media.font(function(name)
    return tostring(name):lower();
end);
