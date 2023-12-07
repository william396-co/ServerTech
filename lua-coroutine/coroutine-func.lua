co = coroutine.create(function(a,b,c)
	print("co",a, b, c+2)
end)

coroutine.resume(co,1,2,3) --> co 1 2 5


co1 = coroutine.create(function(a,b)
	coroutine.yield(a+b,a-b)
end)
print(coroutine.resume(co1,20,10)) -->true 30 10


co2 = coroutine.create(function(x)
	print("co1",x)
	print("co2",coroutine.yield())
end)
coroutine.resume(co2,"hi")-->co1 hi
coroutine.resume(co2,4,5) --> co2 4 5


co3 = coroutine.create(function()
	return 6,7
end)

print(coroutine.resume(co3)) --> true 6 7
