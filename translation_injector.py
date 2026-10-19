#!/usr/bin/env python3
"""Inject Animal Crossing's English text into an Animal Forest build.

Usage:
    python3 text_injector.py path/to/translation_sheet.tsv path/to/ac-files path/to/afrepo

"""
from __future__ import annotations

import csv
import re
import struct
import sys
from pathlib import Path

# AF text format
AF_CHAR_MAP = {
    0x00: 'あ', 0x01: 'い', 0x02: 'う', 0x03: 'え', 0x04: 'お',
    0x05: 'か', 0x06: 'き', 0x07: 'く', 0x08: 'け', 0x09: 'こ',
    0x0A: 'さ', 0x0B: 'し', 0x0C: 'す', 0x0D: 'せ', 0x0E: 'そ', 0x0F: 'た',
    0x10: 'ち', 0x11: 'つ', 0x12: 'て', 0x13: 'と', 0x14: 'な',
    0x15: 'に', 0x16: 'ぬ', 0x17: 'ね', 0x18: 'の', 0x19: 'は',
    0x1A: 'ひ', 0x1B: 'ふ', 0x1C: 'へ', 0x1D: 'ほ', 0x1E: 'ま', 0x1F: 'み',
    0x20: ' ', 0x21: '!', 0x22: '"', 0x23: 'む', 0x24: 'め',
    0x25: '%', 0x26: '&', 0x27: "'", 0x28: '(', 0x29: ')',
    0x2A: '~', 0x2B: '♥', 0x2C: ',', 0x2D: '-', 0x2E: '.', 0x2F: '\u266A',
    0x30: '0', 0x31: '1', 0x32: '2', 0x33: '3', 0x34: '4',
    0x35: '5', 0x36: '6', 0x37: '7', 0x38: '8', 0x39: '9',
    0x3A: ':', 0x3B: '🌢', 0x3C: '<', 0x3D: '=', 0x3E: '>', 0x3F: '?',
    0x40: '@', 0x41: 'A', 0x42: 'B', 0x43: 'C', 0x44: 'D',
    0x45: 'E', 0x46: 'F', 0x47: 'G', 0x48: 'H', 0x49: 'I',
    0x4A: 'J', 0x4B: 'K', 0x4C: 'L', 0x4D: 'M', 0x4E: 'N', 0x4F: 'O',
    0x50: 'P', 0x51: 'Q', 0x52: 'R', 0x53: 'S', 0x54: 'T',
    0x55: 'U', 0x56: 'V', 0x57: 'W', 0x58: 'X', 0x59: 'Y', 0x5A: 'Z',
    0x5B: 'も', 0x5C: '+', 0x5D: 'や', 0x5E: 'ゆ', 0x5F: '_',
    0x60: 'よ', 0x61: 'a', 0x62: 'b', 0x63: 'c', 0x64: 'd',
    0x65: 'e', 0x66: 'f', 0x67: 'g', 0x68: 'h', 0x69: 'i',
    0x6A: 'j', 0x6B: 'k', 0x6C: 'l', 0x6D: 'm', 0x6E: 'n', 0x6F: 'o',
    0x70: 'p', 0x71: 'q', 0x72: 'r', 0x73: 's', 0x74: 't',
    0x75: 'u', 0x76: 'v', 0x77: 'w', 0x78: 'x', 0x79: 'y', 0x7A: 'z',
    0x7B: 'ら', 0x7C: 'り', 0x7D: 'る', 0x7E: 'れ',
    # 0x7F is the control-code escape byte -- deliberately absent.
    0x80: '�', 0x81: '。', 0x82: '「', 0x83: '」', 0x84: '、',
    0x85: '・', 0x86: 'ヲ', 0x87: 'ァ', 0x88: 'ィ', 0x89: 'ゥ',
    0x8A: 'ェ', 0x8B: 'ォ', 0x8C: 'ャ', 0x8D: 'ュ', 0x8E: 'ョ', 0x8F: 'ッ',
    0x90: 'ー', 0x91: 'ア', 0x92: 'イ', 0x93: 'ウ', 0x94: 'エ',
    0x95: 'オ', 0x96: 'カ', 0x97: 'キ', 0x98: 'ク', 0x99: 'ケ',
    0x9A: 'コ', 0x9B: 'サ', 0x9C: 'シ', 0x9D: 'ス', 0x9E: 'セ', 0x9F: 'ソ',
    0xA0: 'タ', 0xA1: 'チ', 0xA2: 'ツ', 0xA3: 'テ', 0xA4: 'ト',
    0xA5: 'ナ', 0xA6: 'ニ', 0xA7: 'ヌ', 0xA8: 'ネ', 0xA9: 'ノ',
    0xAA: 'ハ', 0xAB: 'ヒ', 0xAC: 'フ', 0xAD: 'ヘ', 0xAE: 'ホ', 0xAF: 'マ',
    0xB0: 'ミ', 0xB1: 'ム', 0xB2: 'メ', 0xB3: 'モ', 0xB4: 'ヤ',
    0xB5: 'ユ', 0xB6: 'ヨ', 0xB7: 'ラ', 0xB8: 'リ', 0xB9: 'ル',
    0xBA: 'レ', 0xBB: 'ロ', 0xBC: 'ワ', 0xBD: 'ン', 0xBE: 'ヴ', 0xBF: '😃',
    0xC0: 'ろ', 0xC1: 'わ', 0xC2: 'を', 0xC3: 'ん', 0xC4: 'ぁ',
    0xC5: 'ぃ', 0xC6: 'ぅ', 0xC7: 'ぇ', 0xC8: 'ぉ', 0xC9: 'ゃ',
    0xCA: 'ゅ', 0xCB: 'ょ', 0xCC: 'っ', 0xCD: '\n', 0xCE: 'ガ', 0xCF: 'ギ',
    0xD0: 'グ', 0xD1: 'ゲ', 0xD2: 'ゴ', 0xD3: 'ザ', 0xD4: 'ジ',
    0xD5: 'ズ', 0xD6: 'ゼ', 0xD7: 'ゾ', 0xD8: 'ダ', 0xD9: 'ヂ',
    0xDA: 'ヅ', 0xDB: 'デ', 0xDC: 'ド', 0xDD: 'バ', 0xDE: 'ビ', 0xDF: 'ブ',
    0xE0: 'ベ', 0xE1: 'ボ', 0xE2: 'パ', 0xE3: 'ピ', 0xE4: 'プ',
    0xE5: 'ペ', 0xE6: 'ポ', 0xE7: 'が', 0xE8: 'ぎ', 0xE9: 'ぐ',
    0xEA: 'げ', 0xEB: 'ご', 0xEC: 'ざ', 0xED: 'じ', 0xEE: 'ず', 0xEF: 'ぜ',
    0xF0: 'ぞ', 0xF1: 'だ', 0xF2: 'ぢ', 0xF3: 'づ', 0xF4: 'で',
    0xF5: 'ど', 0xF6: 'ば', 0xF7: 'び', 0xF8: 'ぶ', 0xF9: 'べ',
    0xFA: 'ぼ', 0xFB: 'ぱ', 0xFC: 'ぴ', 0xFD: 'ぷ', 0xFE: 'ぺ', 0xFF: 'ぽ',
}

'''
AC_CHAR_MAP = [
    "¡", "¿", "Ä", "À", "Á", "Â", "Ã", "Å",
    "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î",
    "Ï", "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö",
    "Ø", "Ù", "Ú", "Û", "Ü", "ß", "Þ", "à",
    " ", "!", "\"", "á", "â", "%", "&", "'",
    "(", ")", "~", "♥", ",", "-", ".", "♪",
    "0", "1", "2", "3", "4", "5", "6", "7",
    "8", "9", ":", "🌢", "<", "=", ">", "?",
    "@", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "ã", "💢", "ä", "å", "_",
    "ç", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "è", "é", "ê", "ë", "",
    "�", "ì", "í", "î", "ï", "•", "ð", "ñ",
    "ò", "ó", "ô", "õ", "ö", "⁰", "ù", "ú",
    "ー", "û", "ü", "ý", "ÿ", "þ", "Ý", "¦",
    "§", "ḏ", "ṉ", "‖", "µ", "³", "²", "¹",
    "¯", "¬", "Æ", "æ", "„", "»", "«", "☀",
    "☁", "☂", "🌬", "☃", "∋", "∈", "/", "∞",
    "○", "🗙", "□", "△", "+", "⚡", "♂", "♀",
    "🍀", "★", "💀", "😮", "😄", "😣", "😠", "😃",
    "×", "➗", "🔨", "🎀", "✉", "💰", "🐾", "🐶",
    "🐱", "🐰", "🐦", "🐮", "🐷", "\n", "🐟", "🐞",
    ";", "#", "Ò", "Ó", "⚷", "Õ", "Ö", "×",
    "Ø", "Ù", "Ú", "Û", "Ü", "Ỳ", "ꟓ", "ß",
    "à", "á", "â", "ã", "ä", "å", "æ", "ç",
    "è", "é", "ê", "ë", "ì", "í", "î", "ï",
    "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "÷",
    "ø", "ù", "ú", "û", "ü", "ý", "þ", "ÿ",
]
'''

# Closest match for each AC character
AC_CHAR_MAP = [
    "!", "?", "A", "A", "A", "A", "A", "A",
    "C", "E", "E", "E", "E", "I", "I", "I",
    "I", "D", "N", "O", "O", "O", "O", "O",
    "O", "U", "U", "U", "U", "B", "P", "a",
    " ", "!", "\"", "a", "a", "%", "&", "'",
    "(", ")", "~", "♥", ",", "-", ".", "♪",
    "0", "1", "2", "3", "4", "5", "6", "7",
    "8", "9", ":", "🌢", "<", "=", ">", "?",
    "@", "A", "B", "C", "D", "E", "F", "G",
    "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W",
    "X", "Y", "Z", "a", "+", "a", "a", "_",
    "c", "a", "b", "c", "d", "e", "f", "g",
    "h", "i", "j", "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t", "u", "v", "w",
    "x", "y", "z", "e", "e", "e", "e", " ",
    "�", "i", "i", "i", "i", "・", "d", "n",
    "o", "o", "o", "o", "o", "0", "u", "u",
    "ー", "u", "u", "y", "y", "p", "Y", "・",
    "・", "d", "n", "・", "u", "3", "2", "1",
    "-", "-", "?", "?", "\"", ">", "<", "・",
    "・", "・", "・", "・", "・", "・", "・", "・",
    "O", "X", "ロ", "・", "+", "・", "・", "・",
    "・", "・", "・", "・", "😃", "・", "・", "😃",
    "X", ":", "T", "X", "ロ", "ロ", "・", "😃",
    "・", "・", "・", "・", "・", "\n", "・", "・",
    ",", "・", "O", "O", "・", "O", "O", "X",
    "O", "U", "U", "U", "U", "Y", "3", "B",
    "a", "a", "a", "a", "a", "a", "a", "c",
    "e", "e", "e", "e", "i", "i", "i", "i",
    "d", "n", "o", "o", "o", "o", "o", "・",
    "o", "u", "u", "u", "u", "y", "p", "y",
]
AC_CHAR_DICT = dict(enumerate(AC_CHAR_MAP))

COMMAND_SIZES = [
    2, 2, 2, 3, 2, 5, 2, 2, 5, 5, 5, 5, 5, 2, 4, 4,
    4, 4, 4, 6, 8, 10, 6, 8, 10, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    6, 3, 3, 3, 3, 2, 4, 4, 3, 3, 3, 2, 2, 2, 2, 2,
    2, 2, 2, 6, 3, 3, 4, 3, 2, 2, 6, 2, 2, 3, 3, 3,
    3, 2, 2, 2, 2, 2, 2, 4, 4, 12, 14,
]

COMMAND_CODES = [
    "MSGEND", "MSGCONTINUE", "MSGCLEAR", "PAUSE", "BTN", "TEXTCOLOR", "ABLECANCEL", "UNABLECANCEL",
    "DEMOPLR", "DEMONPC0", "DEMONPC1", "DEMONPC2", "DEMONPCQST", "OPENCHOICE", "SETFORCEMSG", "SETNEXTMSG0",
    "SETNEXTMSG1", "SETNEXTMSG2", "SETNEXTMSG3", "SETNEXTMSGRND2", "SETNEXTMSGRND3", "SETNEXTMSGRND4", "SETSELSTR2", "SETSELSTR3",
    "SETSELSTR4", "FORCENEXT", "STR_PLAYERNAME", "STR_TALKNAME", "STR_TAIL", "STR_YEAR", "STR_MONTH", "STR_WEEK",
    "STR_DAY", "STR_HOUR", "STR_MIN", "STR_SEC", "STR_FREE0", "STR_FREE1", "STR_FREE2", "STR_FREE3",
    "STR_FREE4", "STR_FREE5", "STR_FREE6", "STR_FREE7", "STR_FREE8", "STR_FREE9", "STR_DETERMINATION", "STR_COUNTRYNAME",
    "STR_RNDNUM", "STR_ITEM0", "STR_ITEM1", "STR_ITEM2", "STR_ITEM3", "STR_ITEM4", "STR_FREE10", "STR_FREE11",
    "STR_FREE12", "STR_FREE13", "STR_FREE14", "STR_FREE15", "STR_FREE16", "STR_FREE17", "STR_FREE18", "STR_FREE19",
    "STR_MAIL", "LUCK_NEUTRAL", "LUCK_RELATIONSHIP", "LUCK_UNPOPULAR", "LUCK_BAD", "LUCK_MONEY", "LUCK_GOODS", "LUCK_6",
    "LUCK_7", "LUCK_8", "LUCK_9", "MSGCONTENTS_NORMAL", "MSGCONTENTS_ANGRY", "MSGCONTENTS_SAD", "MSGCONTENTS_FUN", "MSGCONTENTS_SLEEPY",
    "COLORCHARS", "SNDCUT", "LINEOFS", "LINETYPE", "CHARSCALE", "BTN2", "BGMMAKE", "BGMDELETE",
    "MSGTIMEEND", "SNDTRGSYS", "LINESCALE", "SNDNOPAGE", "VOICETRUE", "VOICEFALSE", "SELNOB", "GIVEOPEN",
    "GIVECLOSE", "MSGCONTENTS_GLOOMY", "SELNOBCLOSE", "SETNEXTMSGRNDSECTION", "AGBDUMMY0", "AGBDUMMY1", "AGBDUMMY2", "SPACE",
    "AGBDUMMY3", "AGBDUMMY4", "MALEFEMALECHK", "AGBDUMMY5", "AGBDUMMY6", "AGBDUMMY7", "AGBDUMMY8", "AGBDUMMY9",
    "AGBDUMMY10", "STR_ISLANDNAME", "SETCURSORJUST", "CLRCURSORJUST", "CUTARTICLE", "CAPITALIZE", "STR_AMPM", "SETNEXTMSG4",
    "SETNEXTMSG5", "SETSELSTR5", "SETSELSTR6",
]
assert len(COMMAND_CODES) == len(COMMAND_SIZES)

# AF uses the same opcode numbering and sizes as AC, but only knows the first
# 0x5F opcodes (through SELNOB). One table, AC names, serves both games.
COMMAND_OPCODE = {name: op for op, name in enumerate(COMMAND_CODES)}
assert len(COMMAND_OPCODE) == len(COMMAND_CODES), "duplicate name in COMMAND_CODES"
AF_OPCODE_COUNT = 0x5F
AF_COMMANDS = frozenset(COMMAND_CODES[:AF_OPCODE_COUNT])

AF_DROP_CODES = {"STR_AMPM", "SETCURSORJUST", "CLRCURSORJUST", "CUTARTICLE", "CAPITALIZE", "SELNOBCLOSE", "SPACE"}

AF_INDEX_ARG_CODES = {
    "SETFORCEMSG",
    "SETNEXTMSG0", "SETNEXTMSG1", "SETNEXTMSG2", "SETNEXTMSG3",
    "SETNEXTMSGRND2", "SETNEXTMSGRND3", "SETNEXTMSGRND4",
    "SETSELSTR2", "SETSELSTR3", "SETSELSTR4",
}
AF_MAX_ENTRY_SIZE = 0x400

NPC_NAME_LEN = 8 # include/m_npc.h
NPC_VANILLA_LEN = 6
NPC_BANK_HEADER = 0x08
NPC_BANK_ENTRIES = 0xFF

ITEM_NAME_LEN = 16 # include/m_item_name.h
ITEM_VANILLA_LEN = 10
ITEM_BANK_HEADER = 0x08
ITEM_VANILLA_CATEGORIES = (  # (vanilla offset, entry count), in bank order
    (0x0008, 64), (0x0288, 4), (0x02B0, 36), (0x0418, 32), (0x0558, 255), (0x0F50, 30),
    (0x107C, 64), (0x12FC, 64), (0x157C, 7), (0x15C4, 10), (0x1628, 55), (0x1850, 1),
    (0x185C, 96), (0x1C1C, 32), (0x1D5C, 2), (0x1D70, 4),
    (0x1D98, 3789),  # furniture
)

def item_slot(vanilla_offset: int) -> int:
    """Flat slot of a vanilla item offset (mirrors ITEM_BASE_IDX in m_item_name.c)."""
    return (vanilla_offset - ITEM_BANK_HEADER + ITEM_VANILLA_LEN // 2) // ITEM_VANILLA_LEN

ITEM_BANK_ENTRIES = item_slot(ITEM_VANILLA_CATEGORIES[-1][0]) + ITEM_VANILLA_CATEGORIES[-1][1]

TAG_RE = re.compile(r'<<([A-Za-z0-9_\[\]+]+)((?:\s*\[[^\]]*\])?)>>')
HEX_ARG_RE = re.compile(r'\[([0-9A-Fa-f ]+)\]')

# AC source banks:
#   pair      -> (kind, archive filename), with data/<bank>.bin + _table.bin
#   fixed_arc -> (kind, archive filename, archive path, entry size)
#   fixed_rel -> (kind, section offset, bank byte size)
AC_BANKS = {
    "select_data":     ("pair", "forest_1st.arc"),
    "mail_data":       ("pair", "forest_1st.arc"),
    "super_data":      ("pair", "forest_1st.arc"),
    "ps_data":         ("pair", "forest_1st.arc"),
    "string_data":     ("pair", "forest_1st.arc"),
    "message_data":    ("pair", "forest_2nd.arc"),
    "npc_name_str":    ("fixed_arc", "forest_2nd.arc", "data/npc_name_str_table.bin", 0x08),
    "itemName_paper":  ("fixed_rel", 0x00272160, 0x1000),
    "itemName_money":  ("fixed_rel", 0x00273160, 0x0040),
    "itemName_tool":   ("fixed_rel", 0x002731A0, 0x05C0),
    "itemName_fish":   ("fixed_rel", 0x00273760, 0x0280),
    "itemName_cloth":  ("fixed_rel", 0x002739E0, 0x0FF0),
    "itemName_etc":    ("fixed_rel", 0x002749D0, 0x0310),
    "itemName_carpet": ("fixed_rel", 0x00274CE0, 0x0430),
    "itemName_wall":   ("fixed_rel", 0x00275110, 0x0430),
    "itemName_fruit":  ("fixed_rel", 0x00275540, 0x0080),
    "itemName_plant":  ("fixed_rel", 0x002755C0, 0x00B0),
    "itemName_minidisk": ("fixed_rel", 0x00275670, 0x0370),
    "itemName_dummy":  ("fixed_rel", 0x002759E0, 0x0100),
    "itemName_ticket": ("fixed_rel", 0x00275AE0, 0x0600),
    "itemName_insect": ("fixed_rel", 0x002760E0, 0x02D0),
    "itemName_hukubukuro": ("fixed_rel", 0x002763B0, 0x0020),
    "itemName_kabu":   ("fixed_rel", 0x002763D0, 0x0040),
    "ftrName_table":   ("fixed_rel", 0x00276410, 0x1DF0),
    "ftrName2_table":  ("fixed_rel", 0x00279BF0, 0x1740),
}
AC_REL_FILE = "foresta.rel.szs"
AC_REL_SECTION = 0x05
AC_REL_ENTRY_SIZE = 0x10

# AF banks: (data codeword, table codeword, carrier or None, entry size, skip bytes, count)
AF_BANKS = {
    "message_data": (0x00BD4000, 0x00CF9000, "dialogue", None, None, None),
    "select_data":  (0x00D05000, 0x00D06000, None, None, None, None),
    "mail_data":    (0x00D07000, 0x00D10000, None, None, None, None),
    "super_data":   (0x00D11000, 0x00D12000, None, None, None, None),
    "ps_data":      (0x00D13000, 0x00D15000, None, None, None, None),
    "string_data":  (0x00D16000, 0x00D18000, None, None, None, None),
    "npc_name_str": (0x00E04000, None, None, NPC_NAME_LEN, NPC_BANK_HEADER, NPC_BANK_ENTRIES),
    "item_1xxx":    (0x010F4000, None, None, ITEM_NAME_LEN, ITEM_BANK_HEADER, ITEM_BANK_ENTRIES),
}
AF_CARRIERS = {
    "dialogue": {"segment": "softsprite_matrix_static", "codeword": 0x01913000,
                 "real_size": 0x40, "offset": 0x1000, "end": 0x1C00000},
    "shared":   {"segment": "segment_00BD4000", "codeword": 0x00BD4000, "size": 0x124A10},
}
AF_LINKER_SCRIPT = Path("linker_scripts/jp/auto/undefined_syms_auto.ld")
AF_TRANSLATION_BACKUP_DIR = "translation_offsets"


def decode_entry(raw: bytes, char_map: dict[int, str], opcode_count: int, label: str) -> str:
    out, i = [], 0
    while i < len(raw):
        b = raw[i]
        if b == 0x7F:
            if i + 1 >= len(raw):
                raise ValueError(f"truncated {label} control code at byte {i}")
            opcode = raw[i + 1]
            if opcode >= opcode_count:
                raise ValueError(f"{label} opcode 0x{opcode:02X} at byte {i} is unknown")
            name, size = COMMAND_CODES[opcode], COMMAND_SIZES[opcode]
            if i + size > len(raw):
                raise ValueError(f"truncated {label} {name} at byte {i}")
            args = raw[i + 2:i + size]
            out.append(f"<<{name} [{args.hex().upper()}]>>" if args else f"<<{name}>>")
            i += size
        else:
            if b not in char_map:
                raise ValueError(f"{label} byte 0x{b:02X} at position {i} has no CHAR_MAP entry")
            out.append(char_map[b])
            i += 1
    return "".join(out)


def encode_af_entry(text: str) -> bytes:
    char_to_byte = {char: byte for byte, char in AF_CHAR_MAP.items()}
    out, i, n = bytearray(), 0, len(text)
    while i < n:
        if text[i:i + 2] == "<<":
            end = text.find(">>", i + 2)
            if end == -1:
                raise ValueError(f"unterminated AF control code near: {text[i:i + 20]!r}")
            name, _, arg_part = text[i + 2:end].partition("[")
            name = name.strip()
            if name not in AF_COMMANDS:
                raise ValueError(f"control code {name!r} is not valid in AF")
            opcode = COMMAND_OPCODE[name]
            size = COMMAND_SIZES[opcode]
            args = bytes.fromhex(arg_part.rstrip("]").strip()) if arg_part else b""
            if len(args) != size - 2:
                raise ValueError(f"{name} expects {size - 2} arg byte(s), got {len(args)}")
            out.extend((0x7F, opcode, *args))
            i = end + 2
            continue

        ch = text[i]
        if ch not in char_to_byte:
            raise ValueError(f"character {ch!r} has no byte mapping in AF_CHAR_MAP")
        out.append(char_to_byte[ch])
        i += 1
    return bytes(out)


def normalize_tags(text: str) -> str:
    """Drop AC-only codes AF can't represent and normalize hex args."""
    def repl(m):
        name, rest = m.group(1), m.group(2)
        if name in AF_DROP_CODES:
            return ""
        rest = HEX_ARG_RE.sub(lambda h: "[" + h.group(1).replace(" ", "").upper() + "]", rest)
        return f"<<{name}{rest}>>"

    return TAG_RE.sub(repl, text)


def transfer_jump_targets(translated_text: str, af_raw: bytes | None) -> tuple[str, bool]:
    translated_names = [m.group(1) for m in TAG_RE.finditer(translated_text) if m.group(1) in AF_INDEX_ARG_CODES]

    if af_raw is None:
        return translated_text, not translated_names

    original = []
    i = 0
    while i < len(af_raw):
        if af_raw[i] != 0x7F:
            i += 1
            continue
        if i + 1 >= len(af_raw):
            return translated_text, False
        opcode = af_raw[i + 1]
        if opcode >= AF_OPCODE_COUNT:
            return translated_text, False
        name, size = COMMAND_CODES[opcode], COMMAND_SIZES[opcode]
        if i + size > len(af_raw):
            return translated_text, False
        if name in AF_INDEX_ARG_CODES:
            original.append((name, af_raw[i + 2:i + size]))
        i += size

    if translated_names != [name for name, _ in original]:
        return translated_text, False

    index = 0
    def repl(m):
        nonlocal index
        name = m.group(1)
        if name not in AF_INDEX_ARG_CODES:
            return m.group(0)
        args = original[index][1]
        index += 1
        return f"<<{name} [{args.hex().upper()}]>>" if args else f"<<{name}>>"

    return TAG_RE.sub(repl, translated_text), True


def encode_row(text: str, af_raw: bytes | None, label: str, allow_jump_mismatch: bool = False):
    text, jump_ok = transfer_jump_targets(normalize_tags(text), af_raw)
    if not jump_ok and not allow_jump_mismatch:
        print(f"WARN {label}: leaving original -- jump/choice target mismatch")
        return None

    try:
        encoded = encode_af_entry(text)
        decode_entry(encoded, AF_CHAR_MAP, AF_OPCODE_COUNT, "AF")
    except ValueError as e:
        print(f"WARN {label}: leaving original -- {e}")
        return None

    if len(encoded) > AF_MAX_ENTRY_SIZE:
        print(f"WARN {label}: leaving original -- encoded entry {len(encoded)} bytes exceeds {AF_MAX_ENTRY_SIZE}-byte max entry size")
        return None
    return encoded


def read_offset_table(table_bytes: bytes) -> list[int]:
    return list(struct.unpack(f">{len(table_bytes) // 4}I", table_bytes))


def slice_entries(data_bytes: bytes, offsets: list[int]) -> dict[int, bytes]:
    entries, last_end = {}, 0
    for index, end in enumerate(offsets):
        if end:
            entries[index] = data_bytes[last_end:end]
            last_end = end
    return entries


def pack_raw_entries(entries_by_index: dict[int, bytes], table_len: int) -> tuple[bytes, bytes]:
    data, offsets = bytearray(), [0] * table_len
    for index in range(table_len):
        entry = entries_by_index.get(index)
        if entry is not None:
            data.extend(entry)
            offsets[index] = len(data)
    table = b"".join(struct.pack(">I", offset) for offset in offsets)
    return bytes(data), table


def segname(codeword: int) -> str:
    return f"segment_{codeword:08X}"


def yaz0_decompress(data: bytes) -> bytes:
    if data[:4] != b"Yaz0":
        return data
    dec_size = struct.unpack_from(">I", data, 4)[0]
    out = bytearray(dec_size)
    src, dst, code_byte, bits_left = 0x10, 0, 0, 0
    while dst < dec_size:
        if bits_left == 0:
            code_byte = data[src]
            src += 1
            bits_left = 8
        if code_byte & 0x80:
            out[dst] = data[src]
            dst += 1
            src += 1
        else:
            b1, b2 = data[src], data[src + 1]
            src += 2
            distance = ((b1 & 0x0F) << 8 | b2) + 1
            length = b1 >> 4
            if length == 0:
                length = data[src] + 0x12
                src += 1
            else:
                length += 2
            copy_src = dst - distance
            for _ in range(length):
                out[dst] = out[copy_src]
                dst += 1
                copy_src += 1
        code_byte = (code_byte << 1) & 0xFF
        bits_left -= 1
    return bytes(out)


def unpack_rarc(raw: bytes) -> dict[str, bytes]:
    raw = yaz0_decompress(raw)
    if raw[:4] != b"RARC":
        raise ValueError("not a RARC archive")

    _, _, info_offset, info_size = struct.unpack_from(">4I", raw, 0)
    num_nodes, node_offset, num_files, file_offset, _, string_offset = struct.unpack_from(">6I", raw, info_offset)
    node_offset += info_offset
    file_offset += info_offset
    string_offset += info_offset
    data_offset = info_offset + info_size

    def read_string(offset):
        end = offset
        while end < len(raw) - 1 and raw[end] != 0:
            end += 1
        return raw[offset:end].decode("ascii", errors="replace")

    nodes = []
    offset = node_offset
    for _ in range(num_nodes):
        _, _, _, file_count, file_index = struct.unpack_from(">2I2HI", raw, offset)
        nodes.append((file_count, file_index))
        offset += 0x10

    files = []
    offset = file_offset
    for _ in range(num_files):
        _, _, attrs_name, data_offset_in_archive, data_size, _ = struct.unpack_from(">2H4I", raw, offset)
        files.append((read_string(string_offset + (attrs_name & 0x00FFFFFF)), attrs_name >> 24,
                      data_offset_in_archive, data_size))
        offset += 0x14

    result: dict[str, bytes] = {}

    def walk(node_index, path):
        file_count, file_index = nodes[node_index]
        for name, attrs, entry_offset, entry_size in files[file_index:file_index + file_count]:
            if name in (".", ".."):
                continue
            if attrs & 2:
                walk(entry_offset, f"{path}{name}/")
            elif attrs & 1:
                start = data_offset + entry_offset
                result[f"{path}{name}"] = raw[start:start + entry_size]

    walk(0, "")
    return result


def decode_ac_entry_fixed(raw: bytes) -> str:
    return raw.split(b"\0", 1)[0].decode("ascii", errors="replace").rstrip()


def load_ac_banks(ac_files_dir: Path) -> dict[str, tuple]:
    arcs = {}
    for filename in {spec[1] for spec in AC_BANKS.values() if spec[0] in {"pair", "fixed_arc"}}:
        path = ac_files_dir / filename
        if not path.exists():
            print(f"[!] {path} not found -- skipping rows that use it.")
        else:
            arcs[filename] = unpack_rarc(path.read_bytes())

    rel_data_section = None
    rel_path = ac_files_dir / AC_REL_FILE
    if rel_path.exists():
        data = yaz0_decompress(rel_path.read_bytes())
        section_count, section_table = struct.unpack_from(">2I", data, 0x0C)
        if AC_REL_SECTION >= section_count:
            raise ValueError(f"AC .rel has only {section_count} sections; expected section 0x{AC_REL_SECTION:02X}")
        raw_offset, length = struct.unpack_from(">II", data, section_table + AC_REL_SECTION * 8)
        raw_offset &= ~1
        rel_data_section = data[raw_offset:raw_offset + length]
    else:
        print(f"[!] {rel_path} not found -- skipping rows that use its banks.")

    data_by_bank = {}
    for bank, spec in AC_BANKS.items():
        kind = spec[0]
        if kind == "pair":
            archive = arcs.get(spec[1])
            if archive is None:
                continue
            data_path, table_path = f"data/{bank}.bin", f"data/{bank}_table.bin"
            if data_path not in archive or table_path not in archive:
                print(f"[!] {spec[1]} is missing {data_path} or {table_path}; skipping {bank}.")
                continue
            data_by_bank[bank] = ("pair", slice_entries(archive[data_path], read_offset_table(archive[table_path])))
        elif kind == "fixed_arc":
            archive = arcs.get(spec[1])
            if archive is not None and spec[2] in archive:
                data_by_bank[bank] = ("fixed_arc", archive[spec[2]], spec[3])
        elif kind == "fixed_rel" and rel_data_section is not None:
            data_by_bank[bank] = ("fixed_rel", rel_data_section, spec[1], spec[2])
    return data_by_bank


def find_ac_entry(data_by_bank: dict[str, tuple], ac_bank: str, ac_index_raw: str) -> str:
    ac_bank, ac_index_raw = (ac_bank or "").strip(), (ac_index_raw or "").strip()
    if not ac_bank or not ac_index_raw:
        return ""
    try:
        index = int(ac_index_raw)
    except ValueError:
        raise ValueError(f"{ac_bank}[{ac_index_raw!r}]: ac_index isn't a number") from None

    source = data_by_bank.get(ac_bank)
    if source is None:
        if ac_bank not in AC_BANKS:
            raise ValueError(f"unrecognized AC bank {ac_bank!r}")
        return ""

    kind = source[0]
    try:
        if kind == "pair":
            entries = source[1]
            return decode_entry(entries[index], AC_CHAR_DICT, len(COMMAND_CODES), "AC") if index in entries else ""
        if kind == "fixed_arc":
            data, entry_size = source[1], source[2]
            offset = index * entry_size
            if offset < 0 or offset + entry_size > len(data):
                return ""
            return decode_ac_entry_fixed(data[offset:offset + entry_size])
        data, base, bank_size = source[1], source[2], source[3]
        offset = base + index * AC_REL_ENTRY_SIZE
        if offset < base or offset + AC_REL_ENTRY_SIZE > base + bank_size:
            return ""
        return decode_ac_entry_fixed(data[offset:offset + AC_REL_ENTRY_SIZE])
    except (IndexError, ValueError) as e:
        raise ValueError(f"{ac_bank}[{index}]: decode failed ({e})") from e


def should_inject(row: dict) -> bool:
    override = (row.get("override_text") or "").strip()
    ac_bank = (row.get("ac_bank") or "").strip()
    ac_index = (row.get("ac_index") or "").strip()
    if bool(ac_bank) != bool(ac_index):
        raise ValueError(f"incomplete AC mapping for {row.get('af_bank', '')}[{row.get('af_index', '')}]")
    return bool(override or (ac_bank and ac_index))


def normalize_linebreaks(text: str) -> str:
    text = text.replace(r"\r\n", "\n")
    text = text.replace(r"\n", "\n").replace(r"\r", "\n")
    text = text.replace("\r\n", "\n").replace("\r", "\n")
    return text


def apply_regex_override(text: str, override: str, label: str) -> str:
    groups = override[1:].split("/")
    if len(groups) < 2: return text
    try:
        result, count = re.subn(groups[0], groups[1], text, flags=(re.IGNORECASE | re.MULTILINE | re.DOTALL))
        if count == 0:
            print(f"WARN {label}: No match for regex in override text: {groups[0]!r} -- using AC text unmodified")
            return text
        return result
    except re.error as e:
        print(f"WARN {label}: override_text {override!r} isn't a usable regex ({e}) -- using AC text unmodified")
        return text


def resolve_text(row: dict, ac_data: dict[str, tuple]) -> str:
    override = (row.get("override_text") or "").strip()
    regex_mode = override.startswith("/")
    if override and not regex_mode:
        return normalize_linebreaks(override)

    text = find_ac_entry(ac_data, row.get("ac_bank", ""), row.get("ac_index", "")).strip()
    if regex_mode:
        text = apply_regex_override(text, override, row.get('af_index', ''))
    text = text.replace("Memory Card", "Controller Pak")
    return normalize_linebreaks(text)

def build_item_bank(vanilla: bytes) -> bytearray:
    """Re-lay the vanilla item bank out as a flat grid of ITEM_NAME_LEN-byte entries, indexed by af_index."""
    bank = bytearray(b" " * (ITEM_BANK_HEADER + ITEM_BANK_ENTRIES * ITEM_NAME_LEN))
    bank[:ITEM_BANK_HEADER] = vanilla[:ITEM_BANK_HEADER]
    for base, count in ITEM_VANILLA_CATEGORIES:
        for local in range(count):
            old = base + local * ITEM_VANILLA_LEN
            new = ITEM_BANK_HEADER + (item_slot(base) + local) * ITEM_NAME_LEN
            bank[new:new + ITEM_VANILLA_LEN] = vanilla[old:old + ITEM_VANILLA_LEN]
    return bank


def build_npc_bank(vanilla: bytes) -> bytearray:
    """Re-lay the vanilla NPC name bank out as a flat grid of NPC_NAME_LEN-byte entries."""
    bank = bytearray(b" " * (NPC_BANK_HEADER + NPC_BANK_ENTRIES * NPC_NAME_LEN))
    bank[:NPC_BANK_HEADER] = vanilla[:NPC_BANK_HEADER]
    for i in range(NPC_BANK_ENTRIES):
        old = NPC_BANK_HEADER + i * NPC_VANILLA_LEN
        if old + NPC_VANILLA_LEN > len(vanilla):
            break
        new = NPC_BANK_HEADER + i * NPC_NAME_LEN
        bank[new:new + NPC_VANILLA_LEN] = vanilla[old:old + NPC_VANILLA_LEN]
    return bank


def af_process_bank(name: str, assets_dir: Path, rows: list[dict], ac_data: dict[str, tuple]):
    data_codeword, table_codeword, carrier, entry_size, skip_bytes, count = AF_BANKS[name]
    data_path = assets_dir / f"{segname(data_codeword)}.bin"
    if not data_path.exists():
        print(f"[!] {name}: {data_path} not found -- run `make extract` first. Skipping.")
        return None

    if table_codeword is not None:
        table_path = assets_dir / f"{segname(table_codeword)}.bin"
        if not table_path.exists():
            print(f"[!] {name}: {table_path} not found -- run `make extract` first. Skipping.")
            return None

        table_bytes = table_path.read_bytes()
        backup_dir = assets_dir / AF_TRANSLATION_BACKUP_DIR
        backup_dir.mkdir(exist_ok=True)
        table_backup_path = backup_dir / table_path.name
        data_backup_path = backup_dir / data_path.name
        needs_data_backup = data_path.name == f"{AF_CARRIERS['shared']['segment']}.bin"

        linker_path = assets_dir.parent.parent / AF_LINKER_SCRIPT
        linker_symbol = f"D_{data_codeword:X}"
        symbol_match = None
        if linker_path.exists():
            symbol_match = re.search(
                rf"^{re.escape(linker_symbol)}\s*=\s*0x([0-9A-Fa-f]+)\s*;",
                linker_path.read_text(encoding="utf-8"), re.MULTILINE,
            )

        injected = symbol_match and int(symbol_match.group(1), 16) != data_codeword

        if injected:
            if not table_backup_path.exists():
                raise RuntimeError(
                    f"{name} appears to have already been injected, but its original offset table "
                    f"is missing. Run `make extract` before running the injector again."
                )
            if needs_data_backup and not data_backup_path.exists():
                raise RuntimeError(
                    f"{name} appears to have already been injected, but its original data "
                    f"is missing. Run `make extract` before running the injector again."
                )
        else:
            table_backup_path.write_bytes(table_bytes)
            if needs_data_backup:
                data_backup_path.write_bytes(data_path.read_bytes())

        original_table = table_backup_path.read_bytes()
        original_data = data_backup_path.read_bytes() if needs_data_backup else data_path.read_bytes()
        if injected:
            print(f"  using original offset table from {table_backup_path}")
            if needs_data_backup:
                print(f"  using original data from {data_backup_path}")

        offsets = read_offset_table(original_table)
        entries = dict(slice_entries(original_data, offsets))
        n_ok = n_skip = 0
        for row in rows:
            try:
                if not should_inject(row):
                    continue
                text = resolve_text(row, ac_data)
            except ValueError as e:
                print(f"WARN {name}: leaving original -- {e}")
                n_skip += 1
                continue

            af_index_raw = (row.get("af_index") or "").strip()
            if not text or not af_index_raw:
                continue
            try:
                af_index = int(af_index_raw)
            except ValueError:
                print(f"WARN {name}: af_index {af_index_raw!r} isn't a number, skipping")
                continue

            force = (row.get("override_text") or "").strip().startswith("/force")
            encoded = encode_row(text, entries.get(af_index), f"{name} af_index {af_index}", allow_jump_mismatch=force)
            if encoded is None:
                n_skip += 1
                continue
            entries[af_index] = encoded
            n_ok += 1

        table_len = max(len(offsets), (max(entries) + 1) if entries else len(offsets))
        new_data, new_table = pack_raw_entries(entries, table_len)
        print(f"\n=== {name} === ({len(entries)} entries, table_len {len(offsets)}"
              f"{' -> ' + str(table_len) if table_len != len(offsets) else ''})")
        print(f"translated & injected: {n_ok}  |  left as-is: {n_skip}")
        print(f"new data size: {len(new_data)} bytes (was {len(original_data)})")
        return {"new_data": new_data, "table_path": table_path, "new_table": new_table}

    if name == "item_1xxx":
        new_data = build_item_bank(data_path.read_bytes())
    elif name == "npc_name_str":
        new_data = build_npc_bank(data_path.read_bytes())
    else:
        new_data = bytearray(data_path.read_bytes())
    print(f"\n=== {name} === (fixed-width, {count} entries, {entry_size} bytes each)")
    n_ok = n_skip = 0
    for row in rows:
        try:
            if not should_inject(row):
                continue
            text = resolve_text(row, ac_data)
        except ValueError as e:
            print(f"WARN {name}: leaving original -- {e}")
            n_skip += 1
            continue

        af_index_raw = (row.get("af_index") or "").strip()
        if not text or not af_index_raw:
            continue
        try:
            af_index = int(af_index_raw)
        except ValueError:
            print(f"WARN {name}: af_index {af_index_raw!r} isn't a number, skipping")
            continue
        if af_index >= count:
            print(f"WARN {name} af_index {af_index}: beyond this bank's {count}-entry bound, skipping")
            n_skip += 1
            continue

        try:
            encoded = encode_af_entry(text)
        except ValueError as e:
            print(f"WARN {name} af_index {af_index}: leaving original -- {e}")
            n_skip += 1
            continue
        off = skip_bytes + af_index * entry_size
        if len(encoded) > entry_size:
            print(f"WARN {name} af_index {af_index}: {text!r} is {len(encoded)} bytes, truncating to fit the fixed {entry_size}-byte field")
            encoded = encoded[:entry_size]
        new_data[off:off + entry_size] = encoded.ljust(entry_size, b" ")
        n_ok += 1

    print(f"translated & injected: {n_ok}  |  left as-is: {n_skip}")
    return {"new_data": bytes(new_data)}


def patch_linker_symbol(af_repo: Path, old_codeword: int, new_codeword: int, bank_name: str):
    symbol = f"D_{old_codeword:X}"
    pattern = re.compile(rf"^{re.escape(symbol)}\s*=\s*0x[0-9A-Fa-f]+\s*;.*$", re.MULTILINE)
    path = af_repo / "linker_scripts" / "jp" / "auto" / "undefined_syms_auto.ld"
    if not path.exists():
        print(f"  [ld-patch] WARNING: {path} not found -- run `make extract` first.")
        return
    text = path.read_text(encoding="utf-8")
    if not pattern.search(text):
        print(f"  [ld-patch] WARNING: no '{symbol} = 0x...;' line found in {path}, leaving it untouched.")
        return
    replacement = f"{symbol} = 0x{new_codeword:X}; // moved {bank_name} (text_injector.py)"
    path.write_text(pattern.sub(replacement, text, count=1), encoding="utf-8")
    print(f"  [ld-patch] set {symbol} = 0x{new_codeword:X} ({bank_name})")


def main():
    if len(sys.argv) != 4:
        sys.exit(f"usage: python3 {sys.argv[0]} path/to/translation_sheet.tsv path/to/ac-files path/to/afrepo")

    translation_sheet, ac_files_dir, af_repo = map(Path, sys.argv[1:])
    assets_dir = af_repo / "assets" / "jp"

    with open(translation_sheet, newline="", encoding="utf-8-sig") as f:
        all_rows = list(csv.DictReader(f, delimiter="\t", quoting=csv.QUOTE_NONE))
    rows_by_bank: dict[str, list[dict]] = {}
    for row in all_rows:
        rows_by_bank.setdefault(row.get("af_bank", ""), []).append(row)

    ac_data = load_ac_banks(ac_files_dir)
    try:
        results = {
            name: af_process_bank(name, assets_dir, rows_by_bank.get(name, []), ac_data)
            for name in AF_BANKS
        }
    except RuntimeError as e:
        print(f"ERROR: {e}")
        sys.exit(1)

    failed = False
    ld_patches = []
    shared = AF_CARRIERS["shared"]
    shared_carrier = bytearray(shared["size"])
    cursor = 0

    for name, (old_codeword, _, carrier, _, _, _) in AF_BANKS.items():
        result = results[name]
        if result is None:
            continue
        new_data = result["new_data"]

        if carrier == "dialogue":
            message = AF_CARRIERS["dialogue"]
            total_size = message["end"] - message["codeword"]
            budget = total_size - message["offset"]
            if len(new_data) > budget:
                print(f"ERROR: translated {name} ({len(new_data)} bytes) doesn't fit in "
                      f"{message['segment']}'s translated budget ({budget} bytes). Not writing {name}.")
                failed = True
                continue
            carrier_path = assets_dir / f"{message['segment']}.bin"
            carrier_real = carrier_path.read_bytes()[:message["real_size"]] if carrier_path.exists() else b""
            combined = (carrier_real.ljust(message["offset"], b"\0") + new_data).ljust(total_size, b"\0")
            carrier_path.write_bytes(combined)
            print(f"wrote {carrier_path}")
            new_codeword = message["codeword"] + message["offset"]
        else:
            start = (cursor + 0xFFF) & ~0xFFF
            end = start + len(new_data)
            if end > shared["size"]:
                print(f"ERROR: {name} needs {len(new_data)} bytes but only {shared['size'] - start} are left in "
                      f"{shared['segment']}. Not writing {name}.")
                failed = True
                continue
            shared_carrier[start:end] = new_data
            cursor = end
            new_codeword = shared["codeword"] + start

        if "table_path" in result:
            result["table_path"].write_bytes(result["new_table"])
            print(f"wrote {result['table_path']}")
        ld_patches.append((old_codeword, new_codeword, name))

    shared_path = assets_dir / f"{shared['segment']}.bin"
    shared_path.write_bytes(shared_carrier)
    print(f"\nwrote {shared_path} ({cursor} of {shared['size']} bytes used)")

    print()
    for old_codeword, new_codeword, name in ld_patches:
        patch_linker_symbol(af_repo, old_codeword, new_codeword, name)

    if failed:
        print("\nOne or more banks were NOT written -- see the ERROR lines above. Everything else above was still written.")
    print(f"\nNow run `make` in {af_repo} to build a ROM with these entries.")
    if failed:
        sys.exit(1)


if __name__ == "__main__":
    main()