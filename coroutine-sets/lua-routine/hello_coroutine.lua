co = coroutine.create(function() print("hi") end)
print(type(co))             --> thread

print(coroutine.status(co)) -->suspend
coroutine.resume(co)        --> hi
print(coroutine.status(co))  -->dead
