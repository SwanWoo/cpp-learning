#!/bin/bash

BASE_DIR=$(pwd)/src

# 定义章节目录和文件
declare -A files=(
  ["basic"]="hello"
  ["algo"]="sort"
  ["system"]="thread"
)

for dir in "${!files[@]}"; do
  mkdir -p "$BASE_DIR/$dir"
  cpp_file="$BASE_DIR/$dir/${files[$dir]}.cpp"

  if [ ! -f "$cpp_file" ]; then
    cat > "$cpp_file" <<EOF
#include <iostream>

int main() {
    std::cout << "Running: $dir/${files[$dir]}.cpp" << std::endl;
    return 0;
}
EOF
    echo "Created: $cpp_file"
  else
    echo "Skipped: $cpp_file (already exists)"
  fi
done

