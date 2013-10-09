out = {}

local address = true
local letter = ""

for i, indent, tokens in tokens, info, 0 do
	if tokens[1] == "OBJTYPE" then
		if tokens[2] == "Gateadresse" then
			address = true
		elseif tokens[2] == "Matrikkeladresse" then
			address = false
		end
	elseif tokens[1] == "GATENAVN" then
		out["addr:street"] = tokens[2]
	elseif tokens[1] == "HUSNR" then
		out["addr:housenumber"] = tokens[2]
	elseif tokens[1] == "BOKST" then
		letter = tokens[2]
	elseif tokens[1] == "POSTNR" then
		out["addr:postcode"] = tokens[2]
	elseif tokens[1] == "POSTNAVN" then
		out["addr:city"] = tokens[2]
	end
end

if out["addr:housenumber"] and letter then out["addr:housenumber"] = string.format("%s%s", out["addr:housenumber"], letter) end

if address == false then
	return {}
end

return out
