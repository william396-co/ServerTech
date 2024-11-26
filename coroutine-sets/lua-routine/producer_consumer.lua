function receive(pred)
    local status, value = coroutine.resume(pred)
    return value
end

function send(x)
    coroutine.yield(x)
end

function producer()
    return coroutine.create(function()
        while true do
            local x = io.read() -- producer new value
            send(x)
        end
    end)
end

function filter(pred)
    return coroutine.create(function()
        for line = 1, math.huge do
            local x = receive(pred) -- get new value
            x = string.format("%5d %s", line, x)
            send(x) -- send it to consumer
        end
    end)
end

function consumer(pred)
    while true do
        local x = receive(pred) -- get new value
        io.write(x,"\n") -- consume new value
    end
end

consumer(filter(producer()))
