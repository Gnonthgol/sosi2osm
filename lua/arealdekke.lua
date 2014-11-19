out = {}

for i, indent, tokens in tokens, info, 0 do
    if tokens[1] == "FLATE" then
		out["type"] = "multipolygon"
    elseif tokens[1] == "OBJTYPE" then
		if tokens[2] == "Myr" then
			out["natural"]="wetland"
        elseif tokens[2] == "Innsjø" then
			out["natural"]="water"
        elseif tokens[2] == "ElvBekk" then
			out["waterway"]="river"
        elseif tokens[2] == "SnøIsbre" then
            out["natural"]="glacier"
        elseif tokens[2] == "FerskvannTørrfall" then
            out["natural"]="sand"
        elseif tokens[2] == "InnsjøElvSperre" then
            out["waterway"]="weir"
        elseif tokens[2] == "Skog" then
            out["natural"]="wood"
        elseif tokens[2] == "Dam" then
            out["waterway"]="dam"
        elseif tokens[2] == "DyrketMark" then
            out["landuse"]="farmland"
        elseif tokens[2] == "ÅpentOmråde" then
            out["natural"]="grassland"
        elseif tokens[2] == "TettBebyggelse" then
            out["landuse"]="residential"
        elseif tokens[2] == "Industriområde" then
            out["landuse"]="industrial"
        elseif tokens[2] == "BymessigBebyggelse" then
            out["landuse"]="residential"
        elseif tokens[2] == "Havflate" then
            return {}
        elseif tokens[2] == "SportIdrettPlass" then
            out["leisure"] = "pitch"
        elseif tokens[2] == "Gravplass" then
            out["landuse"] = "cemetery"
        elseif tokens[2] == "Golfbane" then
            out["leisure"] = "golf_course"
        elseif tokens[2] == "Steinbrudd" then
            out["landuse"] = "quarry"
        elseif tokens[2] == "Alpinbakke" then
            out["route"] = "piste"
            out["piste:type"] = "downhill"
        elseif tokens[2] == "Rullebane" then
            out["aeroway"] = "runway"
        elseif tokens[2] == "Arealbrukgrense" then
        
        elseif tokens[2] == "FiktivDelelinje" then
        
        elseif tokens[2] == "Innsjøkant" then
        
        elseif tokens[2] == "KantUtsnitt" then

        elseif tokens[2] == "InnsjøkantRegulert" then
        
        elseif tokens[2] == "ElvBekkKant" then
        
        elseif tokens[2] == "HavElvSperre" then
            out["natural"] = "coastline"
        elseif tokens[2] == "Kystkontur" then
            out["natural"] = "coastline"
        elseif tokens[2] == "Helikopterplass" then
            out["aeroway"] = "helipad"
        elseif tokens[2] == "Skjær" then
            out["seamark:type"] = "rock"
        elseif tokens[2] == "Tregruppe" then
            out["natural"] = "tree"
        elseif tokens[2] == "Dataavgrensning" then
            
        else
            out[tokens[1]] = tokens[2]
		end
    elseif tokens[1] == "HØYDE" then
        out["ele"] = tokens[2]
    elseif tokens[1] == "VANNBR" then
        out["width"] = tokens[2]
    elseif tokens[1] == "OPPDATERINGSDATO" then
        out["source:date"] = tokens[2]
    elseif tokens[1] == "DATAFANGSTDATO" then
        out["source:date"] = tokens[2]
    elseif tokens[1] == "KVALITET" then
    
    elseif tokens[1] == "KURVE" or tokens[1] == "PUNKT" or  tokens[1] == "VATNLNR" then
        
    elseif tokens[1] == "NAVN" then
        out["name"] = tokens[2]
    elseif #tokens == 2 then
        out[tokens[1]] = tokens[2]
    elseif #tokens > 2 then
        out["lko"..tokens[1]] = table.concat(tokens, "; ", 2)
    end
end

if out["waterway"] == "river" and (out["width"] == "1" or out["width"] == "2") then
    out["waterway"] = "stream"
end

return out

