
local test = function(class, vector)
	local str = math.serialize(vector)

	local deserialized = class()
	math.deserialize(deserialized, str)

	assert(vector == deserialized)
end

test(math.vec2, math.vec2(0, 1))
test(math.vec3, math.vec3(0, 1, 2))
test(math.vec4, math.vec4(0, 1, 3, 4))

local test_fail = function(class, vector)
	local str = math.serialize(vector)

	local status, error_message = pcall(
		function()
			local deserialized = class()
			math.deserialize(deserialized, str)
		end)

	assert(status == false)
end

test_fail(math.vec2, math.vec3(0, 1, 5))


