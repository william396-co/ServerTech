fib_gen = coroutine.create(function( ) 
	local a,b = 0, 1
	while a <= b do
		local	next = a + b
		coroutine.yield(next)
		a = b
		b = next
	end
	coroutine.yield(0)
end)

for i = 1, 100 do
--	print(coroutine.status(fib_gen))
	local ok,ret = coroutine.resume(fib_gen)
	if ret ~= 0 then
		print("fib("..i..")="..ret)
	else
		print("fib("..i..") out of range")
		break
	end
end
