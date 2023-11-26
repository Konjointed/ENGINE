local mytable = {1,2,3,4,5,6}

local function add(x, y) 
	return x + y
end

local function printtable()
	for i,v in ipairs(mytable) do
		print(i)
	end
end

print(add(5,5))
printtable()

