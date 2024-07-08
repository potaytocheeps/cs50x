-- Get names of songs that feature more than one artist
SELECT name
FROM songs
WHERE name LIKE '%feat.%';
