#!/usr/bin/env python3
"""Dump the raw AC and AF text referenced by a link sheet, for checking links and manual polishing.

Usage:
    python3 text_extractor.py path/to/link-sheet.csv path/to/ac-files path/to/afrepo [output.tsv]

Writes extracted_text.tsv (or the given output path): the link sheet's rows and columns unchanged,
plus two new columns:
    ac_text  raw decoded text at ac_bank[ac_index] in the user's AC files
    af_text  raw decoded text at af_bank[af_index] in the ORIGINAL (Japanese) AF files

The output contains copyrighted text -- keep it out of the repository (e.g. add it to .gitignore).
"""
from __future__ import annotations

import csv
import sys
from pathlib import Path

from translation_injector import (
    AF_BANKS,
    AF_TRANSLATION_BACKUP_DIR,
    AF_CHAR_MAP, 
    AF_OPCODE_COUNT,
    build_item_bank,
    find_ac_entry,
    decode_entry,
    load_ac_banks,
    read_offset_table,
    segname,
    slice_entries,
)

NEW_COLUMNS = ["ac_text", "af_text"]


def read_original(assets_dir: Path, codeword: int) -> bytes | None:
    """Read a segment as it was before injection (the injector's backup if there is one)."""
    path = assets_dir / f"{segname(codeword)}.bin"
    backup = assets_dir / AF_TRANSLATION_BACKUP_DIR / path.name
    source = backup if backup.exists() else path
    return source.read_bytes() if source.exists() else None


def load_af_banks(assets_dir: Path) -> dict[str, tuple]:
    banks = {}
    for name, (data_codeword, table_codeword, _, entry_size, skip_bytes, count) in AF_BANKS.items():
        data = read_original(assets_dir, data_codeword)
        table = read_original(assets_dir, table_codeword) if table_codeword is not None else None
        if data is None or (table_codeword is not None and table is None):
            print(f"[!] {name}: segment file(s) not found in {assets_dir} -- run `make extract` first. Skipping.")
            continue
        if table is not None:
            banks[name] = ("table", slice_entries(data, read_offset_table(table)))
        else:
            if name == "item_1xxx":
                data = bytes(build_item_bank(data))  # AF item indices refer to the re-laid-out grid
            banks[name] = ("fixed", data, entry_size, skip_bytes, count)
    return banks


def find_af_entry(af_data: dict[str, tuple], af_bank: str, af_index_raw: str) -> str:
    af_bank, af_index_raw = (af_bank or "").strip(), (af_index_raw or "").strip()
    if not af_bank or not af_index_raw:
        return ""
    if af_bank not in AF_BANKS:
        raise ValueError(f"unrecognized AF bank {af_bank!r}")
    try:
        index = int(af_index_raw)
    except ValueError:
        raise ValueError(f"{af_bank}[{af_index_raw!r}]: af_index isn't a number") from None

    source = af_data.get(af_bank)
    if source is None:
        return ""
    if source[0] == "table":
        raw = source[1].get(index)
    else:
        _, data, entry_size, skip_bytes, count = source
        start = skip_bytes + index * entry_size
        raw = data[start:start + entry_size] if 0 <= index < count else None
    if not raw:
        return ""
    try:
        text = decode_entry(raw, AF_CHAR_MAP, AF_OPCODE_COUNT, "AF")
    except ValueError as e:
        raise ValueError(f"{af_bank}[{index}]: decode failed ({e})") from e
    return text.rstrip(" ") if source[0] == "fixed" else text  # fixed-width fields are space padded


def lookup(finder, source: dict, bank: str, index: str) -> str:
    try:
        text = finder(source, bank, index)
        text = text.replace("\r\n", "\n").replace("\r", "\n").replace("\n", r"\n") #escape newlines
        return text
    except ValueError as e:
        print(f"WARN {e}")
        return f"<<ERROR: {e}>>"


def main():
    if len(sys.argv) not in (4, 5):
        sys.exit(f"usage: python3 {sys.argv[0]} path/to/link-sheet.csv path/to/ac-files path/to/afrepo [output.tsv]")
    link_sheet, ac_files_dir, af_repo = map(Path, sys.argv[1:4])
    out_path = Path(sys.argv[4]) if len(sys.argv) == 5 else Path("extracted_text.tsv")
    assets_dir = af_repo / "assets" / "jp"

    with open(link_sheet, newline="", encoding="utf-8-sig") as f:
        reader = csv.DictReader(f, delimiter="\t" if link_sheet.suffix.lower() == ".tsv" else ",")
        fieldnames = [c for c in reader.fieldnames if c not in NEW_COLUMNS] + NEW_COLUMNS
        rows = list(reader)

    ac_data = load_ac_banks(ac_files_dir)
    af_data = load_af_banks(assets_dir)

    for row in rows:
        row["ac_text"] = lookup(find_ac_entry, ac_data, row.get("ac_bank"), row.get("ac_index"))
        row["af_text"] = lookup(find_af_entry, af_data, row.get("af_bank"), row.get("af_index"))

    with open(out_path, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames, delimiter="\t", extrasaction="ignore")
        writer.writeheader()
        writer.writerows(rows)

    filled_ac = sum(1 for r in rows if r["ac_text"])
    filled_af = sum(1 for r in rows if r["af_text"])
    print(f"wrote {out_path}: {len(rows)} rows ({filled_ac} with ac_text, {filled_af} with af_text)")


if __name__ == "__main__":
    main()
