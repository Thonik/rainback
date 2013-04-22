-- Get and copy a clean environment.
require "bit";
local pristine={};
for k,v in pairs(_G) do
	pristine[k]=v;
end;

require "fritomod/currying";
require "fritomod/LuaEnvironment";
require "fritomod/LuaEnvironment-Loaders";
require "fritomod/AllTests";

if arg then
    require "lfs"

	for i=1,#arg do
		arg[i]=arg[i]:gsub("^\./", "");
		local env=LuaEnvironment:New(pristine);
		env:AddLoader(LuaEnvironment.Loaders.Ignore("bit", "lfs"));
		env:AddLoader(LuaEnvironment.Loaders.Filesystem());
		env:Set("AllTests", AllTests);
		env:Require("bin/global");
		env:Require(arg[i]);
	end;
end;

local tests, numRan, numSuccessful, numCrashed, numFailed;

AllTests:AddListener(Metatables.Noop({
    StartAllTests = function(self, suite, name, runner, reason)
        tests = {};
        numRan = 0;
        numSuccessful = 0;
        numFailed = 0;
        numCrashed = 0;
    end,

    FinishAllTests = function(self, suite, successful, report)
        if successful then
            print(("Cumulative: All %d tests ran successfully."):format(numRan));
        else
            print(("Cumulative: %d of %d tests ran successfuly. %d failed, %d crashed"):format(
                numSuccessful,
                numRan,
                numFailed,
                numCrashed));
        end;
    end
}));

AllTests:AddRecursiveListener(Metatables.Noop({
    TestStarted = function(self, suite, name)
        numRan = numRan + 1;
    end,

    TestSuccessful = function(self, suite, name, runner, reason)
        numSuccessful = numSuccessful + 1;
    end,

    TestFailed = function(self, suite, name, runner, reason)
        numFailed = numFailed + 1;
        local testIndex = #tests + 1;
        print(("[FAIL] %d. %s\n  %s"):format(testIndex, name, reason:gsub("\n", "\n	")));
        Lists.Insert(tests, runner);
    end,

    TestCrashed = function(self, suite, name, runner, errorMessage)
        numCrashed = numCrashed + 1;
        local testIndex = #tests + 1;
        print(("[CRASH] %d. %s\n%s"):format(testIndex, name, errorMessage));
        Lists.Insert(tests, runner);
    end,

}));

return AllTests:Run();

-- vim: set ft=lua :
