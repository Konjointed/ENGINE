local module = {}

module.value = "100" 

function module.doTask() 
	print(module.value)
	print("doing something!")
	print("test")
end

return module 

