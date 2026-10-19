# Animal Forest - English Translation

This is a work-in-progress translation project for the Nintendo 64 game *Animal Forest*, also known as どうぶつの森 (*Doubutsu no Mori*). It is based on the previous work by [ZoinKity](https://www.romhacking.net/translations/1581/) and the [partial decomp of Animal Forest](https://github.com/zeldaret/af). Since Animal Crossing for the Nintendo Gamecube contains an official translation of almost all of the lines in Animal Forest, we are porting those English lines over to the N64 original.

## Translation Progress
All strings inside the text banks that have an equivalent in Animal Crossing have been [matched](https://github.com/oprel/af/blob/main/link_sheet.csv) (19,500 out of ~20,000 strings/dialogues). Some might need fixing to reflect the functionality of Animal Forest.

### To-do
- [ ] Extend/compress save file to allow for longer mail.
- [ ] Allow for dialogue entries longer than 1024 bytes.
- [ ] Support for longer catchphrases.
- [ ] Implement keyboard-style text input from Animal Crossing.
- [ ] Improve kerning on font.

Ongoing:
- [ ] Translate strings hardcoded throughout the codebase.
- [ ] Translate lines related to the N64 Controller Pak.
- [ ] Locate and translate Japanese strings not present in the text banks.
- [ ] Fix places where Animal Crossing text doesn't match Animal Forest behaviour.

### Done
- [x] Extract English text from Animal Crossing (Gamecube).
- [x] Inject English text into banks.
  - Dialogue
  - Items
  - Character Names
  - Mail
  - Misc.
- [x] Support for longer choice strings in dialogue.
- [x] Variable width font.
- [x] Inject translated sprites from ZoinKity translation.
- [x] Allow for item names up to 16 characters long.
- [x] Allow for NPC names up to 8 characters long.
- [x] Default to English character input during text input.
- [x] Fix the way time/date are displayed.


#### Helping out
I am currently working on this solo, but if you want to help out you can reach me in the [Animal Crossing Modding Discord](https://discord.gg/dRWBVdjYGK)! If you are technically minded and/or have experience decompiling, your help would be most welcome!

#### AI Disclaimer
I am a video game developer who isn't particularly excited about AI and the future it might have in my profession. However, as it seems more and more likely it's here to stay I felt the need to get familiar with using it as a tool. I'm still doing most of this by hand, I just have a free Claude account provide assistance (python injector, matching strings across different games, decompiling new parts of the code).
