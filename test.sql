SELECT Smoothies.Smoothie_id, Raw_Materials.Material_name as "Fruit 1"
FROM Smoothies
JOIN Raw_Materials 
ON Smoothies.Fruit_1 = Raw_Materials.Raw_Material_id
WHERE Smoothies.Fruit_1 = Raw_Materials.Raw_Material_id 

SELECT Smoothies.Smoothie_id, Raw_Materials.Material_name as "Fruit 2"
FROM Smoothies
JOIN Raw_Materials 
ON Smoothies.Fruit_2 = Raw_Materials.Raw_Material_id
WHERE Smoothies.Fruit_2 = Raw_Materials.Raw_Material_id 

