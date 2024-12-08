import xml.etree.ElementTree as ET
from pathlib import Path

LEVELS = [
    (1, "I"),
    (2, "II"),
    (3, "III"),
    (4, "IV"),
    (5, "V"),
    (6, "VI"),
    (7, "VII"),
    (8, "VIII"),
    (9, "IX"),
    (10, "X"),
]


def main():
    ET.register_namespace("", "http://www.w3.org/2000/svg")
    tree = ET.parse(Path(__file__).with_name("template.svg"))
    svg = tree.getroot()
    for [level, levelStr] in LEVELS:
        valText = svg.find(".//{*}text[@id='val']")
        valText.text = levelStr
        tree.write(Path(__file__).with_name(f"{level}.svg"), xml_declaration=True, encoding='unicode')


if __name__ == "__main__":
    main()
