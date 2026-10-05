Import("env")

import re
import shutil
import subprocess
from datetime import datetime, timezone
from pathlib import Path


def format_bar(percent, width=10):
    filled = max(0, min(width, int(round((percent / 100.0) * width))))
    return "[" + "=" * filled + " " * (width - filled) + "]"


def detect_size_tool():
    candidates = [
        shutil.which("riscv-wch-elf-size"),
        shutil.which("riscv-none-embed-size"),
        shutil.which("riscv64-unknown-elf-size"),
        shutil.which("xtensa-esp32-elf-size"),
        shutil.which("xtensa-esp8266-elf-size"),
        shutil.which("arm-none-eabi-size"),
        shutil.which("avr-size"),
    ]
    for candidate in candidates:
        if candidate:
            return candidate

    project_root = Path(env.subst("$PROJECT_DIR")).resolve()
    tool_root = project_root / ".pio" / "packages"
    for base in [
        tool_root / "toolchain-riscv" / "bin",
        tool_root / "toolchain-xtensa-esp32" / "bin",
        tool_root / "toolchain-xtensa-esp8266" / "bin",
        tool_root / "toolchain-xtensa-lx106" / "bin",
    ]:
        if not base.exists():
            continue
        for path in sorted(base.iterdir()):
            name = path.name.lower()
            if name.endswith("size.exe") or name.endswith("size"):
                return str(path)

    raise RuntimeError("Could not find a size tool for this PlatformIO environment.")


def read_total_memory_from_board(board_name):
    board_sizes = {
        "genericCH32V003A4M6": {"ram": 2048, "flash": 16384},
        "esp32dev": {"ram": 327680, "flash": 1900544},
        "nodemcuv2": {"ram": 81920, "flash": 1048576},
    }
    return board_sizes.get(board_name, {"ram": 2048, "flash": 16384})


def main():
    project_root = Path(env.subst("$PROJECT_DIR")).resolve()
    build_dir = project_root / env.subst("$BUILD_DIR")
    elf_path = build_dir / "firmware.elf"

    if not elf_path.exists():
        return

    size_tool = detect_size_tool()
    result = subprocess.run(
        [size_tool, "-A", str(elf_path)],
        capture_output=True,
        text=True,
        check=True,
    )

    sections = {
        "init": 0,
        "vector": 0,
        "text": 0,
        "fini": 0,
        "data": 0,
        "bss": 0,
        "stack": 0,
    }

    for line in result.stdout.splitlines():
        parts = line.strip().split()
        if len(parts) < 2 or not parts[0].startswith("."):
            continue
        name = parts[0].lower()
        if name not in {".init", ".vector", ".text", ".fini", ".data", ".bss", ".stack"}:
            continue
        value = int(parts[1], 0)
        sections[name[1:]] = value

    flash_used = sections["init"] + sections["vector"] + sections["text"] + sections["data"] + sections["fini"]
    ram_used = sections["data"] + sections["bss"] + sections["stack"]

    board_name = env.get("BOARD", "genericCH32V003A4M6")
    limits = read_total_memory_from_board(board_name)
    ram_total = limits["ram"]
    flash_total = limits["flash"]

    ram_percent = (ram_used / ram_total) * 100.0
    flash_percent = (flash_used / flash_total) * 100.0

    block = (
        'Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"\n'
        f"RAM:   {format_bar(ram_percent)}  {ram_percent:5.1f}% (used {ram_used} bytes from {ram_total} bytes)\n"
        f"Flash: {format_bar(flash_percent)}  {flash_percent:5.1f}% (used {flash_used} bytes from {flash_total} bytes)\n"
    )

    memory_usage_path = project_root / "memory_usage"
    memory_usage_path.write_text(block, encoding="utf-8")


main()
