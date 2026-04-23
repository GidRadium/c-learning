import csv
import urllib.request

url = "https://raw.githubusercontent.com/datasets/airport-codes/master/data/airport-codes.csv"

with (
    urllib.request.urlopen(url) as response,
    open("airports.txt", "w", encoding="utf-8") as fout,
):
    lines = (line.decode("utf-8") for line in response)
    reader = csv.DictReader(lines)

    for row in reader:
        code = (row.get("iata_code") or "").strip()
        name = (row.get("name") or "").strip()
        if code:
            fout.write(f"{code}:{name}\n")
