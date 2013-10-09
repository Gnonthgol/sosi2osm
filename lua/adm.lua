out = {}

local name = nil
local lang = nil
local samisk = false

for i, indent, tokens in tokens, info, 0 do
    if tokens[1] == "OBJTYPE" then
    	if tokens[2] == "Kommunegrense" or tokens[2] == "Kommune" then
    		out["boundary"] = "administrative"
    		out["admin_level"] = "7"
    	elseif tokens[2] == "Fylkesgrense" or tokens[2] == "Fylke" then 
    		out["boundary"] = "administrative"
    		out["admin_level"] = "4"
    	elseif tokens[2] == "Riksgrense" or tokens[2] == "Nasjon" then 
    		out["boundary"] = "administrative"
    		out["admin_level"] = "2"
    	elseif tokens[2] == "Territorialgrense" then 
    		out["boundary"] = "administrative"
    		out["admin_level"] = "2"
    		out["maritime"] = "yes"
    	elseif tokens[2] == "Grunnlinje" then 
    		out["boundary"] = "maritime"
    		out["border_type"] = "baseline"
    	end
    elseif tokens[1] == "FLATE" then
    	out["type"] = "administrative"
    elseif tokens[1] == "GRENSETYPESJØ" and tokens[2] == "6" then
    	out["boundary"] = "administrative"
    	out["admin_level"] = "2"
    elseif tokens[1] == "KOMM" or tokens[1] == "FYLKESNR" then
    	out["ref"] = tokens[2]
    elseif tokens[1] == "NAVN" then
    	name = tokens[2]
    elseif tokens[1] == "SPRÅK" then
    	lang = tokens[2]
    	if lang == "nor" then lang = "no" end
    elseif tokens[1] == "SAMISKFORVALTNINGSOMRÅDE" then
    	samisk = tokens[2] == "JA"
    elseif tokens[1] == "INFORMASJON" then
    	out["note"] = tokens[2]
    elseif tokens[1] == "OPPHAV" then
    	out["source"] = tokens[2]
    end
    
    if name ~= nil and lang ~= nil then
    	out[string.format("name:%s", lang)] = name
    	name = nil
    	lang = nil
    end
end

if samisk and out["name:sme"] then
	out["name"] = out["name:sme"]
	out["name:sme"] = nil
elseif samisk and out["name:smj"] then
	out["name"] = out["name:smj"]
	out["name:smj"] = nil
else
	out["name"] = out["name:no"]
	out["name:no"] = nil
end

return out
