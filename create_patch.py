from pathlib import Path
import pyxdelta

root = Path(__file__).resolve().parent
pyxdelta.run(
    str(root / "baseroms/jp/baserom.n64"),
    str(root / "build/animalforest-jp-compressed.z64"),
    str(root / "animalforest-en.xdelta"),
)