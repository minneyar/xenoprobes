import re
import sys
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path


@dataclass
class FnSite:
    id: int
    position: tuple[int, int]
    mining_grade: str
    revenue_grade: str
    combat_grade: str
    sightseeing_spots: int
    neighbors: set[int]


def chunk(arr: list, size: int):
    for i in range(0, len(arr), size):
        yield arr[i:i + size]


def main():
    if len(sys.argv) != 5:
        print(f"Usage: {sys.argv[0]} <sites.csv> <mira.dot> <map.svg> <out.cpp>")
        return 1

    sites: dict[int, FnSite] = {}

    # Load FN Site metadata.
    sites_csv_path = Path(sys.argv[1])
    with sites_csv_path.open("rt") as sites_csv:
        for line in sites_csv:
            line = line.strip()
            if line.startswith("#") or len(line) == 0:
                # Comment
                continue
            parts = line.split(",")
            site = FnSite(
                id=int(parts[0], 10),
                position=(0, 0),
                mining_grade=parts[1],
                revenue_grade=parts[2],
                combat_grade=parts[3],
                sightseeing_spots=int(parts[4], 10),
                neighbors=set(),
            )

            sites[site.id] = site

    # Load links.
    links_dot_path = Path(sys.argv[2])
    with links_dot_path.open("rt") as links_dot:
        for line in links_dot:
            line = line.strip()
            if (match := re.match(r"(\d+) -- (\d+)", line)) is None:
                continue
            lhs = int(match.group(1), 10)
            rhs = int(match.group(2), 10)
            sites[lhs].neighbors.add(rhs)

    # Load node map positions.
    svg_path = Path(sys.argv[3])
    svg = ET.parse(svg_path)
    nodes = svg.findall("./{http://www.w3.org/2000/svg}g[@id='nodeslayer']/{http://www.w3.org/2000/svg}image")
    for node in nodes:
        match = re.match(r"FN(\d+)", node.get("{http://www.inkscape.org/namespaces/inkscape}label"))
        if match is None:
            continue
        name = int(match.group(1), 10)
        x = int(node.get("x"))
        y = int(node.get("y"))
        width = int(node.get("width"))
        height = int(node.get("height"))
        # Origin is top-left.
        sites[name].position = (int(x + (width / 2)), int(y + (height / 2)))

    # Sort site by ID for writing.
    sites_list = list(sites.values())
    sites_list.sort(key=lambda v: v.id)

    # Print site constants for header file.
    for site in sites_list:
        print(f"static const Site::Ptr FN{site.id};")

    # Write CPP definition file.
    out_path = Path(sys.argv[4])
    with out_path.open('w') as out:
        out.write(
            "/* To update this file, run scripts/fn_sites.py */\n"
            "\n"
            '#include "probeoptimizer/site.h"\n'
            "\n"
        )
        # Sites
        for site in sites_list:
            out.write(
                f'const Site::Ptr Site::FN{site.id} = new Site{{\n'
                f'    .name = {site.id},\n'
                f'    .position = {{{site.position[0]}, {site.position[1]}}},\n'
                f'    .production = Grade::{site.mining_grade},\n'
                f'    .revenue = Grade::{site.revenue_grade},\n'
                f'    .combat = Grade::{site.combat_grade},\n'
                f'    .sightseeing = {site.sightseeing_spots},\n'
                f'}};\n'
            )

        # Site ID Map
        out.write(
            "\n"
            "const std::map<Site::Id, Site::Ptr> Site::ALL{\n"
        )
        for site_chunk in chunk(sites_list, 3):
            out.write('    ')
            for site in site_chunk:
                out.write(f'{{{site.id}, Site::FN{site.id}}}, ')
            out.write("\n")

        # Site Neighbors Map
        out.write(
            "};\n"
            "\n"
            "const std::unordered_map<Site::Id, std::vector<Site::Ptr>> Site::NEIGHBORS{\n"
        )
        for site in sites_list:
            neighbors = [f'Site::FN{neighbor}' for neighbor in site.neighbors]
            out.write(f'    {{{site.id}, {{{", ".join(neighbors)}}}}},\n')
        out.write('};\n')

    return 0


if __name__ == "__main__":
    exit(main())
