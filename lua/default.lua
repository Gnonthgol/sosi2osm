#! /usr/bin/env lua

out = {}

for i, line in pairs(info) do
    indent = 0
    while line:byte() == 46 do
        line = line:sub(2)
        indent = indent +1
    end
    
    if indent == 1 then line = line:sub(1, -2) end
    
    tokens = {}
    for x in string.gmatch(line, "(%S+)") do
        if #tokens > 0 and string.byte(tokens[#tokens]) == 34 then
            tokens[#tokens] = table.concat({tokens[#tokens], x}, " ")
        else
            tokens[#tokens+1] = x
        end
        
        if #tokens[#tokens] > 1
            and string.byte(tokens[#tokens], 1) == 34
            and string.byte(tokens[#tokens], -1) == 34 then
            tokens[#tokens] = tokens[#tokens]:sub(2, -2)
        end
    end
    
    if #tokens == 2 then
        out[tokens[1]] = tokens[2]
    elseif #tokens > 2 then
        out[tokens[1]] = table.concat(tokens, " ", 2)
    end
end

return out

