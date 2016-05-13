
function __G__TRACKBACK__(errorMessage)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(errorMessage) .. "\n")
    print(debug.traceback("", 2))
    print("----------------------------------------")
end

--package.path = package.path .. ";scripts/Game/"
cc.FileUtils:getInstance():setPopupNotify(false)

local function main()
	-- 运行slot
	require("app.MyApp").new():run()
end

xpcall(main, __G__TRACKBACK__)