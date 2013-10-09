out = {}

for i, indent, tokens in tokens, info, 0 do
	if tokens[1] == "FLATE" then
		out["type"] = "multipolygon"
    elseif #tokens == 2 then
        out[tokens[1]] = tokens[2]
    elseif #tokens > 2 then
        out[tokens[1]] = table.concat(tokens, "; ", 2)
    end
end

return out

